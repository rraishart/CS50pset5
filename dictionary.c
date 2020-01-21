/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>

#include "dictionary.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>
uint32_t SuperFastHash (const char * data, int len);

// typedef linked list. code from https://www.youtube.com/watch?v=E_1D17P-bM0
typedef struct node
{
    char word[LENGTH + 1];
    bool full;
    struct node* next;
}
node;

int wordcount = 0;
// array of pointers to nodes       
node *hashtable[HASH_MAX + 1];

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // help with changing case from http://stackoverflow.com/questions/15241380/need-assistance-with-converting-const-char-word-to-a-lowercase-version-of-it
    int size = strlen(word), i = 0;
    char lowerword[LENGTH + 1];
    strncpy(lowerword, word, size);
    lowerword[size] = '\0';
    while (lowerword[i] != '\0')
    {
        lowerword[i] = tolower(lowerword[i]);
        i++;
    }
    uint32_t hash = SuperFastHash(lowerword, strlen(lowerword) + 1);
    node* crawler = hashtable[hash];
    if (strncmp(crawler->word, lowerword, LENGTH + 1) == 0)
        {
            return true;
        }
    do
    {
        if (crawler->next != NULL)
        {
            crawler = crawler->next;
        }
        if (strncmp(crawler->word, lowerword, LENGTH + 1) == 0)
        {
            return true;
        }
    } while (crawler->next != NULL);
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // TODO
// initialize hashtable
    for (int i = 0; i < HASH_MAX + 1; i++)
    {
        // store address of node
        hashtable[i] = malloc(sizeof(node));
        if (hashtable[i] == NULL)
        {
            printf("Memory error\n");
            return false;
        }
        // set full flag
        hashtable[i]->full = false;
        hashtable[i]->next = NULL;
    }
    
    // open dictionary
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        return false;
    }
    
     int index = 0;
    char word[LENGTH+1];
    // spell-check each word in text
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // append character to word
            word[index] = c;
            index++;

            // ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // consume remainder of alphabetical string
                while ((c = fgetc(fp)) != EOF && isalpha(c));

                // prepare for new word
                index = 0;
            }
        }

        // ignore words with numbers (like MS Word can)
        else if (isdigit(c))
        {
            // consume remainder of alphanumeric string
            while ((c = fgetc(fp)) != EOF && isalnum(c));

            // prepare for new word
            index = 0;
        }

        // we must have found a whole word
        else if (index > 0)
        {
            // terminate current word
            word[index] = '\0';
            // get hash code
            uint32_t hash = SuperFastHash(word, strlen(word) + 1);
            
            // if exists and bucket is empty
            if (hashtable[hash] != NULL && hashtable[hash]->full != true)
            {
                strncpy(hashtable[hash]->word, word, LENGTH + 1);
                wordcount++;
                hashtable[hash]->full = true;
            }
            // if exists and bucket is full
            else if (hashtable[hash] != NULL && hashtable[hash]->full == true)
            {
                // check for end of linked list
                if (hashtable[hash]->next == NULL)
                {
                    // make temp node
                    node* temp_node;
                    temp_node = (node*) malloc(sizeof(node));
                    temp_node->next = NULL;
                    temp_node->full = true;
                    strncpy(temp_node->word, word, LENGTH + 1);
                    wordcount++;
                    hashtable[hash]->next = temp_node;
                }
                else
                {
                    node* crawler = hashtable[hash]->next;
                    while (crawler->next != NULL)
                    {
                        crawler = crawler->next;
                    }
                    // make temp node
                    node* temp_node;
                    temp_node = (node*) malloc(sizeof(node));
                    temp_node->next = NULL;
                    temp_node->full = true;
                    strncpy(temp_node->word, word, LENGTH + 1);
                    wordcount++;
                    
                    crawler->next = temp_node;
                }
                // traversal ponter
                
            }
            // prepare for next word
            index = 0;
        }
    }
    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return wordcount;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    for (int i = 0; i < HASH_MAX + 1; i++)
    {
        node* cursor = hashtable[i];
        while (cursor != NULL)
        {
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}
