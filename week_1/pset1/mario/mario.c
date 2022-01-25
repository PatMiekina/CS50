#include <cs50.h>
#include <stdio.h>

int get_positive_int(void);

int main(void)
{
    int rows = get_positive_int();
    // Build the pyramid
    for (int r = 1; r <= rows; r++)
    {
        // Build white spaces to make the pyramid right aligned
        for (int i = rows; i > r; i--)
        {
            printf(" ");
        }
        // Build right-aligned hashes
        int j = 0;
        while (j < r)
        {
            printf("#");
            j++;
        }
        // Add gap between steps
        printf("  ");
        // Reset j so that the loop runs again
        j = 0;
        // Build left-aligned hashes
        while (j < r)
        {
            printf("#");
            j++;
        }
        printf("\n");
    }
}

// Get an integer between 1 and 8 from the user
int get_positive_int(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);
    return height;
}