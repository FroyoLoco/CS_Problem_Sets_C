#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

//Create a new type for bytes
typedef int8_t BYTE8;
//Create a file for the input file
FILE *cardImage;
//Store the first 3 bytes to compare on the jpg header
const BYTE8 JPGHEADERSTART[] = {0xff, 0xd8, 0xff};

//Prototype function
bool readCard(FILE *input);

int main(int argc, char *argv[])
{
    //Ensure correct usage
    if (argc != 2)
    {
        printf("Correct Usage: ./recover card.raw");
        return 1;
    }

    //Get the file name for the recovery file
    cardImage = fopen(argv[1], "r");

    //Check that the card image was allocated properly
    if (!cardImage)
    {
        printf("Error opening file.");
        return 2;
    }

    //Read the card to get the jpgs
    if (readCard(cardImage))
    {
        return 0;
    }
    else
    {
        printf("Failed to read file.");
        return 3;
    }

    //Close the card image file
    fclose(cardImage);

    return 0;
}

bool readCard(FILE *input)
{
    //Create a 512 byte buffer for the block
    BYTE8 *readBlockBuffer = malloc(sizeof(BYTE8) * 512);

    //Keep track of how many jpgs weve found
    int jpgCount = 0;

    char jpgFileName[8];
    FILE *jpgFile;

    //Read the first block
    while (fread(readBlockBuffer, sizeof(readBlockBuffer), 1, input))
    {
        //If there was a read error, exit
        if (ferror(input))
        {
            return false;
        }

        //check the first 4 bytes in the block for a jpg header
        if (readBlockBuffer[0] == JPGHEADERSTART[0] && readBlockBuffer[1] == JPGHEADERSTART[1]
            && readBlockBuffer[2] == JPGHEADERSTART[2])
        {
            //Check 4th byte to confirm jpg
            if ((readBlockBuffer[3] & 0xf0) == 0xe0)
            {
                //If there's an open jpg file we need to close it
                if (jpgFile)
                {
                    fclose(jpgFile);
                }

                //Create the file name and pad with zeroes
                snprintf(jpgFileName, 8, "%0*i.jpg", 3, jpgCount);
                //Open a new jpg file
                jpgFile = fopen(jpgFileName, "w");
                //Increase the count of jpg files weve found
                jpgCount++;
                //If the jpg file failed to open, exit
                if (!jpgFile)
                {
                    return false;
                }
            }
        }

        //If theres an open jpg file at this point, write the 512byte block
        if (jpgFile)
        {
            fwrite(readBlockBuffer, sizeof(readBlockBuffer), 1, jpgFile);
            //If we errored while writing to the file then exit
            if (ferror(jpgFile))
            {
                return false;
            }
        }
    }

    //Free the block buffer
    free(readBlockBuffer);

    return true;
}