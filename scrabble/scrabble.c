#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);
string word_lower(string word);
string trim(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    int lenght = strlen(word);
    int points = 0;

    fot (int i = 0; i < lenght; i++)
    {
        if ()
    }
}

string word_lower(string word)
{
    int lenght = strlen(word);

    for (int i = 0; i < lenght; i++)
    {
        word[i] = tolower(word[i]);
    }
    return word;
}

string trim(string word)
{
    int counter;
    int lenght = strlen(word);
    for (int i = 0; i < lenght; i++)
    {
        if ((word[i] >= 65 && word[i] <= 90) || (word[i] >= 97 && word[i] <= 122))
        {
            counter++;
        }
    }
    char trimmed[counter];
    int arrCounter = 0;

    for (int i = 0; i < lenght; i++)
    {
        if ((word[i] >= 65 && word[i] <= 90) || (word[i] >= 97 && word[i] <= 122))
        {
            trimmed[arrCounter] = word[i];
            arrCounter++;
        }
    }

    return trimmed;
}