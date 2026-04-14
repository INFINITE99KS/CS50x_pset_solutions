#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // checks for number of command line arguments.
    if (argc != 2)
    {
        printf("Please run the program as following: ./recover <one argument>\n");
        return 1;
    }

    // Open the memory card.
    FILE *card = fopen(argv[1], "r");

    if (card == NULL)
    {
        printf("Please enter a valid card.\n");
        return 2;
    }
    // Creating the image that's gonna be written to.
    FILE *img = NULL;

    // Create a buffer for a block of data.
    uint8_t buffer[512];

    // While there's data left to read from the memory card.
    int count = 0; // Count of JPEGs found.
    while (fread(buffer, 1, 512, card) == 512)
    {
        // first three bytes: 0xff 0xd8 0xff
        // Last Byte: 0xe0, 0x0e1, 0x0e2, etc...
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // Closes the previous JPEG, if it existed.
            if (img != NULL)
            {
                fclose(img);
            }
            // Makes a brand new JPEG.
            char filename[8];
            sprintf(filename, "%03i.jpg", count);
            img = fopen(filename, "w");
            count++;
        }

        // Continues to write to it.
        if (img != NULL)
        {
            fwrite(buffer, 1, 512, img);
        }
    }

    // Closes the very last JPEG.
    if (img != NULL)
    {
        fclose(img);
    }

    fclose(card);
    return 0;
}
