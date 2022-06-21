//
// Created by carson on 6/11/22.
//

#ifndef TLS_SERVER_TRIE_H
#define TLS_SERVER_TRIE_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * 0-9 at this time
 * We have to handle (10 * sizeof(ptr)) bytes
 * for all trie nodes. We can probably make
 * this more space efficient if we create
 * a map data structure instead.
 */
#define TRIE_CHILDREN_SIZE 10

/**
 * Allow half a KB in key len for now.
 */
#define MAX_TRIE_KEY_SIZE_BYTES 512

/**
 * 4KB maybe for efficient storage on disk if we get there?
 */
#define MAX_TRIE_DATA_SIZE_BYTES 4 * 1024

/**
 * A wrapper for keys.
 * This is just for convenience.
 */
typedef struct trie_key {
    size_t len;
    const char * key;
} trie_key;

/**
 * A wrapper for storing data in a trie node.
 * This is just for convenience.
 */
typedef struct trie_data {
    void * data;
    size_t len;
} trie_data;

/**
 * A wrapper for storing trie range results.
 * This is just for convenience.
 */
typedef struct trie_range_result {
    size_t len;
    trie_data **range;
} trie_range_result;

/**
 * This is a very basic trie that acts as a simple index
 * on characters 0-9, a-z. We allow 36 characters for now and
 * at 8 bytes per pointer we are looking at roughly 288 bytes
 * allocated just for children for each node.
 * A trie node is minimum 1 + 288 + 1 + 8 + 8 = 306 bytes.
 *
 * This seems fairly space inefficient but we can optimize
 * how we store children later to reduce the minimum node size.
 * Ideally, we find a way to pack it to < 20 bytes.
 */
typedef struct trie {
    trie_data * data;
    struct trie * children[TRIE_CHILDREN_SIZE];
    char c;
    unsigned char word;
} trie;

/**
 * Generates a new root trie structure.
 * @return A pointer to a trie node.
 */
trie * new_trie();
/**
 * Generates a new data wrapper.
 * @param len - The length of the data being stored.
 * @param data - The data being stored.
 * @return A wrapper to the data.
 */
trie_data * new_trie_data(size_t len, void * data);

int insert_trie_data(trie *root, const trie_key * key, const trie_data * data);
int delete_trie_data(trie *root, const trie_key * key);
int find_trie_data(const trie *root, const trie_key * key, trie_data **out);
int find_trie_data_range(const trie *root, const trie_key * start, const trie_key * end, const trie_range_result **out);

void free_trie(trie *root);
void free_trie_data(trie_data *data);
void free_trie_range_result(trie_range_result *result);

#endif //TLS_SERVER_TRIE_H
