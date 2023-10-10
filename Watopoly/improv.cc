string propName;
cout << "Enter property that you want to improve\n";
cin >> propName;
Ownable *ownable = game[index];
Improvable *improvable = game[index];
if (ownable->owner != currPlayer)
{
    cout << "You cannot improve, since you are not the owner\n";
    continue;
}
int x;
cout << "Enter 1: improve buy\nEnter 2: improve sell\n";
cin >> x;
string deptName = improvable->dept;
int ownedCount = currPlayer->ownedProps[deptName];
if (x == 1)
{
    if (((deptName == "Math" || deptName == "Arts1") && ownedCount == 2) || ownedCount == 3)
    {
        improvBuy(currPlayer, improvable);
    }
    else
    {
        cout << "You cannot buy improvements because it is not part of a monopoly\n";
        continue;
    }
}
else if (x == 2)
{
    if (improvable->improvs == 0)
    {
        cout << "This building does not have any improvements to sell\n";
        continue;
    }
    else
    {
        improvSell(currPlayer, improvable);
    }
}
else
{
    cout << "Incorrect option entered\n" continue;
}
continue;