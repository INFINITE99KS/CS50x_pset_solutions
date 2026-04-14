#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet.
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word); // Declares a function to computer scores.

int main(void)
{
    string word1 = get_string("Player 1: "); // Prompts player 1 to enter a word.
    string word2 = get_string("Player 2: "); // Prompts player 2 to enter a word.

    int score1 = compute_score(word1); // Computes score of word 1.
    int score2 = compute_score(word2); // Computes score of word 2.

    // Checks whoever won between the players.
    if (score1 > score2)
    printf("Player 1 wins!\n");
    else if (score1 < score2)
    printf("Player 2 wins!\n");
    else
    printf("Tie!\n");
}

int compute_score(string word)
{
    int points = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        word[i] = toupper(word[i]); // Converts every letter to an uppercase,
                                    // Streamlines the points counting func.
        if (isupper(word[i]))
        points += POINTS[word[i] - 'A'];
    }
    return points;
}
