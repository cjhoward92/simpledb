//
// Created by carson on 6/11/22.
//

#include <stdlib.h>

#include "trie.h"

// Normalize chars to 0-9.
// Chars are 48-59 in ascii table.
static char clean_key_char(const char c) {
    if (c < 48 || c > 59) {
        return (char)0;
    }
    return (char)(c - 48);
}

trie * new_trie() {
    trie * root = (trie *)malloc(sizeof(trie));
    root->data = NULL;
    root->word = 1;
    root->c = '\0';

    for (u_int8_t i = 0; i < TRIE_CHILDREN_SIZE; i++) {
        root->children[i] = NULL;
    }

    return root;
}

trie_data * new_trie_data(size_t len, void * data) {
    if (len > MAX_TRIE_DATA_SIZE_BYTES) {
        // TODO: Is NULL the best response? Is there a better way to handle this?
        perror("trie data is over maximum byte size");
        return NULL;
    }

    trie_data * td = (trie_data *)malloc(sizeof(trie_data));
    void * clean_data = malloc(len);
    // This might be unnecessary, I should do more research.
    // Reason being - I'm overwriting everything in the next call
    // no matter if it's data from an overflow or legit
    memset(clean_data, 0, len);
    // Copy to a size-constrained buffer so we can transfer data
    // ownership and ensure we have the data we need pre-allocated
    // to the buffer.
    memcpy(clean_data, data, len);

    td->len = len;
    td->data = clean_data;
    return td;
}

void free_trie(trie *root) {
    if (root == NULL) {
        return;
    }

    for (u_int8_t i = 0; i < TRIE_CHILDREN_SIZE; i++) {
        free_trie(root->children[i]);
        root->children[i] = NULL;
    }

    if (root->data != NULL) {
        free_trie_data(root->data);
    }

    free(root);
    root = NULL;
}

void free_trie_data(trie_data *data) {
    if (data == NULL) {
        return;
    }

    free(data->data);
    data->data = NULL;
    free(data);
    data = NULL;
}

void free_trie_range_result(trie_range_result *result) {
    if (result == NULL) {
        return;
    }

    if (result->len > 0) {
        for (size_t i = 0; i < result->len; i++) {
            trie_data *d = result->range[i];
            free_trie_data(d);
        }
    }

    free(result);
    result = NULL;
}

int insert_trie_data(trie *root, const trie_key * key, const trie_data * data) {
    if (root == NULL || key == NULL || data == NULL) {
        return -1;
    }

    trie * tmp = root;
    for (size_t i = 0; i < key->len; i++) {
        const char c = clean_key_char(key->key[i]);
        trie * next_node = tmp->children[c];
        if (next_node == NULL) {
            tmp->children[c] = new_trie();
            next_node = tmp->children[c];
            next_node->word = 0;
            next_node->c = c;
            next_node->data = NULL;
        }

        tmp = next_node;
    }

    tmp->word = 1;
    tmp->data = new_trie_data(data->len, data->data);
    tmp = NULL;

    return 0;
}

int delete_trie_data(trie *root, const trie_key * key) {
    if (root == NULL || key == NULL) {
        return -1;
    }

    trie * tmp = root;
    for (size_t i = 0; i < key->len; i++) {
        const char c = clean_key_char(key->key[i]);
        tmp = tmp->children[c];
        if (tmp == NULL) {
            return -1;
        }
    }

    tmp->word = 0;
    tmp->c = '\0';
    free_trie_data(tmp->data);
    tmp = NULL;

    return 0;
}


int find_trie_data(const trie *root, const trie_key * key, trie_data **out) {
    if (root == NULL || key == NULL || out == NULL) {
        return -1;
    }

    trie * tmp = root;
    for (size_t i = 0; i < key->len; i++) {
        const char c = clean_key_char(key->key[i]);
        tmp = tmp->children[c];
        if (tmp == NULL) {
            return -1;
        }
    }

    *out = tmp->data;
    tmp = NULL;
    return 0;
}

int find_trie_data_range(const trie *root, const trie_key * start, const trie_key * end, const trie_range_result **out) {
    // TODO: this
    return -1;
}