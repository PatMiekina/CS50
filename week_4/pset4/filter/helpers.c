#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate average byte
            int average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // assign average byte to R, G & B bytes
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;

        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue;
    //   sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue;
    //   sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // assign average byte to R, G & B bytes
            int red = round(image[i][j].rgbtRed * 0.393 + image[i][j].rgbtGreen * 0.769 + image[i][j].rgbtBlue * 0.189);
            int green = round(image[i][j].rgbtRed * 0.349 + image[i][j].rgbtGreen * 0.686 + image[i][j].rgbtBlue * 0.168);
            int blue = round(image[i][j].rgbtRed * 0.272 + image[i][j].rgbtGreen * 0.534 + image[i][j].rgbtBlue * 0.131);

            if (red > 255)
            {
                red = 255;
            }

            if (green > 255)
            {
                green = 255;
            }

            if (blue > 255)
            {
                blue = 255;
            }

            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE reversed[height][width];

    // Create a new reversed 2D array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            reversed[i][j] = image[i][width - j - 1];
        }
    }

    // Assign new value
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = reversed[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE buffer[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avrgRed = 0;
            int avrgGreen = 0;
            int avrgBlue = 0;

            float counter = 0.00;

            for (int k = (i - 1); k <= (i + 1); k++)
            {
                for (int l = (j - 1); l <= (j + 1); l++)
                {
                    if (k >= 0 & k <= (height - 1) & l >= 0 & (l <= width - 1))
                    {
                        avrgRed += image[k][l].rgbtRed;
                        avrgGreen += image[k][l].rgbtGreen;
                        avrgBlue += image[k][l].rgbtBlue;

                        counter++;
                    }
                }
            }
            avrgRed = round(avrgRed / counter);
            avrgGreen = round(avrgGreen / counter);
            avrgBlue = round(avrgBlue / counter);

            buffer[i][j].rgbtRed = avrgRed;
            buffer[i][j].rgbtGreen = avrgGreen;
            buffer[i][j].rgbtBlue = avrgBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = buffer[i][j].rgbtRed;
            image[i][j].rgbtGreen = buffer[i][j].rgbtGreen;
            image[i][j].rgbtBlue = buffer[i][j].rgbtBlue;
        }
    }
    return;
}
