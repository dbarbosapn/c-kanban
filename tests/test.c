#include <linked_list.h>
#include <stdio.h>
#include <string.h>

void int_serializer(void* value, FILE* output) {
    int val = (int)value;
    fprintf(output, "%d", val);
}

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

    printf("Serialized final list: ");
    list_serialize(stdout, n, int_serializer);
}

int main(int argc, char const* argv[]) {
    printf("########### TESTING LINKED LISTS ###########\n");
    run_test_linked_list();
    printf("############################################\n");
    putchar('\n');
    return 0;
}
