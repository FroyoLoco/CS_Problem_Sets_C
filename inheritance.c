// Simulate genetic inheritance of blood type

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
}
person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();

int main(void)
{
    // Seed random number generator
    srand(time(0));

    // Create a new family with three generations
    person *p = create_family(GENERATIONS);

    // Print family tree of blood types
    print_family(p, 0);

    // Free memory
    free_family(p);
}

// Create a new individual with `generations`
person *create_family(int generations)
{
    //Allocate memory for new person
    person * newPerson = malloc(sizeof(person));
    //Check memory is not NULL
    if(newPerson == NULL)
    {
        return NULL;
    }

    // Generation with parent data
    if (generations > 1)
    {
        //Recusively assign parents
        newPerson->parents[0] = create_family(generations-1);
        newPerson->parents[1] = create_family(generations-1);
        //Generate a random number between 0 - 1
        int r = rand() % 2;
        //Assign the random allele from each parent
        newPerson->alleles[0] = newPerson->parents[0]->alleles[r];
        newPerson->alleles[1] = newPerson->parents[1]->alleles[r];


    }
    else
    {
        //Randomly get alleles and set parents to NULL for highest generation
        newPerson->alleles[0] = random_allele();
        newPerson->alleles[1] = random_allele();
        newPerson->parents[0] = NULL;
        newPerson->parents[1] = NULL;
    }

    return newPerson;
}

// Free `p` and all ancestors of `p`.
void free_family(person *p)
{
    //Return if p == NULL because there's more more people left to free
    if(p == NULL)
    {
        return;
    }
    else
    {
        //Recursively free parents of p
        free_family(p->parents[0]);
        free_family(p->parents[1]);
        //Free p
        free(p);
    }
}

// Print each family member and their alleles.
void print_family(person *p, int generation)
{
    // Handle base case
    if (p == NULL)
    {
        return;
    }

    // Print indentation
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    // Print person
    printf("Generation %i, blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// Randomly chooses a blood type allele.
char random_allele()
{
    int r = rand() % 3;
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 'O';
    }
}
