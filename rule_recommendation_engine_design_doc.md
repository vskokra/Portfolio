# Rule Recommendation Engine: Design & Technical Deep Dive

**Project:** AI-powered rule generation for bank transaction reconciliation at Carta  
**Timeframe:** May 2025 - Aug 2025  
**Role:** AI/ML Engineer, led end-to-end design and implementation  
**Key Metric:** 90%+ recommendation accuracy

---

## 1. Problem Statement

### Context
Carta processes millions of bank transactions daily requiring metadata tagging: category, vendor, partner, amount range, etc. Existing rules cover only ~<15% of transactions; the rest require manual human reconciliation.

**Business Impact:**
- Initial coverage: <15% (85% of transactions require manual review)
- Thousands of transactions exceed rule coverage
- Manual review is expensive and slow
- Accuracy is critical (financial metadata errors cascade)
- Bottleneck: human reconciliation time

**Assignment:** Build a system to automatically generate rules from patterns, increasing coverage while maintaining accuracy.

---

## 2. Solution Evolution: Three Iterations

### V1: Direct Transaction Labeling via Similar Memo Patterns

**Design:**
- Retrieve 20 semantically similar transactions (RAG over all transactions)
- Analyze how those similar transactions were labeled by humans
- Apply those labels directly to the new transaction
- User reviews the suggestion

**Architecture:**
```
New Transaction → Semantic Search (all transactions)
                → Retrieve 20 similar
                → See how they're labeled
                → Apply labels directly
                → User Reviews
```

**Result:** 12-13% accuracy  
**Status:** Failed experiment

**Why It Failed:**
- Low accuracy meant users couldn't trust suggestions
- Barely reduced human work: users still had to manually review every transaction
- Even with suggestions, going through thousands of transactions one-by-one is expensive
- No leverage: labeling individual transactions doesn't help the next transaction

**Key Insight:** Labeling individual transactions one-at-a-time doesn't scale. Need to extract *reusable rules* that apply to many transactions at once.

---

### V2: Pattern Extraction → Rule Generation (The Pivot)

**The Pivot:** Instead of labeling transactions individually, extract *reusable rules* from patterns that apply to many transactions at once.

**Design:**
- Retrieve 20 similar transactions from manually-reconciled set only
- Extract patterns from those transactions
- Use LLM to infer *rules* from the patterns (not just label this one transaction)
- User reviews the rule, accepts it, applies it to all matching transactions

**Architecture:**
```
New Transaction → Semantic Search (reconciled transactions only)
                → Retrieve 20 similar
                → Pattern Extraction (LLM) → GENERATE RULE
                → User Reviews Rule & Accepts
                → Rule applies to ALL matching transactions (not just this one)
```

**Result:** 40-45% user acceptance rate for rules  
**Status:** Improved, but insufficient

**Problem:** 
- High volume of low-quality rules
- Rules conflicted with each other
- Many rules didn't actually work on the candidate set they were extracted from
- Users overwhelmed; accuracy of accepted rules still uncertain

**Technical Gap Identified:** No verification step—rules generated without testing they work on their own candidate set.

---

### V3: Rule Verification + Conflict Detection + Rejection Signals

**Design:** Multi-layer verification pipeline before rule surfaces to users.

**Architecture:**
```
New Transaction → Semantic Search (reconciled transactions only)
                → Retrieve 20 similar
                → Pattern Analysis (LLM)
                → Generate Rule
                    ↓
                ╔═══════════════════════════════╗
                ║ VERIFICATION LAYER (NEW)      ║
                ╠═══════════════════════════════╣
                ║ 1. Rule Verification          ║
                ║    - Test rule against        ║
                ║      candidate set            ║
                ║    - Accept if ≥X% coverage   ║
                ║                               ║
                ║ 2. Conflict Detection         ║
                ║    - Check vs. existing rules ║
                ║    - Detect overlaps          ║
                ║                               ║
                ║ 3. Rejection Analysis         ║
                ║    - Use rejected rules as    ║
                ║      negative signal          ║
                ║                               ║
                ║ 4. Context & Reasoning        ║
                ║    - Surface LLM reasoning    ║
                ║    - Estimate impact volume   ║
                ╚═══════════════════════════════╝
                ↓
            [Filtered Rules]
                ↓
            User Reviews
            (Higher Quality)
```

