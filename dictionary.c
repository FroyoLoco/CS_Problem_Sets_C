// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26 * 26;

// Hash table
node *table[N];

int wordCount;
bool loadComplete = false;

bool InitialiseLinkedList(int hashTableIndex, char *value, int wordLength);
bool InsertToLinkedList(node *start, char *value, int wordHash, int wordLength);
void ClearString(char *s, int l);

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //Store the hashtable index for the word to check
    int checkHash = hash(word);

    //Make sure there is a node to check at this index
    if (table[checkHash] != NULL)
    {
        //Get the word from the node and store it
        char *checkWord = table[checkHash]->word;

        //Check to see if the first node matches the word
        if (strcasecmp(checkWord, word) == 0)
        {
            return true;
        }

        //Check to see if there's another node in the linked list at this index
        if (table[checkHash]->next == NULL)
        {
            return false;
        }

        //Starting at the next node from the original
        //Cycle through each next node and compare the words until the next node = NULL
        for (node *newNode = table[checkHash]->next; newNode != NULL; newNode = newNode->next)
        {
            //Store the word
            checkWord = newNode->word;

            //Check the 3rd charcters in the current word and the word to check isn't the end of the string
            if (checkWord[2] != 0 && word[2] != 0)
            {
                //If the current word in the dictionary's 3rd character is less than the current word's 3rd charcter
                //Then we have gone past the potential matching words
                //As the dictionary is in lexiographic order, it will fill from z-a
                if (checkWord[2] < word[2])
                {
                    return false;
                }
            }

            //Compare the words, ignoring case.
            if (strcasecmp(checkWord, word) == 0)
            {
                return true;
            }
        }

        return false;
    }
    else
    {
        return false;
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //Store the hash to return
    int _hash = 0;
    //Store the first char in the word in lowercase
    char c = tolower(word[0]);

    //Make sure the character is an alphabetical character
    if (c >= 97 && c <= 122)
    {
        //Covert the character to the zeroed out ascii numeric value * 26 as we're making a "2d" hash table out of a "1d" hash table
        _hash += ((c - 97) * 26);
    }

    //Make sure the end of the string isn't the start charcter, so we don't check past it
    if (c != 0)
    {
        //Store the 2nd charcter
        char d = tolower(word[1]);
        //Make sure the charcter is an alphabetical charcter
        if (d >= 97 && d <= 122)
        {
            //Add the zeroed out ascii charcter to the hash
            _hash += (d - 97);
        }
    }

    return _hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //Open the dictionary file
    FILE *dictionaryFile = fopen(dictionary, "r");
    //Make sure the file opened correctly
    if (dictionaryFile == NULL)
    {
        //Close the file if it did nt open corretly
        fclose(dictionaryFile);
        return false;
    }

    //Create a char variable to store the current read char in the file
    char c;
    //Allocate enough memory for the maximum potential size of the string
    char *newWord = malloc(sizeof(char) * (LENGTH + 1));
    //Make sure the memory was allocated correctly
    if (!newWord)
    {
        return false;
    }

    //Clear our new string of garbage values
    ClearString(newWord, LENGTH + 1);

    //Keep track of the length of the string and the hash of the current word
    int wordIndexCount = 0, wordHash = -1;
    //Make sure the word count isnt a garbage value
    wordCount = 0;

    //Keep doing until the end of the file
    while (feof(dictionaryFile) == 0)
    {
        //Get the current char we are reading from the dictionary
        c = fgetc(dictionaryFile);

        //Check the file for potential read errors
        if (ferror(dictionaryFile))
        {
            //Close the file if there was a read error
            fclose(dictionaryFile);
            return false;
        }

        //Make sure the character is not a new line or '
        if (c != 10 && c != 39)
        {
            //Make sure the character isn't anything else other than an alphabetical character
            if (c < 97 || c > 122)
            {
                continue;
            }
        }

        //If the character is a new line then we have hit the end of a word
        if (c == 10)
        {
            //Get the hashed index for the word
            wordHash = hash(newWord);

            //Check the hash table to see if we have already stored a node in this index
            if (table[wordHash] == NULL)
            {
                //If we haven't then initialise a new linked list at this position in the hash table
                if (!InitialiseLinkedList(wordHash, newWord, wordIndexCount))
                {
                    //If we failed to create a linked list, close the file and exit
                    fclose(dictionaryFile);
                    return false;
                }
            }
            else
            {
                //If there is a node at this position then insert a new node into the linked list
                if (!InsertToLinkedList(table[wordHash], newWord, wordHash, wordIndexCount))
                {
                    //If we failed to insert the node then close the file and exit
                    fclose(dictionaryFile);
                    return false;
                }
            }

            //Reset the count of the length of the string
            wordIndexCount = 0;
            //Add to the word count
            wordCount++;

            //free the allocation for the word
            free(newWord);
            //Allocate memory for the next word
            newWord = malloc(sizeof(char) * (LENGTH + 1));

            //If we failed to allocate memory then close the file and exit
            if (!newWord)
            {
                fclose(dictionaryFile);
                return false;
            }

            //Clear the new words string of garbage values
            ClearString(newWord, LENGTH + 1);

            continue;
        }

        //Set the current string's character to the read character
        newWord[wordIndexCount] = c;
        //Add to the length of the string
        wordIndexCount++;
    }

    //Free the memory for the current string
    free(newWord);
    //Close the file
    fclose(dictionaryFile);
    //Change the load to completed
    loadComplete = true;
    return true;
}

bool InitialiseLinkedList(int hashTableIndex, char *value, int wordLength)
{
    //Allocate memory for the current node to create
    node *newNode = malloc(sizeof(node));
    //Report failed if we didn't manage to allocate memory
    if (!newNode)
    {
        return false;
    }

    //Initialise a new node and tell the pointer to point to this node
    node n = {.next = NULL};
    *newNode = n;

    //Store the word in the new node
    for (int i = 0; i < wordLength; i++)
    {
        newNode->word[i] = value[i];
    }

    //Set the node at this hash table index to the new node
    table[hashTableIndex] = newNode;

    return true;
}

bool InsertToLinkedList(node *start, char *value, int wordHash, int wordLength)
{
    //Allocate memory for a new node
    node *newNode = malloc(sizeof(node));
    //If we failed to allocate any memory then report failed
    if (!newNode)
    {
        return false;
    }

    //Initialise a new node and tell the pointer to point to that node
    //Tell the next pointer in the new node to point to the start node
    node n = {.next = start};
    *newNode = n;

    //Store the word in the new node
    for (int i = 0; i < wordLength; i++)
    {
        newNode->word[i] = value[i];
    }

    //Set the root node to this node
    table[wordHash] = newNode;

    return true;
}

void ClearString(char *s, int l)
{
    for (int i = 0; i < l; i++)
    {
        //Set each value in this string to '\0'
        s[i] = 0;
    }
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    //Return the word count if the load is complete, otherwise return 0
    return loadComplete ? wordCount : 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //For each index in the hash table
    for (int i = 0; i < N; i++)
    {
        //Make sure the node at this index exists
        if (table[i] != NULL)
        {
            //Check if the next value at this node not NULL
            if (table[i]->next != NULL)
            {
                //If next exists tthen follow the chain of nodes and free each one via the root node
                for (node *n = table[i]->next; n != NULL; n = table[i]->next)
                {
                    //Set the root nodes next value to the node we are going to free's next value
                    table[i]->next = n->next;
                    //Free the node
                    free(n);
                }

                //Once all next nodes are free, free the root node
                free(table[i]);
            }
            else
            {
                //If there are no next nodes then free the root node
                free(table[i]);
            }
        }
    }

    return true;
}
