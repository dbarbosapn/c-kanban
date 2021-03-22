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

Node *list_deserialize(char *input, int value_buffer_size,
                       void *(*deserializer)(char *));