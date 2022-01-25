#include <cs50.h>
#include <stdio.h>
#include <math.h>

int get_change(void);

int main(void)
{
    int coins = 0;
    int change = get_change();
    // Check how many quarters
    while (change >= 25)
    {
        coins += 1;
        change -= 25;
    }
    // Check how many dimes
    while (change >= 10)
    {
        coins += 1;
        change -= 10;
    }
    // Check how many nickels
    while (change >= 5)
    {
        coins += 1;
        change -= 5;
    }
    // Check how many cents
    while (change >= 1)
    {
        coins += 1;
        change -= 1;
    }
    printf("%i\n", coins);
}

// Prompt user to write how much change he's owed
int get_change(void)
{
    float change;
    do
    {
        change = get_float("Change owed: ");
    }
    while (change < 0);
    int cents = round(change * 100);
    return cents;
}