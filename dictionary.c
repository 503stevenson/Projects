// Reads each individual word from a dictionary text file, sorts the words by letter and inputs them into a hash table that can be referred to 
// by the "speller.c" spell checking algorithm

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;
bool loaded;

// Hash table
node* table[N];

// Checks if a word is in the dictionary
bool check(const char *word)
{

    //set up dictionary search
    int hashindex = hash(word);
    node* current_nodeptr = table[hashindex];

    //run through linked list at hash value to find word match
    while (current_nodeptr != NULL)
    {
        if (strcasecmp(current_nodeptr->word, word) == 0)
        {
            return true;
        }
        current_nodeptr = current_nodeptr->next;
    }
    return false;
}

// Hash words to a number
unsigned int hash(const char word[])
{
    int letter_index;
    
    if (isupper(word[0]) != 0)
    {
        letter_index = word[0] - 'A';
    }
    
    if (islower(word[0]) != 0)
    {
        letter_index = word[0] - 'a';
    }

    return letter_index;
}

// Load dictionary into memory
bool load(const char *dictionary)
{
    // declaraions
    char new_word[LENGTH + 1];
    int letter_index;

    //set all pointers to NULL
    for(int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    FILE *f = fopen(dictionary, "r");
    if (f == NULL)
    {
        return false;
    }

    //read one word from the dictionary file
    while(fscanf(f, "%s", new_word) != EOF)
    {
        letter_index = hash(new_word);

    //create new node for word
        node* nodeptr = malloc(sizeof(node));
        if (nodeptr == NULL)
        {
            return false;
        }
        nodeptr->next = NULL;

    //put new node into hash table
        strcpy(nodeptr->word, new_word);
        
        if (table[letter_index] == NULL)
        {
            table[letter_index] = nodeptr;
        }
        else
        {
            nodeptr->next = table[letter_index];
            table[letter_index] = nodeptr;
        }
    }

    loaded = true;
    fclose(f);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if(!loaded)
    {
        return 0;
    }
    
    int count = 0;
    node *p;
    
    for (int i = 0; i < N; i++)
    {
        p = table[i];
        if (p != NULL)
        {
            p = p->next;
            count++;
        }
    }
    
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //declarations
    node* current_node;
    node*tmp;

    for (int i = 0; i < N; i++)
    {
        tmp = table[i];
        current_node = table[i];
        
        if (table[i] != NULL)
        {
            current_node = tmp->next;
        }

        while(tmp != NULL)
        {
            free(tmp);
            tmp = current_node;

            //handles 1+ nodes at hash index
            if(current_node != NULL)
            {
                current_node = current_node->next;
            }
            else
            {
                free(tmp);
            }
        }
    }
    return true;
}