**Component Details:**

#### Rule Verification Layer
Test rule against the 20 candidate transactions before surfacing:
- Does the rule actually work on transactions it was extracted from?
- What's the coverage percentage?
- Which transactions does it misclassify?
- Only surface if coverage ≥ threshold (e.g., 80%)

#### Conflict Detection
Check new rule against existing rule set:
- Does it conflict with existing rules (both apply to same transaction, produce different labels)?
- Does it overlap (partial applicability)?
- Use subset/superset analysis on rule conditions (see Section 4)

#### Rejection Signal
When users reject a rule:
- Treat rejection as negative signal
- Check if rejected rule would've caused conflicts
- Use to inform why it was rejected
- Prevents resuggesting similar patterns

#### Reasoning & Context
Surface to users:
- Why was this rule generated? (LLM reasoning)
- How many transactions in candidate set does it cover?
- Estimated impact on full unreconciled dataset
- Reasoning helps users make faster, more confident decisions

**Result:** >90% recommendation accuracy  
**Status:** Production-grade system

---

## 3. Concrete Examples: OpenAI vs. Burger King

### Example 1: OpenAI (High-Confidence Rule)

**Manually-Reconciled Transactions:**
```
Transaction A1:
  Bank Memo: #OPENai34bdasdwde
  Amount: $20
  Label: Category=AI Subscription, Vendor=OpenAI

Transaction A2:
  Bank Memo: #openai_sep_charge
  Amount: $29.99
  Label: Category=AI Subscription, Vendor=OpenAI

Transaction A3:
  Bank Memo: #OPENai_quarterly_sub
  Amount: $19
  Label: Category=AI Subscription, Vendor=OpenAI

Transaction A4:
  Bank Memo: #OPEN_invoice_office
  Amount: $150
  Label: Category=Business Services, Vendor=Office Vendor
```

**New Incoming Transaction:**
```
Bank Memo: #OPENai_monthly
Amount: $21.50
Label: ???
```

**V3 System Flow:**

1. **Retrieve Similar:**
   - Semantic search on "#OPENai_monthly"
   - Returns 20 most similar reconciled transactions (A1, A2, A3 rank highest)

2. **Pattern Extraction (LLM):**
   ```
   Observed Pattern:
   - Memos: All contain "openai" (case-insensitive)
   - Labels: All tagged as AI Subscription/OpenAI
   - Amount: Consistent range $15-$35
   - Frequency: Monthly recurring charges
   
   Proposed Rule:
   IF memo CONTAINS "openai" (case-insensitive)
      AND amount BETWEEN $15-$35
   THEN category = "AI Subscription", vendor = "OpenAI"
   ```

3. **Verification (Critical Step):**
   ```
   Test against candidate set (20 similar transactions):
   
   ✓ A1 (#OPENai34bdasdwde, $20) → MATCHES → AI Subscription/OpenAI ✓
   ✓ A2 (#openai_sep_charge, $29.99) → MATCHES → AI Subscription/OpenAI ✓
   ✓ A3 (#OPENai_quarterly_sub, $19) → MATCHES → AI Subscription/OpenAI ✓
   ✓ A18 (similar transaction) → MATCHES → AI Subscription/OpenAI ✓
   
   Edge Cases Handled:
   ✗ A4 (#OPEN_invoice_office, $150) → MATCHES memo BUT amount=$150 > $35
      → Correctly NOT labeled by rule
   
   Pass Rate: 18/20 (90% coverage) ✓ ABOVE THRESHOLD
   ```

4. **User Presentation:**
   ```
   ┌─────────────────────────────────────────────────────┐
   │ NEW RULE RECOMMENDATION (90% Confidence)            │
   ├─────────────────────────────────────────────────────┤
   │ Rule: MemoContains("openai") AND                    │
   │       AmountRange($15-$35)                          │
   │                                                     │
   │ → Label as:                                         │
   │   Category: AI Subscription                         │
   │   Vendor: OpenAI                                    │
   │                                                     │
   │ Coverage: Covers 18/20 similar reconciled           │
   │           transactions (90%)                        │
   │                                                     │
   │ Reasoning:                                          │
   │ Pattern: Subscription charges with "openai"         │
   │ in memo, consistent $15-$35 monthly range.          │
   │ Excludes outliers (e.g., $150 office invoice).     │
   │                                                     │
   │ Estimated Impact:                                   │
   │ ~47 unreconciled transactions match pattern         │
   │ ~42 would be labeled by this rule (89%)             │
   │                                                     │
   │ [ACCEPT] [REJECT] [MODIFY]                          │
   └─────────────────────────────────────────────────────┘
   ```

