#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
//Set the ascii codes for A and Z
const int ASCIICODEA = 65;
const int ASCIICODEZ = 90;

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);


    //Print out thr correct winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    //Create a variable to hold the score
    int returnScore = 0;

    //Cycle through each character in the string word
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        //Get the ascii code of the current character in uppercase
        int asciiCode = toupper(word[i]);
        //Take the ascii code of upper case A from the character ascii code to zero the index for our score array
        int scoreIndex = asciiCode - ASCIICODEA;

        //Check if the ascii code is bigger or the same as A or less than or the same as Z
        if (asciiCode >= ASCIICODEA && asciiCode <= ASCIICODEZ)
        {
            //Add points if it's a valid character
            returnScore += POINTS[scoreIndex];
        }
    }

    //Return the score
    return returnScore;
}
