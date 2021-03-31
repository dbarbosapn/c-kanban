#ifndef LINKED_LIST_H

#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>

#define LIST_SERIALIZE_BUFFER_SIZE 4096

typedef struct node {
    void *value;
    struct node *next;
} Node;

Node *create_node(void *value, size_t alloc_size);

Node *list_prepend(Node *head, void *value, size_t alloc_size);

Node *list_append(Node *head, void *value, size_t alloc_size);

Node *list_add_inorder(Node *head, void *value, size_t alloc_size,
                       int (*comparator)(void *, void *));

Node *list_get(Node *head, int index);

Node *list_remove(Node *head, Node *to_remove);

Node *list_sort(Node *head, int (*comparator)(void *, void *));

char *list_serialize(Node *head, char *(*serializer)(void *));

void list_save(Node *head, FILE *fp, void (*value_saver)(void *, FILE *));

Node *list_load(FILE *fp, void *(*value_loader)(FILE *), size_t alloc_size);

int list_size(Node *head);
#endif