**Outcome:** Rule accepted, applied to future transactions.

---

### Example 2: Burger King (Low-Confidence Rule - Rejected by System)

**Manually-Reconciled Transaction:**
```
Transaction B1:
  Bank Memo: #BK237853527y3
  Amount: $78.50
  Label: Category=Food & Beverage, Vendor=Burger King
```

**New Incoming Transaction:**
```
Bank Memo: #BK_9384729x
Amount: $82.00
Label: ???
```

**Why Burger King is Hard:**
- Memo is an opaque transaction reference code, not readable text
- "BK" is ambiguous (Burger King? Bank? Something else?)
- No semantic signal in the memo itself
- Pattern extraction has insufficient evidence

**V3 System Behavior:**

1. **Pattern Extraction (LLM attempts):**
   ```
   Observed Pattern (limited signal):
   - Memos start with "BK"
   - Amount in $70-$85 range
   - Some labeled as Food & Beverage
   
   Proposed Rule (low confidence):
   IF memo STARTS_WITH "BK"
      AND amount BETWEEN $70-$85
   THEN category = "Food & Beverage", vendor = "Burger King"
   ```

2. **Verification (Catches the Problem):**
   ```
   Test against candidate set:
   
   ✓ B1 (#BK237853527y3, $78.50) → MATCHES → Food & Beverage/BK ✓
   
   But in the 20 similar reconciled transactions:
   ✗ B5 (#BK_invoice_software, $79.99) → MATCHES memo+amount
      BUT labeled as Software/Vendor X (CONFLICT) ✗
   ✗ B12 (#BK_legal_retainer, $75.00) → MATCHES memo+amount
      BUT labeled as Legal Services/Law Firm (CONFLICT) ✗
   ✗ B18 (#BK_misc_vendor, $80.00) → MATCHES memo+amount
      BUT labeled as Other/Vendor Z (CONFLICT) ✗
   
   Pass Rate: 1/20 (5% accuracy) ✗ BELOW THRESHOLD (need ≥80%)
   
   Conflict Count: 3 conflicting labels
   ```

3. **System Decision:**
   ```
   Rule REJECTED by verification layer.
   
   Reason Shown to LLM:
   "This rule would conflict with 3 existing reconciliations. 
    The 'BK' pattern is too ambiguous to reliably indicate 
    Burger King. Recommend manual review."
   ```

4. **User Experience:**
   - User never sees the low-confidence rule
   - No confusion, no false positive acceptance
   - System avoids polluting rule set with unreliable patterns

**Outcome:** Rule never surfaced to user. Burger King transaction requires manual review.

---

### Comparison: Why Verification Matters

| Aspect | OpenAI | Burger King |
|--------|--------|------------|
| **Signal Quality** | Strong (readable memo text) | Weak (opaque reference code) |
| **Pattern Clarity** | Clear (consistent memo + amount range) | Ambiguous ("BK" could mean anything) |
| **Verification Pass Rate** | 90% | 5% |
| **System Decision** | Surface to user | Reject before user sees |
| **User Benefit** | High-confidence rule applied automatically | Avoids false positive suggestion |
| **False Positive Risk** | Low | High |

---

## 4. Rule Overlap Detection: Linear-Time Algorithm

### The Problem

Initially, overlap detection was delegated to another team's service. As rule volume scaled, bottlenecks emerged:
- Checking every new rule against all existing rules was computationally expensive
- Naive approach: O(n²) rule comparisons or worse
- Couldn't prevent future conflicts by design
- Dependency on external service blocked iteration

### Key Insight

Rule overlap is fundamentally a **set relationship problem**, not a transaction-by-transaction problem.

**Two Rules Overlap If:**
- Rule A's conditions are a subset of Rule B's conditions (A is more specific)
- Rule B's conditions are a subset of Rule A's conditions (B is more specific)
- Their conditions have partial overlap (conflict risk)

