#include <kanban_task.h>
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

void run_test_task() {
    // STEP 1 - init
    printf("-- STEP #1: ");
    char str1[] = "I'm beautiful";
    char str2[] = "My name is Jeff";
    KanbanTask* t1 = create_task(str1, 10);
    task_assign(t1, str2);
    if (strncmp(t1->description, str1, strlen(str1)) != 0 ||
        strncmp(t1->worker, str2, strlen(str2)) != 0 || t1->deadline != NULL ||
        t1->state != TODO || t1->priority != 10) {
        printf("FAILED\n");
    } else {
        printf("PASSED\n");
        printf("-- Today is: %s", asctime(t1->creation_date));
    }

    // STEP 2 - dates
    printf("-- STEP #2: ");
    task_set_deadline(t1, 25, 4, 2019);
    Date* d1 = t1->deadline;
    task_set_finish(t1, 23, 4, 2018);
    Date* d2 = t1->finish_date;
    if (d1->tm_mday != 25 || d1->tm_mon != 4 || d1->tm_year != 2019 ||
        d2->tm_mday != 23 || d2->tm_mon != 4 || d2->tm_year != 2018) {
        printf("FAILED\n");
    }
    printf("PASSED\n");

    // STEP 3 - changes
    printf("-- STEP #3: ");
    task_set_state(t1, 2);
    task_set_priority(t1, 4);
    if (t1->state != DONE | t1->priority != 4) {
        printf("FAILED\n");
    } else {
        printf("PASSED\n");
    }

    // STEP 4 - cleaning memory
    printf("-- STEP #4: ");
    KanbanTask* t2 = create_task("I'm so cool", 1);
    task_assign(t2, "Bob");
    task_set_finish(t2, 1, 1, 1999);
    task_set_deadline(t2, 1, 2, 2000);
    task_free(t2);  // untestable
    task_reopen(t1);
    if (t1->state != TODO || t1->finish_date != NULL || t1->deadline != NULL ||
        t1->worker != NULL) {
        printf("FAILED\n");
    } else {
        printf("PASSED\n");
    }
}

int main(int argc, char const* argv[]) {
    putchar('\n');
    printf("############### TESTING TASK ###############\n");
    run_test_task();
    printf("############################################\n\n");

    printf("########### TESTING LINKED LISTS ###########\n");
    run_test_linked_list();
    printf("############################################\n\n");
    putchar('\n');
    return 0;
}
