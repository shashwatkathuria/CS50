// Implements a dictionary's functionality

#include <stdbool.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<cs50.h>
#include "dictionary.h"
//defines a user type called node
typedef struct node
{
    bool is_word;
    struct node *children[27];
}
node;
//defining unload helper function
void freeNode(node *currentNode);
//j counts no of word in dictionary
int j = 1;
int k = 1;
node *root;
// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *temp = root;
    //does necessary checks for checking if the word exists in the dictionary or not
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        //does necessary stuff using pointers
        if (word[i] == '\'')
        {
//            bool x=temp->children[0] == NULL
            if (temp->children[0] == NULL)
            {
                return false;
                break;
            }
            else
            {
                temp = temp->children[0];
                if (i == n - 1 && temp->is_word == true)
                {
                    return true;
                }
                if (i == n - 1 && temp->is_word == false)
                {
                    return false;
                }
            }
            continue;
        }
        //does necessary stuff using pointers
        int x;
        int ch;
        ch = toupper(word[i]);
        x = ch - 64;
        if (temp->children[x] == NULL)
        {
            return false;
            break;
        }
        else
        {
            temp = temp->children[x];
            if (i == n - 1 && temp->is_word == true)
            {
                return true;
            }
            if (i == n - 1 && temp->is_word == false)
            {
                return false;
            }
        }

    }
    //frees variables used
    free(temp);
    return false;

}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //defines word
    char word[LENGTH + 1];
    FILE *dict = fopen(dictionary, "r");
    //returns if no file found
    if (dict == NULL)
    {
        return false;
    }
    else
    {
        //allocates space for root
        root = malloc(sizeof(node));
        //initializes all children to null
        memset(root, 0, sizeof(node));
        while (fscanf(dict, "%s", word) != EOF)
        {
            j++;
            node *temp = root;
            for (int i = 0, n = strlen(word); i < n; i++)
            {
                //does necessary calculations
                if (word[i] == '\'')
                {
                    if (temp->children[0] == NULL)
                    {
                        temp->children[0] = malloc(sizeof(node));
                        memset(temp->children[0], 0, sizeof(node));
                        temp = temp->children[0];
                        if (i == n - 1)
                        {
                            temp->is_word = true;
                        }
                    }
                    else
                    {
                        temp = temp->children[0];
                        if (i == n - 1)
                        {
                            temp->is_word = true;
                        }
                    }
                    continue;
                }
                int x;
                int ch;
                ch = toupper(word[i]);
                x = ch - 64;
                //does necessary calculations
                if (temp->children[x] == NULL)
                {
                    temp->children[x] = malloc(sizeof(node));
                    memset(temp->children[x], 0, sizeof(node));
                    temp = temp->children[x];
                    if (i == n - 1)
                    {
                        temp->is_word = true;
                    }
                }
                else
                {
                    temp = temp->children[x];
                    if (i == n - 1)
                    {
                        temp->is_word = true;
                    }
                }

            }
            //frees temp
            temp = NULL;
            free(temp);

        }
        //closes opened file
        fclose(dict);
        return true;
    }
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //returns j-1, which is size of dict
    if (j > 1)
    {
        return j - 1;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //unloads by calling recursive function
    for (int i = 0; i < 27; i++)
    {
        if (root->children[i] != NULL)
        {
            freeNode(root->children[i]);
        }
    }
    //frees root
    free(root);
    return true;
}

void freeNode(node *currentNode)
{
    //dives into the recursive levels
    for (int i = 0; i < 27; i++)
    {
        if (currentNode->children[i] != NULL)
        {
            freeNode(currentNode->children[i]);
        }
    }
    free(currentNode);
}