### Solution: Subset/Superset Analysis

Instead of checking transactions, model rules as condition sets and analyze relationships:

**Rule Representation:**
```python
Rule A:
  conditions = {
    "memo_contains": ["openai"],
    "amount_range": [15, 35]
  }
  outcome = "AI Subscription / OpenAI"

Rule B:
  conditions = {
    "amount_range": [20, 30]
  }
  outcome = "AI Subscription / OpenAI"
```

**Overlap Analysis:**
```
Is Rule B's condition set a subset of Rule A's?
  → Rule B requires: amount in [20, 30]
  → Rule A requires: memo contains "openai" AND amount in [15, 35]
  → NO. Rule B has no memo requirement.

Is Rule A's condition set a subset of Rule B's?
  → Rule A requires: memo contains "openai" AND amount in [15, 35]
  → Rule B requires: amount in [20, 30]
  → NO. Rule A has memo requirement Rule B lacks.

Partial Overlap?
  → Both apply to transactions with amount in [20, 30]
  → Rule A: also requires "openai" in memo
  → Risk: Transaction with memo="#openai_charge", amount=$25
    - Rule A matches: YES
    - Rule B matches: YES
    - Both label as "AI Subscription/OpenAI" (consistent in this case)
  → Flag for review: overlapping applicability

Recommendation:
  [CONFLICT] Rules A and B have overlapping scope. Review consistency.
```

### Algorithm Complexity

**Naive Approach (Transaction-Level):**
- For each new rule: scan all transactions
- For each transaction: check if it matches new rule and existing rules
- Complexity: O(num_rules × num_transactions)
- Infeasible at scale

**Optimized Approach (Condition-Level):**
- For each new rule: compare conditions against all existing rules
- For each comparison: subset/superset check on condition structures
- Complexity: O(num_rules²) rule pairs, linear per comparison
- One-time cost, not per-transaction

