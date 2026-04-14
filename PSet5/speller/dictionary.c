// Implements a dictionary's functionality

#include <stdio.h>     // FILE, fopen, fscanf, fclose, EOF
#include <stdlib.h>    // malloc, free
#include <string.h>    // strcpy, strcmp, strcasecmp
#include <stdbool.h>   // bool, true, false
#include <ctype.h>     // toupper, tolower, isalpha
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// word count.
int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hashing the word to get the index.
    int index = hash(word);

    // Making a temp. ptr to traverse the linked list.
    node *ptr = table[index];

    while (ptr != NULL)
    {
        // Compares the the word in the table with the given word.
        if (strcasecmp(ptr->word, word) == 0)
        {
            return true;
        }
        else
        {
            // Shift ptr to the next pointer to check the rest of the linked list.
            ptr = ptr->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Checks if the first character is a letter.
    if (isalpha(word[0]))
    {
        return (toupper(word[0]) - 'A');
    }
    else
    {
        // if it's not, we return zero.
        return 0;
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file.
    FILE *source = fopen(dictionary, "r");

    // Checking to find file.
    if (source == NULL)
    {
        // Failed to find file.
        return false;
    }

    // Set a buffer of length +1.
    char buffer[LENGTH+1];

    while (fscanf(source, "%s", buffer) != EOF)
    {
        // set a node to copy the word.
        node *load_word = malloc(sizeof(node));

        // Checking for available memory.
        if (load_word == NULL)
        {
            // Failed to allocate memory.
            // TODO: remember to unload();
            return false;
        }

        // Copy the word to the node.
        strcpy(load_word->word, buffer);

        // Get index using hash function.
        int hash_value = hash(load_word->word);

        // if it's the first time for the hash value to appear,
        // then we just link it to the table at said index.
        if(table[hash_value] == NULL)
        {
            load_word->next = NULL;
            table[hash_value] = load_word;
        }
        // if not, then we append the word to the beginning of the linked list at said index.
        else
        {
            load_word->next = table[hash_value];
            table[hash_value] = load_word;
        }
        word_count++;
    }
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
  return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Looping through the indices.
    for (int i = 0; i < N; i++)
    {
        // Intializing a tmp pointer to traverse the linked list.
        node *temp = table[i];
        while(temp != NULL)
        {
            // assigning that tmp pointer to the next element.
            temp = table[i]->next;

            // Freeing the head of the linked list.
            free(table[i]);

            // Moving the head of the node towards being the next one.
            table[i] = temp;
        }
    }
    return true;
}
