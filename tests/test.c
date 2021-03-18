#include <linked_list.h>
#include <stdio.h>
#include <string.h>

char* int_serializer(void* value) {
    int val = (int)value;
    char* buffer = malloc(48 * sizeof(char));
    sprintf(buffer, "%d", val);
    return buffer;
}

int int_comparator(void* value1, void* value2) {
    int val1 = (int)value1;
    int val2 = (int)value2;

    return val1 - val2;
}

void* int_deserializer(char* input) { return atoi(input); }

void run_test_linked_list() {
    int failed = 0;

    // STEP 1
    printf("-- STEP #1: ");

    Node* n = create_node(1, sizeof(int));
    n = list_prepend(n, 2, sizeof(int));

    int n0 = list_get(n, 0)->value;
    int n1 = list_get(n, 1)->value;
    if (n0 != 2 || n1 != 1) {
        printf("FAILED\n");
        failed = 1;
    } else
        printf("PASSED\n");

    // STEP 2
    printf("-- STEP #2: ");

    n = list_append(n, 3, sizeof(int));

    int n2 = list_get(n, 2)->value;
    if (n2 != 3) {
        printf("FAILED\n");
        failed = 1;
    } else
        printf("PASSED\n");

    // STEP 3
    printf("-- STEP #3: ");

    n = list_prepend(n, 1, sizeof(int));
    n = list_remove(n, list_get(n, 0));
    n = list_remove(n, list_get(n, 1));

    int n3 = list_get(n, 1)->value;
    if (n3 != 3) {
        printf("FAILED\n");
        failed = 1;
    } else
        printf("PASSED\n");

    // STEP 4
    printf("-- STEP #4: ");

    n = list_add_inorder(n, -1, sizeof(int), int_comparator);
    n = list_add_inorder(n, 0, sizeof(int), int_comparator);

    int n4 = list_get(n, 0)->value;
    int n5 = list_get(n, 1)->value;
    if (n4 != -1 || n5 != 0) {
        printf("FAILED\n");
        failed = 1;
    } else
        printf("PASSED\n");

    printf("Serialized list: %s\n", list_serialize(n, int_serializer));

    // STEP 5
    printf("-- STEP #5: ");

    Node* dnode = list_deserialize("[1,5,6]", int_deserializer);
    int n6 = list_get(dnode, 0)->value;
    int n7 = list_get(dnode, 1)->value;
    int n8 = list_get(dnode, 2)->value;
    if (n6 != 1 || n7 != 5 || n8 != 6) {
        printf("FAILED\n");
        failed = 1;
    } else
        printf("PASSED\n");
}

int main(int argc, char const* argv[]) {
    printf("########### TESTING LINKED LISTS ###########\n");
    run_test_linked_list();
    printf("############################################\n");
    putchar('\n');
    return 0;
}