**Benefits:**
- Scales with rule count, not transaction volume
- Prevents future conflicts by design
- Cheaper than transaction-level checks
- Deterministic (rule logic doesn't change mid-day)

### Implementation Details

**Condition Matching Logic:**
```python
def rules_overlap(rule_a, rule_b):
    """
    Check if two rules have overlapping applicability.
    Returns: overlap_type, affected_conditions
    """
    # Extract condition structures
    conditions_a = rule_a.conditions  # dict of constraints
    conditions_b = rule_b.conditions
    
    # Find overlapping constraints
    overlap = {}
    for field, constraint_a in conditions_a.items():
        if field in conditions_b:
            constraint_b = conditions_b[field]
            # Check if constraints have intersection
            if constraints_intersect(constraint_a, constraint_b):
                overlap[field] = True
    
    # Classification
    if len(overlap) == len(conditions_a) == len(conditions_b):
        return "EXACT_OVERLAP"  # Same rule, different outcome?
    elif len(overlap) == len(conditions_a):
        return "A_SUBSET_OF_B"   # A is more specific
    elif len(overlap) == len(conditions_b):
        return "B_SUBSET_OF_A"   # B is more specific
    elif len(overlap) > 0:
        return "PARTIAL_OVERLAP" # Conflict risk
    else:
        return "DISJOINT"        # No overlap
```

---

## 5. Technical Architecture

### System Components

```
┌──────────────────────────────────────────────────────────────┐
│                    INCOMING TRANSACTION                      │
│              (Bank Memo, Amount, Other Metadata)             │
└────────────────────────────┬─────────────────────────────────┘
                             ↓
┌──────────────────────────────────────────────────────────────┐
│         1. SEMANTIC RETRIEVAL LAYER                          │
│  ────────────────────────────────────────────────────────────│
│  • Query embedding for bank memo                            │
│  • Vector DB search (reconciled transactions only)          │
│  • Retrieve top-20 similar transactions                     │
│  • Filter by relevance score                                │
└────────────────────────────┬─────────────────────────────────┘
                             ↓
┌──────────────────────────────────────────────────────────────┐
│         2. PATTERN EXTRACTION LAYER (LLM)                    │
│  ────────────────────────────────────────────────────────────│
│  • Analyze 20 similar reconciled transactions                │
│  • Identify common memo patterns                             │
│  • Extract amount ranges, constraints                        │
│  • Generate rule conditions in structured format             │
└────────────────────────────┬─────────────────────────────────┘
                             ↓
┌──────────────────────────────────────────────────────────────┐
│         3. VERIFICATION LAYER                                │
│  ────────────────────────────────────────────────────────────│
│  A. Rule Verification                                        │
│     • Test rule against 20 candidate transactions            │
│     • Calculate coverage %                                   │
│     • Accept if ≥80% (configurable threshold)              │
│                                                              │
│  B. Conflict Detection                                       │
│     • Compare vs. existing rules (subset/superset)          │
│     • Flag overlaps and inconsistencies                      │
│                                                              │
│  C. Rejection Signal Analysis                               │
│     • Check if rule pattern has been rejected before        │
│     • Assess viability before surfacing                      │
│                                                              │
│  D. Context Generation                                       │
│     • LLM reasoning for rule generation                      │
│     • Estimate impact volume on full dataset                 │
└────────────────────────────┬─────────────────────────────────┘
                             ↓
                    [PASS FILTERS?]
                         ↙      ↘
                       YES      NO
                        ↓        ↓
                   [PRESENT]  [REJECT]
                        ↓        ↓
┌──────────────────────────┐   ├──────────────────┐
│  USER REVIEW INTERFACE   │   │ STORE REJECTION  │
│ ──────────────────────── │   │ SIGNAL FOR       │
│ • Show rule & reasoning  │   │ FUTURE LEARNING  │
│ • Show coverage %        │   │                  │
│ • Show impact estimate   │   │ (Negative signal)│
│ • [ACCEPT/REJECT/MODIFY] │   └──────────────────┘
└──────────────┬───────────┘
               ↓
         [USER DECISION]
               ↓
    [STORE RULE / UPDATE REJECTION LOG]
```

### Technology Stack

**Retrieval:**
- Vector embeddings for bank memos (semantic similarity)
- Vector database for scalable nearest-neighbor search
- Filtered to manually-reconciled transactions (high-quality data)

**Pattern Extraction & Rule Generation:**
- LLM for inference (Claude or similar)
- Structured prompting to generate rule conditions in consistent format
- Context window includes: similar transactions, labels, memo patterns

**Verification & Conflict Detection:**
- Rule-condition parser (structured rules)
- Subset/superset analyzer for conflict detection
- Rejection log storage for negative signal learning

**Backend Integration:**
- Integrated into Carta's monolithic backend (existing infrastructure)
- Built reusable service abstractions for rule management
- API endpoints: generate_rule, verify_rule, detect_conflicts

**Data Storage:**
- Rule definitions: SQL (queryable, versioned)
- Embeddings: Vector database (fast retrieval)
- Rejection log: Time-series store (learning signal)

---

## 6. Implementation & Development Process

### Development Phases

**Phase 1: V1 Baseline (Week 1-2)**
- Implement naive RAG + pattern extraction
- Establish baseline metrics (12-13% accuracy)
- Identify failure modes: noisy ground truth, no verification

**Phase 2: V2 Refinement (Week 3-4)**
- Pivot to verified-data-only retrieval
- Integrate with Carta's reconciliation database
- Test on real transaction set; achieve 40-45% acceptance

**Phase 3: V3 Verification & Scaling (Week 5-8)**
- Build rule verification layer
- Implement conflict detection (initial brute-force)
- Test and refine thresholds
- Achieve >90% accuracy

**Phase 4: Optimization (Week 8)**
- Identify overlap detection bottleneck
- Develop linear-time subset/superset algorithm
- Replace brute-force with optimized approach
- Validate performance and coverage

### Testing Strategy

**Unit Tests:**
- Rule condition parsing (correct format extraction)
- Subset/superset analysis (correct conflict detection)
- Verification logic (coverage calculation)

**Integration Tests:**
- End-to-end flow: transaction → rule generation → verification → user display
- Real transaction data from Carta database
- Edge cases: ambiguous memos (Burger King), outliers (amount ranges)

**Validation:**
- Compare against manually-labeled test set
- Measure: accuracy, precision, recall, false positive rate
- Benchmark: V1 vs. V2 vs. V3 results
- User acceptance rate before/after verification layer

---

## 7. Results & Metrics

### Key Outcomes

| Metric | V1 | V2 | V3 |
|--------|----|----|-----|
| **Recommendation Accuracy** | 12-13% | 40-45% | >90% |
| **Rule Coverage** | <15% | ~15-20% | 35-40% |
| **User Acceptance Rate** | ~0% (not trusted) | 40-45% | Significantly higher |
| **False Positive Rate** | High | High | Low |
| **Rules in Production** | 0 | ~50-100 | 500+ |

### Impact

- **Accuracy:** >90% recommendation accuracy (vs. 12-13% initial)
- **Coverage Growth:** <15% initial → 35-40% final (2-3x improvement in rule coverage)
- **Reconciliation:** Rules now handle 35-40% of previously manual transactions
- **User Trust:** Verification layer provides transparency; users comfortable accepting recommendations
- **Scalability:** Optimized conflict detection supports 500+ rules without performance degradation
- **Time Saved:** Thousands of transactions per month auto-reconciled instead of manual review

---

## 8. Challenges & Lessons Learned

### Challenge 1: No Leverage (V1 Failure)

**Problem:**
- Labeling transactions individually by finding similar examples doesn't scale
- Even with 12-13% accuracy suggestions, users still had to manually review every single transaction
- Didn't reduce manual work: taking 10,000 transactions from "all manual" to "10,000 with suggestions" is barely different
- No multiplicative effect: labeling transaction A doesn't help with transaction B

**Solution:**
- Pivot from labeling individual transactions to *generating reusable rules*
- A single rule applies to hundreds of transactions at once
- This creates leverage: extract pattern once, apply to many

**Lesson:** Labeling one thing at a time doesn't scale. Extract patterns into rules that apply broadly.

---

### Challenge 2: Unverified Rules (V2 Gap)

**Problem:**
- Generated rules without testing them on their source data
- Many rules didn't actually work on the 20 transactions they were extracted from
- No feedback loop to catch bad patterns before user review

**Solution:**
- Add verification step: test rule against candidate set before surfacing
- Threshold-based acceptance (≥80% coverage)
- Rejection signal for learning

**Lesson:** Always verify recommendations before presenting. Verification is a feature, not overhead.

---

### Challenge 3: Rule Conflicts at Scale (V3 Performance)

**Problem:**
- Brute-force overlap detection checked every new rule against all transactions
- O(n²) complexity; bottleneck as rule count grew
- Blocked iteration; dependency on external service

**Solution:**
- Abstract to rule-level instead of transaction-level
- Subset/superset analysis on rule conditions
- Linear time per rule pair

**Lesson:** When you hit a scaling wall, step back and model the problem at a higher level of abstraction.

---

### Challenge 4: User Trust in Recommendations

**Problem:**
- Even high-accuracy rules didn't translate to user adoption
- Users want to understand *why* a rule was suggested
- Impact estimates matter for decision-making

**Solution:**
- Surface LLM reasoning
- Show coverage % on candidate set
- Estimate impact on full unreconciled dataset

**Lesson:** Explainability and context are as important as accuracy for user adoption.

---

## 9. What You'd Do Differently

1. **Verify Earlier:** Start with verification from V1. Don't wait until V3 to test rules on source data.

2. **Model Domain Problem First:** Spend time understanding rule logic structure (conditions, overlaps, subsets) before building. This leads faster to optimized algorithms.

3. **Rejection Signals:** Treat rejected rules as valuable negative signal from day one. Mine them for learning.

4. **User Studies:** Earlier validation with users on what information they need to trust recommendations (reasoning, impact, coverage).

5. **Threshold Tuning:** Verify coverage threshold (80% used here) earlier in development; it significantly impacts false positive rate.

---

## 10. Interview Talking Points

### Key Narrative Arcs

**Arc 1: From Noisy Data to Clean Signals**
- V1 failed because training on unverified transactions
- V2 succeeded by limiting to manually-reconciled data
- Signal quality determines outcome

**Arc 2: Verification is the Difference**
- Without verification: rules don't work on source data, user confusion
- With verification: only surface tested, high-confidence rules
- Why OpenAI rule works (90% coverage) but Burger King fails (5% coverage)

**Arc 3: Scaling Through Abstraction**
- Naive approach: check rule against every transaction
- Optimized approach: check rule relationships at rule level
- Subset/superset analysis scales with rule count, not transaction volume

### Likely Follow-Up Questions & Answers

**Q: Why didn't you use a different retrieval strategy?**  
A: Semantic search on manually-reconciled transactions gave us high-quality candidates. We validated this was the right signal source in V1→V2 pivot. Alternative: could have tried BM25 or keyword matching, but semantic similarity better captures patterns like "openai" variants.

**Q: How did you handle rule deprecation?**  
A: Tracked rule acceptance rate over time. If rule stopped matching incoming transactions (detection drift), flagged for review. Also accepted user feedback on individual rule applications.

**Q: What about the Burger King case—how would you improve that?**  
A: That's a signal problem. Opaque reference codes don't have semantic content. Improvements: (1) Integrate external data (merchant category codes, vendor databases) to enrich memos, (2) Lower coverage threshold for high-confidence partial patterns, (3) Multi-field rules (use amount + date patterns together for stronger signal).

**Q: Why didn't you fine-tune an LLM instead?**  
A: Didn't need to. Prompting + structured retrieval was sufficient for >90% accuracy. Fine-tuning would've added latency and complexity without clear benefit. Rule generation is few-shot reasoning, not distribution learning.

**Q: How did you validate the algorithm improvements?**  
A: Benchmarked old vs. new conflict detection on real rule set. Measured: query time, memory, correctness (all conflicts detected). New algorithm: O(rules²) vs. O(rules × transactions) for old approach. Faster and more predictable.

---

## Appendix: Code-Level Details (Optional)

### Rule Representation (Structured Format)

```json
{
  "rule_id": "rule_012345",
  "created_at": "2025-05-15T10:30:00Z",
  "generated_from": {
    "source_transactions": [
      "txn_001", "txn_002", "txn_003"
    ],
    "candidate_count": 20,
    "coverage_percent": 90
  },
  "conditions": {
    "memo": {
      "type": "contains",
      "values": ["openai"],
      "case_sensitive": false
    },
    "amount": {
      "type": "range",
      "min": 15,
      "max": 35
    }
  },
  "outcome": {
    "category": "AI Subscription",
    "vendor": "OpenAI"
  },
  "confidence_score": 0.92,
  "estimated_impact": {
    "unreconciled_matches": 47,
    "expected_coverage": 0.89
  },
  "conflicts": [],
  "user_accepted": true,
  "acceptance_date": "2025-05-15T11:00:00Z"
}
```

### Verification Pseudocode

```python
def verify_rule(rule, candidate_transactions):
    """
    Test rule against candidate set.
    Returns: (pass, coverage_percent, mismatch_details)
    """
    matches = 0
    mismatches = []
    
    for txn in candidate_transactions:
        # Test rule conditions
        if rule.matches(txn):
            # Check if outcome matches user-applied label
            if txn.label == rule.outcome:
                matches += 1
            else:
                mismatches.append({
                    'txn_id': txn.id,
                    'rule_prediction': rule.outcome,
                    'actual_label': txn.label
                })
    
    coverage = matches / len(candidate_transactions)
    threshold = 0.80  # 80% required
    
    return {
        'passed': coverage >= threshold,
        'coverage_percent': coverage * 100,
        'mismatches': mismatches
    }
```

---

## Summary

The Rule Recommendation Engine evolved through three iterations:
- **V1** failed because labeling individual transactions had no leverage (still required manual review of every transaction)
- **V2** pivoted to rule generation (reusable patterns applied to many transactions), but lacked verification of generated rules
- **V3** added comprehensive verification layer (rule testing, conflict detection, context), enabling production-grade accuracy

Final system achieves >90% accuracy by combining high-quality data signals (verified transactions), rigorous verification (testing rules on source data before surfacing), and transparent presentation (reasoning + impact estimates).

Key technical insight: Rule overlap is a set relationship problem, not a transaction-level problem. Modeling this abstraction led to linear-time conflict detection that scales.

**End Result:** Production system growing rule coverage from <15% to 35-40% (2-3x improvement), reconciling tens of thousands of transactions per month that previously required manual review, with high user trust and confidence.
