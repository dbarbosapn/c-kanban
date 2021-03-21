#include <linked_list.h>
#include <stdint.h>
#include <string.h>

/**
 * Creates a new node with the given value.
 * If alloc_size is 0, the node will be created by reference, not by value.
 * Returns the new node.
 **/
Node *create_node(void *value, size_t alloc_size) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->value = NULL;
    node->next = NULL;

    if (alloc_size > 0) {
        node->value = malloc(alloc_size);
    } else {
        node->value = value;
    }

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
 * Adds a node in order to the list.
 * Returns the head.
 **/
Node *list_add_inorder(Node *head, void *value, size_t alloc_size,
                       int (*comparator)(void *, void *)) {
    Node *node = create_node(value, alloc_size);

    // Edge case where the new node should be the new head
    if ((*comparator)(head->value, node->value) > 0) {
        node->next = head;
        return node;
    }

    Node *prev = head;
    Node *curr = head->next;

    while (curr != NULL && (*comparator)(curr->value, node->value) <= 0) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = node;
    node->next = curr;

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
 * Serializes the given list.
 * Requires for a function that will serialize the value by itself.
 * The values must not contain the character '|', since it is used as a
 * separator
 **/
char *list_serialize(Node *head, char *(*value_serializer)(void *)) {
    char *buffer = malloc(LIST_SERIALIZE_BUFFER_SIZE * sizeof(char));

    int i = 0;

    Node *curr = head;

    buffer[i] = '[';
    i++;

    while (curr != NULL) {
        char *value = (*value_serializer)(curr->value);

        for (int j = 0; j < strlen(value); j++, i++) {
            buffer[i] = value[j];
        }
        free(value);

        if (curr->next != NULL) {
            buffer[i] = '|';
            i++;
        }
        curr = curr->next;
    }

    buffer[i] = ']';
    buffer[i + 1] = '\0';

    return buffer;
}

/**
 * Deserializes the given string to a list.
 * Requires a function that will serialize the value by itself.
 * The function does not verify if input is valid.
 **/
Node *list_deserialize(char *input, int value_buffer_size,
                       void *(*deserializer)(char *)) {
    Node *head = NULL;

    int curr_index = 1;

    while (input[curr_index] != ']') {
        char *value_buffer = malloc(value_buffer_size * sizeof(char));
        int i = 0;

        while (input[curr_index] != ']' && input[curr_index] != '|') {
            value_buffer[i] = input[curr_index];
            curr_index++;
            i++;
        }

        if (input[curr_index] == '|') curr_index++;

        value_buffer[i] = '\0';

        void *value = (*deserializer)(value_buffer);
        free(value_buffer);

        if (head != NULL) {
            head = list_append(head, value, 0);
        } else {
            head = list_prepend(head, value, 0);
        }
    }

    return head;
}