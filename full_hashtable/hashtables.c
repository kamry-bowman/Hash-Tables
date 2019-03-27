#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair
{
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable
{
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL)
  {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char *u_str = (unsigned char *)str;

  while ((c = *u_str++))
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable *));

  ht->storage = calloc(capacity, sizeof(LinkedPair *));

  ht->capacity = capacity;

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{

  int index = hash(key, ht->capacity);
  if (ht->storage[index])
  {
    LinkedPair *current = ht->storage[index];
    for (;;)
    {
      if (strcmp(current->key, key) == 0)
      {
        current->value = value;
        break;
      }
      else if (current->next == NULL)
      {
        current->next = create_pair(key, value);
        break;
      }
      current = current->next;
    }
  }
  else
  {
    ht->storage[index] = create_pair(key, value);
  }
}
/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  
  int index = hash(key, ht->capacity);
  LinkedPair *current = ht->storage[index];
  int found = 0;
  
  if (current)
  {
    if (strcmp(current->key, key) == 0) {
      // possible crash source
      ht->storage[index] = current->next;
      found = 1;
    // infinite loop until key is found or linkedPairs are exhausted
    } else {
      for(;;)
      {
        if (current->next == NULL)
        {
          break;
        } else if (strcmp(current->next->key, key) == 0) {
          LinkedPair *deleted = current->next;
          current->next = current->next->next;
          destroy_pair(deleted);
          found = 1;
          break;
        }
        current = current->next;
      }
    }
  }

  if (!found) {
    perror("Key not found!");
  }

}


char *hash_table_retrieve(HashTable *ht, char *key)
{

  int index = hash(key, ht->capacity);
  if (ht->storage[index])
  {
    LinkedPair *current = ht->storage[index];
    int found = 0;
    char *value;
    while (!found)
    {
      if (strcmp(current->key, key) == 0)
      {
        value = current->value;
        found = 1;
      }
      else if (current->next == NULL)
      {
        break;
      }
      current = current->next;
    }
    if (!found) {
      return NULL;
    } else {
      return value;
    }
  }
  else
  {
    return NULL;
  }
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++) {
    LinkedPair *current = ht->storage[i];

    while (current) {
      LinkedPair *next = current->next;
      destroy_pair(current);
      current = next;
    }
  }
  free(ht->storage);
  free(ht);
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = create_hash_table(ht->capacity * 2);
  for (int i = 0; i < ht->capacity; i++) {
    LinkedPair *current = ht->storage[i];

    while (current) {
      LinkedPair *next = current->next;
      hash_table_insert(new_ht, current->key, current->value);
      current = next;
    }
  }

  destroy_hash_table(ht);
  return new_ht;
}

#ifndef TESTING
int main(void)
{
  // struct HashTable *ht = create_hash_table(2);

  // hash_table_insert(ht, "line_1", "Tiny hash table\n");
  // hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  // hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  // printf("%s", hash_table_retrieve(ht, "line_1"));
  // printf("%s", hash_table_retrieve(ht, "line_2"));
  // printf("%s", hash_table_retrieve(ht, "line_3"));

  // int old_capacity = ht->capacity;
  // ht = hash_table_resize(ht);
  // int new_capacity = ht->capacity;
  // printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  // hash_table_insert(ht, "key-0", "val-0");
  // hash_table_insert(ht, "key-1", "val-1");
  // hash_table_insert(ht, "key-2", "val-2");
  // hash_table_insert(ht, "key-3", "val-3");
  // hash_table_insert(ht, "key-4", "val-4");
  // hash_table_insert(ht, "key-5", "val-5");
  // hash_table_insert(ht, "key-6", "val-6");
  // hash_table_insert(ht, "key-7", "val-7");
  // hash_table_insert(ht, "key-8", "val-8");
  // hash_table_insert(ht, "key-9", "val-9");

  // hash_table_remove(ht, "key-9");
  // hash_table_remove(ht, "key-8");
  // hash_table_remove(ht, "key-7");
  // hash_table_remove(ht, "key-6");
  // hash_table_remove(ht, "key-5");
  // hash_table_remove(ht, "key-4");
  // hash_table_remove(ht, "key-3");
  // hash_table_remove(ht, "key-2");
  // hash_table_remove(ht, "key-1");
  // hash_table_remove(ht, "key-0");


  // destroy_hash_table(ht);

      struct HashTable *ht = create_hash_table(8);

    hash_table_insert(ht, "key-0", "val-0");
    hash_table_insert(ht, "key-1", "val-1");
    hash_table_insert(ht, "key-2", "val-2");
    hash_table_insert(ht, "key-3", "val-3");
    hash_table_insert(ht, "key-4", "val-4");
    hash_table_insert(ht, "key-5", "val-5");
    hash_table_insert(ht, "key-6", "val-6");
    hash_table_insert(ht, "key-7", "val-7");
    hash_table_insert(ht, "key-8", "val-8");
    hash_table_insert(ht, "key-9", "val-9");

    hash_table_remove(ht, "key-9");
    hash_table_remove(ht, "key-8");
    hash_table_remove(ht, "key-7");
    hash_table_remove(ht, "key-6");
    hash_table_remove(ht, "key-5");
    hash_table_remove(ht, "key-4");
    hash_table_remove(ht, "key-3");
    hash_table_remove(ht, "key-2");
    hash_table_remove(ht, "key-1");
    hash_table_remove(ht, "key-0");

    // hash_table_retrieve(ht, "key-0") == NULL
    // mu_assert(hash_table_retrieve(ht, "key-1") == NULL, "Deleted value is not NULL");
    // mu_assert(hash_table_retrieve(ht, "key-2") == NULL, "Deleted value is not NULL");
    // mu_assert(hash_table_retrieve(ht, "key-3") == NULL, "Deleted value is not NULL");
    // mu_assert(hash_table_retrieve(ht, "key-4") == NULL, "Deleted value is not NULL");
    // mu_assert(hash_table_retrieve(ht, "key-5") == NULL, "Deleted value is not NULL");
    // mu_assert(hash_table_retrieve(ht, "key-6") == NULL, "Deleted value is not NULL");
    // mu_assert(hash_table_retrieve(ht, "key-7") == NULL, "Deleted value is not NULL");
    // mu_assert(hash_table_retrieve(ht, "key-8") == NULL, "Deleted value is not NULL");
    // mu_assert(hash_table_retrieve(ht, "key-9") == NULL, "Deleted value is not NULL");

    
    char * res = hash_table_retrieve(ht, "key-0");
    if (res == NULL) {
      printf("IS NULL");
    } else {
      printf("value is %s\n", res);
    }

  return 0;
}
#endif
