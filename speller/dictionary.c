// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 19683;

// Hash table
node *table[N];

int wordCounter = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    node *cursor = table[hash(word)];
    while (true)
    {
        if (cursor == NULL)
        {
            return false;
        }
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}
int count7 = 0;
int count24 = 0;
void results(void)
{
    printf("count 7: %i \n count 24: %i\n", count7, count24);
}
// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    if (strlen(word) == 1)
    {
        return toupper(word[0]) - 'A';
    }
    else if (strlen(word) == 2)
    {
        return abs((toupper(word[0]) - 'A') + 26 * toupper(word[1]) - 'A');
    }
    else
    {
        return abs(toupper(word[0]) - 'A' + 26 * (toupper(word[1]) - 'A') + 26 * 26 * (toupper(word[2]) - 'A'));
    }

}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    char tempword[LENGTH + 1];

    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    while (fscanf(file, "%s", tempword) != EOF)
    {
        node *n = malloc(sizeof(node));
        strcpy(n->word, tempword);
        n->next = table[hash(tempword)];
        table[hash(tempword)] = n;
        wordCounter++;
    }

    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return wordCounter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *temp;

        while (cursor != NULL)
        {
            temp = cursor->next;
            free(cursor);
            cursor = temp;
        }
    }
    return true;
}
