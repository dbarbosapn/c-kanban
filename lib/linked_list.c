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
        memcpy(&node->value, &value, alloc_size);
    } else {
        node->value = value;
    }

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

    if (head == NULL) return node;

    Node *last = head;
    while (last->next != NULL) last = last->next;

    last->next = node;

    return head;
}

/**
 * Adds a node in order to the list.
 * Returns the head.
 **/
Node *list_add_inorder(Node *head, void *value, size_t alloc_size,
                       int (*comparator)(void *, void *)) {
    Node *node = create_node(value, alloc_size);

    if (head == NULL) {
        return node;
    }

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
 * Saves the list in the given file. fp must have write binary permissions.
 **/
void list_save(Node *head, FILE *fp, void (*value_saver)(void *, FILE *)) {
    Node *curr = head;
    while (curr != NULL) {
        (*value_saver)(curr->value, fp);
        curr = curr->next;
    }
}

/**
 * Loads the list from the given file. fp must have read binary permissions.
 * Assumes that the value_loader will return NULL when there are no more values
 * to read.
 **/
Node *list_load(FILE *fp, void *(*value_loader)(FILE *), size_t alloc_size) {
    Node *curr;
    Node *head = NULL;

    while (1) {
        void *value = (*value_loader)(fp);
        if (value == NULL) break;
        if (head != NULL) {
            Node *n = create_node(value, alloc_size);
            curr->next = n;
            curr = n;
        } else {
            head = create_node(value, alloc_size);
            curr = head;
        }
    }

    return head;
}

/**
 * Returns list size
 */
int list_size(Node *head) {
    int size = 0;
    Node *curr = head;
    while (curr != NULL) {
        size++;
        curr = curr->next;
    }
    return size;
}
