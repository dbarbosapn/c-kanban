#include <linked_list.h>
#include <stdint.h>

/**
 * Creates a new node with the given value
 * Returns the new node.
 **/
Node *create_node(void *value, size_t alloc_size) {
    Node *node = (Node *)malloc(sizeof(Node));

    node->value = malloc(alloc_size);
    node->next = NULL;

    memcpy(&node->value, &value, alloc_size);

    return node;
}

/**
 * Adds a node to the beginning of the list.
 * Returns the new head.
 **/
Node *list_prepend(Node *head, void *value, size_t alloc_size) {
    Node *node = create_node(value, alloc_size);
    node->next = head;

    return node;
}

/**
 * Adds a node to the end of the list.
 * Returns the head.
 **/
Node *list_append(Node *head, void *value, size_t alloc_size) {
    Node *node = create_node(value, alloc_size);

    Node *prev = head;
    Node *curr = head->next;
    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = node;

    return head;
}

/**
 * Gets the node at the given index.
 * Returns NULL if invalid index.
 **/
Node *list_get(Node *head, int index) {
    Node *curr = head;

    for (int i = 0; i < index; i++) {
        if (curr == NULL) break;
        curr = curr->next;
    }

    return curr;
}

/**
 * Removes the given node from the list, if exists
 * Returns the new head
 **/
Node *list_remove(Node *head, Node *to_remove) {
    Node *prev = NULL;
    Node *curr = head;

    while (curr != NULL && curr != to_remove) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) return head;

    Node *next = curr->next;

    free(&(curr->value));
    free(curr);

    if (prev == NULL) {
        head = next;
    } else {
        prev->next = next;
    }

    return head;
}

/**
 * Prints the serialized list in the provided file pointer.
 * Requires for a function that will serialize the value by itself.
 **/
void list_serialize(FILE *output, Node *head,
                    void (*value_serializer)(void *, FILE *)) {
    Node *curr = head;
    putc('[', output);
    while (curr != NULL) {
        (*value_serializer)(curr->value, output);
        if (curr->next != NULL) putc(',', output);
        curr = curr->next;
    }
    putc(']', output);
    putc('\n', output);
}