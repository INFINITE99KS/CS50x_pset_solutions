// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

typedef uint8_t header_bytes;
typedef int16_t wav_bytes;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    header_bytes header[44];
    fread(header, 44, 1, input);
    fwrite(header, 44, 1, output);

    // create a buffer for a single sample at a time.
    wav_bytes buffer;

    // Read single sample into buffer.
    while( fread(&buffer, sizeof(wav_bytes), 1, input))
    {
    // update volume of sample
    buffer *= factor;

    // Write updated sample to new file.
    fwrite(&buffer, sizeof(wav_bytes), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
