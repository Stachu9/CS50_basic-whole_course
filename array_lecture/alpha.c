#include<stdio.h>
#include<cs50.h>
#include<string.h>

int main(void)
{
    string word;
    do
    {
        word = get_string("Word: ");
    }
    while(!word);

    int length = strlen(word);
    printf("%i\n", length);

    if (length == 1)
    {
        printf("YES\n");
    }

    for ( int i = 1; i < length; i++)
    {
        if (word[i] < word[i-1])
        {
            printf("NO\n");
            return 0;
        }
    }
    printf("YES\n");
    return 0;
}