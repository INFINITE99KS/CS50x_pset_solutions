#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long getnumber = get_long("Number: ");
    long copy = getnumber;
    bool isSecond = false;
    int DigitSum = 0;

    while (copy > 0) // Checking if the card is valid.
    {
        int digit = copy % 10;

        if (isSecond)
        {
            digit *= 2;
            DigitSum += (digit / 10 + digit % 10); // If the number is a second digit, then we
                                                   // multiply it by 2 and use its products.
        }
        else
        {
            DigitSum += digit;
        }
        isSecond = !isSecond;
        copy /= 10;
    }

    if (DigitSum % 10 == 0)
    {
        copy = getnumber;
        int start = 0;
        int length = 0;
        int first_two = 0;

        while (copy > 0) // obtain the first digit as well as the first two digits.
        {
            if (copy >= 10 && copy < 100)
            {
                first_two = copy;
            }
            start = copy;
            copy /= 10;
            length++;
        }

        // Here we try to determine the sort of card used.
        if ((length == 15) && ((first_two == 34) || (first_two == 37)))
        printf("AMEX\n");
        else if ((length == 16) && ((first_two >= 51 && first_two <= 55)))
        printf("MASTERCARD\n");
        else if ((length == 13 || length == 16) && (start == 4))
        printf("VISA\n");
        else
        printf("INVALID\n");
    }
    else
    {
        // We fall back to invalid if the card doesn't pass the intial check.
        printf("INVALID\n");
    }
}
