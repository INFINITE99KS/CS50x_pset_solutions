#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompt the user to enter some text.
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentence in the text.
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Calcualtes the L and S for the formula.
    float L = (letters / (float) words) * 100;
    float S = (sentences / (float) words) * 100;

    // Calculating the index.
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int printed_index = round(index);

    // Printing the index.
    if (printed_index < 1)
        printf("Before Grade 1\n");
    else if (printed_index >= 1 && printed_index <= 16)
        printf("Grade %i\n", printed_index);
    else
        printf("Grade 16+\n");
}

int count_letters(string text)
{
    // Counts letters inside of text.
    int letters = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
            letters++;
    }
    return letters;
}

int count_words(string text)
{
    // Return the number of words in text.
    int words_count = 1;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == ' ')
        {
            words_count++;
        }
    }
    return words_count;
}

int count_sentences(string text)
{
    // Return the number of sentences in text.
    int sentence_count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentence_count++;
        }
    }
    return sentence_count;
}
