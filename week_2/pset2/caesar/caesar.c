#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // printf("%i", argc);
    // Check if 1 argument
    // printf("Checking argument size...\n");
    if (argc != 2)
    {
        printf("Wrong number of arguments!\n");
        return 1;
    }


    // Check if digits
    // printf("Checking argument type...\n");
    for (int j = 0; j < strlen(argv[1]); j++)
    {
        if (!isdigit(argv[1][j]))
        {
            printf("Not a number!\n");
            return 1;
        }
    }


    // Convert argument to an integer (key)
    int key = atoi(argv[1]);

    // Prompt user for text to be encrypted
    string plaintext = get_string("plaintext: ");
    printf("ciphertext: ");

    for (int i = 0; i < strlen(plaintext); i++)
    {
        int c = 0;
        // Check if character is a letter
        if (isalpha(plaintext[i]))
        {

            // Check if lowercase or uppercase
            if (islower(plaintext[i]))
            {
                int p = (int)plaintext[i] - 97;
                c = (p + key) % 26 + 97;
                printf("%c", c);
            }
            else
            {
                int p = (int)plaintext[i] - 65;
                c = (p + key) % 26 + 65;
                printf("%c", c);
            }
            // Convert int c to char ch
            // char ch = (char)c;
            // printf("%c", ch);

            // strcat(ciphertext, &ch);
        }
        else
        {
            // Add non-letter characters
            printf("%c", plaintext[i]);
            // strcat(ciphertext, &nonalpha);
        }
    }
    printf("\n");
}
