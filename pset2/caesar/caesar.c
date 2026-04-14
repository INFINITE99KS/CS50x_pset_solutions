#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_digit(string s); // Declares is digit to check if the passed argument is indeed an int.
char rotate(char input, int key); // Declares rotate in order to rotate the letters.

int main(int argcount, string argvector[])
{
    // checks for the given conditions.
    if (argcount != 2 || !is_digit(argvector[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // converts the string onto an int so it can be used later on.
    int key = atoi(argvector[1]);
    // Prompts the user for text.
    string user = get_string("plaintext: ");
    // Loops over the string so we can encrypt it all.
    for (int i = 0; user[i] != '\0'; i++)
    {
        user[i] = rotate(user[i], key);
    }
    printf("ciphertext: %s\n", user);
}

bool is_digit(string s)
{
    // Checks if the entered argument is actually a number.
    if (s[0] == '\0')
        return false;
    // Loops through the string to check if there's any non-digit.
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (s[i] < '0' || s[i] > '9')
            return false;
    }
    return true;
}

char rotate(char input, int key)
{
    // Does the rotation for us.
    if (input >= 'a' && input <= 'z')
        return (((input + key - 'a') % 26) + 'a');
    else if (input >= 'A' && input <= 'Z')
        return (((input + key - 'A') % 26) + 'A');
    else
        return input;
}
