#include <cs50.h>
#include <stdio.h>

//Calculate llama years prototype function
void calculateLlamaYears();

int main(void)
{
    //Call the calculate llama years function
    calculateLlamaYears();
}

void calculateLlamaYears()
{
    //Declare statsize and end size
    int startSize, endSize;

    //Ask user for a start size
    do
    {
        startSize = get_int("Start Size: ");
    }
    while (startSize < 9);

    //Ask user for an end size
    do
    {
        endSize = get_int("End Size: ");
    }
    while (endSize < startSize);

    //Declare the year count
    int yearCount = 0;

    //Loop until we are more or equal to the end size
    while (startSize < endSize)
    {
        //Calculate the new llamas and the dead llamas
        int newLlamas = startSize / 3;
        int deadLlamas = startSize / 4;

        //Adjust the start size for that years calculation
        startSize = (startSize + newLlamas) - deadLlamas;

        //increas the year count by 1
        yearCount++;

    }

    //Print the year count
    printf("Years: %i\n", yearCount);
}
