// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Number of words
int words_number = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Hash word to obtain hash value
    int index = hash(word);
    // Access linked list at that index
    node *cursor = table[index];
    // Use strcasecmp to check if word exists
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor-> next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    // Output between 0 and N-1
    int index = 0;
    // If "a", then hash equals to 0, because index = n - 97
    if (word[0] >= 97 && word[0] <= 122)
    {
        index = word[0] - 97;
    }
    // If "A", then hash equals to 0, because index = n - 65
    else if (word[0] >= 65 && word[0] <= 90)
    {
        index = word[0] - 65;
    }
    return index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    // Open the file
    FILE *input = fopen(dictionary, "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Initialize temporary word
    char word_new[LENGTH + 1];

    // Scan and save words from the file one by one
    while (fscanf(input, "%s", word_new) != EOF)
    {
        // Allocate memory to a new node
        node *n = malloc(sizeof(node));
        // Check if no problems with memory allocation!!!! TBD
        if (n == NULL)
        {
            printf("Problem with memory allocation.\n");
            return 1;
        }

        // Copy the word to the new node
        strcpy(n->word, word_new);
        n->next = NULL;

        // Define which letter the word starts with (specific index in the array)
        int i = hash(word_new);
        // printf("%i", i);

        // if the first word beginning with this letter - make it equal to the initial node
        if (table[i] == NULL)
        {
            table[i] = n;
        }
        // if not, point to the first node, then make the current node the first node
        else
        {
            n->next = table[i];
            table[i] = n;
        }
        // printf("word: %s\n next word: %s", n->word, n->next->word);
        words_number++;
    }
    fclose(input);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return words_number;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < LENGTH; i++)
    {
        node *cursor = table[i];

        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }

        if (i == N - 1 && cursor == NULL)
        {
            return true;
        }
    }
    return false;
}
