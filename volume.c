// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

typedef uint8_t BYTE8;
typedef int16_t BYTE16;

int copyHeaderToFile(FILE *input, FILE *output);
int increaseSamplesByFactor(FILE *input, FILE *output, float factor);

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

    //Try to copy the header
    if (copyHeaderToFile(input, output) == 0)
    {
        //Try to copy the rest of the samples and alter by factor
        if (increaseSamplesByFactor(input, output, factor) != 0)
        {
            //If copying the samples fails exit and print error
            printf("Sample Factoring failed!");
            return 1;
        }
    }
    else
    {
        //If copying the header fails exit and print error
        printf("Copy header failed!");
        return 1;
    }

    // Close files
    fclose(input);
    fclose(output);
}

int copyHeaderToFile(FILE *input, FILE *output)
{
    //Create a buffer the size of the header size
    BYTE8 *buffer = malloc(sizeof(BYTE8) * HEADER_SIZE);

    //Make sure the buffer has been assined an adress
    if (!buffer)
    {
        return 1;
    }

    //Read the header into the buffer
    fread(buffer, sizeof(BYTE8), HEADER_SIZE, input);

    //Make sure there were no errors
    if (ferror(input))
    {
        return 1;
    }


    //Write the buffer to the output
    fwrite(buffer, sizeof(BYTE8), HEADER_SIZE, output);

    //Make sure there were no errors
    if (ferror(output))
    {
        return 1;
    }

    //Free the allocated memeory to the buffer
    free(buffer);
    return 0;
}

int increaseSamplesByFactor(FILE *input, FILE *output, float factor)
{
    //Create a buffer the size of one sample
    BYTE16 *sampleBuffer = malloc(sizeof(BYTE16));

    //Make sure the buffer has been allocated memory
    if (!sampleBuffer)
    {
        return 1;
    }

    //While there is something to read, place it in the buffer
    while (fread(sampleBuffer, sizeof(BYTE16), 1, input))
    {
        //If there was an error return and exit
        if (ferror(input))
        {
            return 1;
        }

        //Multiply the sample by the factor
        *sampleBuffer *= factor;

        //Write the sample in the buffer to the output file
        fwrite(sampleBuffer, sizeof(BYTE16), 1, output);

        //If there was an error exit
        if (ferror(output))
        {
            return 1;
        }
    }

    //Free up the sample buffer memory
    free(sampleBuffer);
    return 0;
}
