#include "helpers.h"
#include <math.h>

int isint_lower(int a, int b)
{
    // a functions that returns the lower of two numbers.
    if (a < b)
        return a;
    return b;
}

// Grayscale implementation
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    for (int i = 0; i < height; i++)
    {

        for (int j = 0; j < width; j++)
        {
            // Take average of red, green, and blue.
            int avg =
                round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0);

            // Updating pixel values.
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
        }
    }
}

// Sepia implementation
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Computing sepia.
            int sepiaRed = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen +
                                 0.189 * image[i][j].rgbtBlue);
            int sepiaGreen = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen +
                                   0.168 * image[i][j].rgbtBlue);
            int sepiaBlue = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen +
                                  0.131 * image[i][j].rgbtBlue);

            // Update pixel with sepia values
            image[i][j].rgbtRed = isint_lower(sepiaRed, 255);
            image[i][j].rgbtGreen = isint_lower(sepiaGreen, 255);
            image[i][j].rgbtBlue = isint_lower(sepiaBlue, 255);
        }
    }
}

// Reflect implementation
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
}

void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int totalRed = 0, totalGreen = 0, totalBlue = 0;
            int count = 0;

            // Looping through neighbouring pixels.
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    // vars to check if we went out of bounds.
                    int ni = i + di;
                    int nj = j + dj;

                    // checking if we went out of bounds.
                    if (ni >= 0 && nj < width && ni < height && nj >= 0)
                    {
                        totalRed += copy[ni][nj].rgbtRed;
                        totalGreen += copy[ni][nj].rgbtGreen;
                        totalBlue += copy[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }
            // Compute average
            image[i][j].rgbtRed = round((float) totalRed / count);
            image[i][j].rgbtBlue = round((float) totalBlue / count);
            image[i][j].rgbtGreen = round((float) totalGreen / count);
        }
    }
}
