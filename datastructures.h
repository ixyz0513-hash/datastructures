#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


#define ASSERT(c,m) \
    if(!(c)) { \
        fprintf(stderr,__FILE__":%d: assertion %s failed %s\n",__LINE__,#c,m); \
        exit(1); \
    }

unsigned short alloc_counter;


//STRING_MAL
typedef struct 
{
    unsigned short length;
    char *data;
} string_mal;


string_mal *init_string_mal(char *data) 
{

    if(data == NULL) 
    {
        printf("data is null\n");
        return NULL;
    }

    const unsigned short length = strlen(data);

    string_mal *string = malloc(sizeof(string_mal));

    if(string != NULL) 
    {
        string->length = length;
        string->data = data;
        ++alloc_counter;
        return string;
    }
    return NULL;
}

string_mal string_mal_stack(char *data) 
{
    ASSERT(data != NULL,"data is equal to NULL");

    return (string_mal)
    {
        strlen(data), //length
        data //string
    };
}

void free_string_mal(string_mal *p) 
{
    if(p != NULL) 
    {
        free(p);
        --alloc_counter;
    }

    else printf("tried to free a null pointer\n");
}


void chop_left(string_mal *string,const unsigned char amount) 
{
    ASSERT(string->length > 0,"string length is less then zero or equal to zero");

    string->data += amount;
    string->length -= amount;
}

void chop_right(string_mal *string,const unsigned char amount) 
{
    ASSERT(string->length > 0,"string length is less then zero or equal to zero");
    string->length -= amount;
}

void choose_string(string_mal *string,char *data) 
{
    ASSERT(data != NULL && string != NULL,"data or string_mal pointer are equal to NULL");

    unsigned short length = strlen(data);
    string->data = data;
    string->length = length;
}






//HASHHH
#define MAX_NAME 250
#define MAX_TABLE 100

typedef struct 
{
    char name[MAX_NAME];
    int num;
} hash_table;

uint32_t hash(char *name,int length)
{
    ASSERT(name != NULL,"name is equal to NULL (hash)");

    uint32_t result = 0;
    for(int i = 0; i < length; ++i) 
    {
        result += name[i];
        result = (result * name[i]) % MAX_TABLE;
    }
    return result;
}

void init_hash(hash_table *hash_map[MAX_TABLE]) 
{
    ASSERT(hash_map != NULL,"hash_map is equal to NULL (init_hash)");

    for(int i = 0; i < MAX_TABLE; ++i) hash_map[i] = NULL;
}

bool hash_insert(hash_table *map,hash_table *hash_map[MAX_TABLE]) 
{
    ASSERT(map != NULL,"name is equal to NULL (hash_insert)");
    ASSERT(hash_map != NULL,"hash_map is equal to NULL (hash_insert)");
    

    uint32_t index = hash(map->name,strlen(map->name));
    if(hash_map[index] != NULL) return false;
    
    hash_map[index] = map;
    return true;
}

bool hash_delete(char *name,hash_table *hash_map[MAX_TABLE]) 
{
    ASSERT(name != NULL,"name is equal to NULL (hash_delete)");
    ASSERT(hash_map != NULL,"hash_map is equal to NULL (hash_delete)");

    uint32_t length = strlen(name) - 1;

    uint32_t index = hash(name,length);
    if(hash_map[index] == NULL) return false;
    
    else if(strncmp(name,hash_map[index]->name,length)) return false;

    hash_map[index] = NULL;
    return true;
}

hash_table *look_up(char *name,hash_table *hash_map[MAX_TABLE],int length) 
{
    ASSERT(name != NULL,"name is equal to NULL (look_up)");
    ASSERT(hash_map != NULL,"hash_map is equal to NULL (look_up)");

    uint32_t index = hash(name,length - 1);
    if(hash_map[index] != NULL && !strncmp(name,hash_map[index]->name,length - 1)) return hash_map[index];
    
    else return NULL;
}

void hash_map_clear(hash_table *hash_map[MAX_TABLE]) 
{
    ASSERT(hash_map != NULL,"hash_map is equal to NULL (hash_map_clear)");

    for(int i = 0; i < MAX_TABLE; ++i) 
    {
        if(hash_map[i] != NULL) free(hash_map[i]);
    }
}

void print_hash_map(hash_table *hash_map[MAX_TABLE]) 
{
    ASSERT(hash_map != NULL,"hash_map is equal to NULL (print_hash_map)");

    printf("Started\n");
    for(int i = 0; i < MAX_TABLE; ++i) 
    {
        if(hash_map[i] != NULL) printf("%d: Name: %s Value: %d\n",i,hash_map[i]->name,hash_map[i]->num);

        else printf("%d:\n",i);
    }
    printf("Ended\n");
}


//LINKEDLIST

typedef struct linklist
{
    struct linklist *previous;
    struct linklist *next;
    int data;
} linklist;


linklist* create_node(linklist *previous_address, linklist *next_address, int value) 
{
    linklist *node = malloc(sizeof(linklist));
    node->previous = previous_address;
    node->next = next_address;
    node->data = value;
    ++alloc_counter;
    return node;
}

void print_nodes(linklist *head) 
{
    if(head == NULL) return;

    linklist *temp = head;
    while(temp != NULL) 
    {
        printf("%d\n",temp->data);
        temp = temp->next;
    }
}

void insertion_node(linklist *node,linklist *destination)
{
    ASSERT(node != NULL,"node is equal to NULL (insertion_node)");
    ASSERT(destination != NULL,"destination is equal to NULL (insertion_node)");

    node->next = destination;
    node->previous = destination->previous;
    destination->previous = node;

    if(node->previous != NULL) node->previous->next = node;
}

void remove_node(linklist *destination) 
{
    ASSERT(destination != NULL,"destination is equal to NULL (remove_node)");

    if(destination->next != NULL) 
    {
        destination->next->previous = destination->previous;
        destination->data = -1;
    }  
    
    if(destination->previous != NULL) destination->previous->next = destination->next;

    destination->next = NULL;
    free(destination);
}

bool checkif_list_sorted(linklist *head) 
{
    ASSERT(head != NULL,"head is equal to NULL (checkif_list_sorted)");

    linklist *temp = head;
    while(temp != NULL)
    {
        if(temp->next != NULL) 
        {
            if(temp->data > temp->next->data) return false;
        }
        

        temp = temp->next;
    }
    return true;
}

void link_swap(linklist *num1, linklist *num2) 
{
    ASSERT(num1 != NULL,"num1 is equal to NULL (link_swap)");
    ASSERT(num2 != NULL,"num2 is equal to NULL (link_swap)");

    int temp = num2->data;
    num2->data = num1->data;
    num1->data = temp;
}

void link_sort_bubble(linklist *head) //n^3 bubble sort normally has a worst case of n^2
{
    ASSERT(head != NULL,"head is equal to NULL (link_sort_bubble)");

    if(checkif_list_sorted(head)) return;

    linklist *temp = head;
    bool swapped = false;
    do 
    {
        swapped = false;

        if(temp->next != NULL) 
        {
            if(temp->data > temp->next->data) 
            {
                link_swap(temp,temp->next);
                swapped = true;
            }
        }

        temp = temp->next;

        if(temp == NULL) temp = head;
    } while(swapped != false);
            
}

void free_linklist(linklist *p) 
{
    ASSERT(p != NULL,"pointer equals too NULL");

    free(p);
    --alloc_counter;
}

void check_if_free() 
{
    ASSERT(alloc_counter < 1,"User didnt dealloc everything");
}


//datastructure!
char *optarg = NULL;

char pr_getopt(int argc,char **argv,char *optstring) 
{
    static char *p;
    static unsigned int length = 1;
    static unsigned int counter = 0;
    if(!p && argv[1][0] == '-') 
    {
        p = argv[1] + 1;
    } 
    
    if(*p != '\0' && *p == optstring[counter]) 
    {
        char arg = *p;
        ++counter;
        ++p;
        return arg;
    }

    else if(*p == '\0' && length < argc) 
    {
        ++length;
        p = argv[length];
        ++p;
        return 0;
    }

    else if(optstring[counter] == ':') 
    {
        if(length < argc && argv[length][0] != '-') optarg = argv[length];

        else optarg = "?";

        ++counter;
        return 0;
    }
    

    return -1;
}


#endif