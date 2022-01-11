#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //Cycle through the width and height of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Get each of the rgb values for the current pixel
            int _r = image[i][j].rgbtRed;
            int _g = image[i][j].rgbtGreen;
            int _b = image[i][j].rgbtBlue;
            //Calculate the average by adding them up and dividing by 3
            float average = (((float)_r + (float)_g + (float)_b) / 3.0f);

            //Set the new rgb values to their new average values, if they are not greater than 255 otherwise set to 255
            image[i][j].rgbtRed = average > 255 ? 255 : round(average);
            image[i][j].rgbtGreen = average > 255 ? 255 : round(average);
            image[i][j].rgbtBlue = average > 255 ? 255 : round(average);
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //Set the max width so i dont have to keep using wisth -1
    int maxWidth = width - 1;
    //Create a new image to reflect the old one into
    RGBTRIPLE newImage[height][width];

    //Cycle though the width and height of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Set the begining of the new image to the end of the original
            newImage[i][j] = image[i][maxWidth - j];
            //Set the end of the new image to the beginning of the original
            newImage[i][maxWidth - j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            //Apply the new image pixels to the original
            image[i][j] = newImage[i][j];
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Set the adjustmant constants for ease of design
    const int ADJUSTEDHEIGHT = height -1;;
    const int ADJUSTEDWIDTH = width -1;;
    const int MAXBLURBOXSIZE = 9;

    //Create a new image to hold the image changes
    RGBTRIPLE newImage[height][width];

    //Cycle though the width and height of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Set a variable so that we can keep track of how many pixels are around the current one
            int boxCount = 0;
            //Set a variable to add the rgb values to
            int rCount = 0;
            int gCount = 0;
            int bCount = 0;

            //Cycle through the max possible pixels that could be around a pixel
            for (int k = 0; k < MAXBLURBOXSIZE; k++)
            {
                switch(k)
                {
                    case 0:
                        //Calculate top left
                        if(i - 1 >= 0 && j - 1 >= 0)
                        {
                            boxCount++;
                            rCount += image[i - 1][j - 1].rgbtRed;
                            gCount += image[i - 1][j - 1].rgbtGreen;
                            bCount += image[i - 1][j - 1].rgbtBlue;
                        }
                        break;
                    case 1:
                        //Calculate top middle
                        if(i - 1 >= 0)
                        {
                            boxCount++;
                            rCount += image[i - 1][j].rgbtRed;
                            gCount += image[i - 1][j].rgbtGreen;
                            bCount += image[i - 1][j].rgbtBlue;
                        }
                        break;
                    case 2:
                        //Calculate top right
                        if(i - 1 >= 0 && j + 1 <= ADJUSTEDWIDTH)
                        {
                            boxCount++;
                            rCount += image[i - 1][j + 1].rgbtRed;
                            gCount += image[i - 1][j + 1].rgbtGreen;
                            bCount += image[i - 1][j + 1].rgbtBlue;
                        }
                        break;
                    case 3:
                        //Calculate left
                        if(j - 1 >= 0)
                        {
                            boxCount++;
                            rCount += image[i][j - 1].rgbtRed;
                            gCount += image[i][j - 1].rgbtGreen;
                            bCount += image[i][j - 1].rgbtBlue;
                        }
                        break;
                    case 4:
                        //Calculate the current pixel
                        boxCount++;
                        rCount += image[i][j].rgbtRed;
                        gCount += image[i][j].rgbtGreen;
                        bCount += image[i][j].rgbtBlue;
                        break;
                    case 5:
                        //Calculate right
                        if(j + 1 <= ADJUSTEDWIDTH)
                        {
                            boxCount++;
                            rCount += image[i][j + 1].rgbtRed;
                            gCount += image[i][j + 1].rgbtGreen;
                            bCount += image[i][j + 1].rgbtBlue;
                        }
                        break;
                    case 6:
                        //Calculate the bottom right
                        if(i + 1 <= ADJUSTEDHEIGHT && j - 1 >= 0)
                        {
                            boxCount++;
                            rCount += image[i + 1][j - 1].rgbtRed;
                            gCount += image[i + 1][j - 1].rgbtGreen;
                            bCount += image[i + 1][j - 1].rgbtBlue;
                        }
                        break;
                    case 7:
                        //Calculate the bottom middle
                        if(i + 1 <= ADJUSTEDHEIGHT)
                        {
                            boxCount++;
                            rCount += image[i + 1][j].rgbtRed;
                            gCount += image[i + 1][j].rgbtGreen;
                            bCount += image[i + 1][j].rgbtBlue;
                        }
                        break;
                    case 8:
                        //Calculate the bottom right
                        if(i + 1 <= ADJUSTEDHEIGHT && j + 1 <= ADJUSTEDWIDTH)
                        {
                            boxCount++;
                            rCount += image[i + 1][j + 1].rgbtRed;
                            gCount += image[i + 1][j + 1].rgbtGreen;
                            bCount += image[i + 1][j + 1].rgbtBlue;
                        }
                        break;
                }

                //Since all of the values are already added.. divide by the amount of pixels found to get the average
                //Then set the new image pixel to the rounded new value if its less than or = to 255
                float adjustedColour = (float)rCount / (float)boxCount;
                newImage[i][j].rgbtRed = adjustedColour > 255 ? 255 : round(adjustedColour);

                adjustedColour = (float)bCount / (float)boxCount;
                newImage[i][j].rgbtBlue = adjustedColour > 255 ? 255 : round(adjustedColour);

                adjustedColour = (float)gCount / (float)boxCount;
                newImage[i][j].rgbtGreen = adjustedColour > 255 ? 255 : round(adjustedColour);
            }
        }
    }

    //Apply the new image pixels to the original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = newImage[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Set constant variables for ease of design
    const int ADJUSTEDHEIGHT = height -1;;
    const int ADJUSTEDWIDTH = width -1;;
    const int MAXEDGEBOXSIZE = 9;

    //Create a variable to hold the new image
    RGBTRIPLE newImage[height][width];

    //Cycle though the width and height of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Set variables to hold the calculations for Gx and Gy for each channel
            int gXr = 0;
            int gXg = 0;
            int gXb = 0;

            int gYr = 0;
            int gYg = 0;
            int gYb = 0;

            for (int k = 0; k < MAXEDGEBOXSIZE; k++)
            {
                switch(k)
                {
                    case 0:
                        //Top Left
                        if(i - 1 >= 0 && j - 1 >= 0)
                        {
                            gXr += (image[i - 1][j - 1].rgbtRed * -1);
                            gXg += (image[i - 1][j - 1].rgbtGreen * -1);
                            gXb += (image[i - 1][j - 1].rgbtBlue * -1);

                            gYr += (image[i - 1][j - 1].rgbtRed * -1);
                            gYg += (image[i - 1][j - 1].rgbtGreen * -1);
                            gYb += (image[i - 1][j - 1].rgbtBlue * -1);
                        }
                        break;
                    case 1:
                        //Top middle
                        if(i - 1 >= 0)
                        {
                            gYr += (image[i - 1][j].rgbtRed * -2);
                            gYg += (image[i - 1][j].rgbtGreen * -2);
                            gYb += (image[i - 1][j].rgbtBlue * -2);
                        }
                        break;
                    case 2:
                        //Top right
                        if(i - 1 >= 0 && j + 1 <= ADJUSTEDWIDTH)
                        {
                            gXr += image[i - 1][j + 1].rgbtRed;
                            gXg += image[i - 1][j + 1].rgbtGreen;
                            gXb += image[i - 1][j + 1].rgbtBlue;

                            gYr += (image[i - 1][j + 1].rgbtRed * -1);
                            gYg += (image[i - 1][j + 1].rgbtGreen * -1);
                            gYb += (image[i - 1][j + 1].rgbtBlue * -1);
                        }
                        break;
                    case 3:
                        //Left
                        if(j - 1 >= 0)
                        {
                            gXr += (image[i][j - 1].rgbtRed * -2);
                            gXg += (image[i][j - 1].rgbtGreen * -2);
                            gXb += (image[i][j - 1].rgbtBlue * -2);
                        }
                        break;
                    case 4:
                        //Ignore current pixel as its modifier is always 0
                        break;
                    case 5:
                        //Right
                        if(j + 1 <= ADJUSTEDWIDTH)
                        {
                            gXr += (image[i][j + 1].rgbtRed * 2);
                            gXg += (image[i][j + 1].rgbtGreen * 2);
                            gXb += (image[i][j + 1].rgbtBlue * 2);
                        }
                        break;
                    case 6:
                        //Bottom left
                        if(i + 1 <= ADJUSTEDHEIGHT && j - 1 >= 0)
                        {
                            gXr += (image[i + 1][j - 1].rgbtRed * -1);
                            gXg += (image[i + 1][j - 1].rgbtGreen * -1);
                            gXb += (image[i + 1][j - 1].rgbtBlue * -1);

                            gYr += image[i + 1][j - 1].rgbtRed;
                            gYg += image[i + 1][j - 1].rgbtGreen;
                            gYb += image[i + 1][j - 1].rgbtBlue;
                        }
                        break;
                    case 7:
                        //Bottom middle
                        if(i + 1 <= ADJUSTEDHEIGHT)
                        {
                            gYr += (image[i + 1][j].rgbtRed * 2);
                            gYg += (image[i + 1][j].rgbtGreen * 2);
                            gYb += (image[i + 1][j].rgbtBlue * 2);
                        }
                        break;
                    case 8:
                        //Bottom right
                        if(i + 1 <= ADJUSTEDHEIGHT && j + 1 <= ADJUSTEDWIDTH)
                        {
                            gXr += image[i + 1][j + 1].rgbtRed;
                            gXg += image[i + 1][j + 1].rgbtGreen;
                            gXb += image[i + 1][j + 1].rgbtBlue;

                            gYr += image[i + 1][j + 1].rgbtRed;
                            gYg += image[i + 1][j + 1].rgbtGreen;
                            gYb += image[i + 1][j + 1].rgbtBlue;
                        }
                        break;
                }
            }

            //Square each of the channel Gx and Gy values then add together
            gXr = pow(gXr, 2);
            gYr = pow(gYr, 2);
            int product = gXr + gYr;

            //Square root the product of the calculation
            float gXgYValueR = sqrt((float)product);

            gXg = pow(gXg, 2);
            gYg = pow(gYg, 2);
            product = gXg + gYg;

            float gXgYValueG = sqrt((float)product);

            gXb = pow(gXb, 2);
            gYb = pow(gYb, 2);
            product = gXb + gYb;

            float gXgYValueB = sqrt((float)product);

            //Round the calculated values
            gXgYValueR = round(gXgYValueR);
            gXgYValueG = round(gXgYValueG);
            gXgYValueB = round(gXgYValueB);

            //Apply the values to the new image if they are less than or equal to 255
            newImage[i][j].rgbtRed = gXgYValueR > 255 ? 255 : gXgYValueR;
            newImage[i][j].rgbtGreen = gXgYValueG > 255 ? 255 : gXgYValueG;
            newImage[i][j].rgbtBlue = gXgYValueB > 255 ? 255 : gXgYValueB;
        }
    }

    //Apply the changes to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = newImage[i][j];
        }
    }

    return;
}
