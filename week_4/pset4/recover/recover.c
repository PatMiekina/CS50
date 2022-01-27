#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover input\n");
        return 1;
    }


    // Open file or check if file can be opened
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Read the file
    BYTE buffer[512];

    int file_number = 0;
    // name the file
    char filename[8];
    sprintf(filename, "%03i.jpg", file_number);
    FILE *image = fopen(filename, "w");

    while (fread(&buffer, 512, 1, input) == 1)
    {

        // If start of the new .jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {

            // if not first jpeg
            if (file_number != 0)
            {
                fclose(image);
            }

            // New file
            sprintf(filename, "%03i.jpg", file_number);
            image = fopen(filename, "w");

            // Increment file number
            file_number++;
        }
        // If not start of the jpeg
        if (file_number != 0)
        {
            fwrite(&buffer, 512, 1, image);
        }
    }

    // Close the input & recovery file
    fclose(image);
    fclose(input);
    return 0;
}