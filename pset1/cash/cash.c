#include <cs50.h>
#include <stdio.h>

int coins(int *change,
          int coin_type); // Defines a function coins, to calculate coins of each type.
                          // Passes change by reference, so we don't have to alter the value of
                          // change every time. added a variable "coin_type", to avoid boilerplate.

int main(void)
{
    int change;
    int coins_total = 0;
    do // In here we prompt the user to enter a number that's not -ve, and we keep on prompting them
       // if it's.
    {
        change = get_int("Change Owed: ");
    }
    while (change < 0);

    int quarters = coins(&change, 25); // Calculates quarters.
    int dimes = coins(&change, 10);    // Calcualtes dimes.
    int nickels = coins(&change, 5);   // Calculates nickels.
    int pennies = coins(&change, 1);   // Calculates pennies.

    coins_total =
        quarters + dimes + nickels + pennies; // Sums the number of coins that's gonna be given.
    printf("%i\n", coins_total);
}

int coins(int *change, int coin_type)
{
    int coin = *change / coin_type; // Calculates the number of coins for each coin type.
    *change %=
        coin_type; // Passing by reference ensures that we don't need to change "change" in main.
    return coin;
}
