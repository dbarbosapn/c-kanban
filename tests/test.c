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

void run_test_linked_list() {
    // STEP 1
    printf("-- STEP #1: ");

    Node* n = create_node(1, sizeof(int));
    n = list_prepend(n, 2, sizeof(int));

    int n0 = list_get(n, 0)->value;
    int n1 = list_get(n, 1)->value;
    if (n0 != 2 || n1 != 1)
        printf("FAILED\n");
    else
        printf("PASSED\n");

    // STEP 2
    printf("-- STEP #2: ");

    n = list_append(n, 3, sizeof(int));

    int n2 = list_get(n, 2)->value;
    if (n2 != 3)
        printf("FAILED\n");
    else
        printf("PASSED\n");

    // STEP 3
    printf("-- STEP #3: ");

    n = list_prepend(n, 1, sizeof(int));
    n = list_remove(n, list_get(n, 0));
    n = list_remove(n, list_get(n, 1));

    int n3 = list_get(n, 1)->value;
    if (n3 != 3)
        printf("FAILED\n");
    else
        printf("PASSED\n");

    // STEP 4
    printf("-- STEP #4: ");

    n = list_add_inorder(n, -1, sizeof(int), int_comparator);
    n = list_add_inorder(n, 0, sizeof(int), int_comparator);

    int n4 = list_get(n, 0)->value;
    int n5 = list_get(n, 1)->value;
    if (n4 != -1 || n5 != 0)
        printf("FAILED\n");
    else
        printf("PASSED\n");

    list_remove(n, list_get(n, 1));
    printf("Serialized list: %s\n", list_serialize(n, int_serializer));

    // STEP 5
    KanbanTask* task1 = create_task(1, "Task1", 1);
    KanbanTask* task2 = create_task(2, "Task2", 2);

    Node* task_list = create_node(task1, 0);
    task_list = list_append(task_list, task2, 0);

    printf("-- STEP #5: ");
    FILE* fp = fopen("test.bin", "wb");
    list_save(task_list, fp, task_save);
    fclose(fp);
    fp = fopen("test.bin", "rb");
    Node* task_list1 = list_load(fp, task_load, 0);
    fclose(fp);

    KanbanTask* task3 = list_get(task_list1, 0)->value;
    KanbanTask* task4 = list_get(task_list1, 1)->value;
    if (task3->id != 1 || task4->id != 2)
        printf("FAILED\n");
    else
        printf("PASSED\n");

    printf("Serialized list of tasks: %s\n",
           list_serialize(task_list1, task_serialize));

    free(n);
    free(task_list);
    delete_task(task1);
    delete_task(task2);
    free(task_list1);
    delete_task(task3);
    delete_task(task4);


    // STEP 6
    printf("-- STEP #6: ");
    int p = 2;
    Node *empty_list;
    Node *unempty_list = create_node(&p,sizeof(int));
    unempty_list->next = create_node(&p,sizeof(int));
    if (list_size(unempty_list) != 2)
        printf("FAILED\n");
    else
        printf("PASSED\n");

    free(unempty_list->next);
    free(unempty_list);
}

void run_test_task() {
    // STEP 1 - init
    printf("-- STEP #1: ");
    char str1[] = "I'm beautiful";
    char str2[] = "My name is Jeff";
    KanbanTask* t1 = create_task(1, str1, 10);
    task_assign(t1, str2);
    if (strncmp(t1->description, str1, strlen(str1)) != 0 ||
        strncmp(t1->worker, str2, strlen(str2)) != 0 || t1->deadline != -1 ||
        t1->state != TODO || t1->priority != 10) {
        printf("FAILED\n");
    } else {
        printf("PASSED\n");
        printf("-- Today is: %s", asctime(localtime(&t1->creation_date)));
    }

    // STEP 2 - dates
    printf("-- STEP #2: ");
    task_set_deadline(t1, 25, 4, 2019);
    task_set_finish(t1, 23, 4, 2018);
    Date d1 = *localtime(&t1->deadline);
    Date d2 = *localtime(&t1->finish_date);

    if (d1.tm_mday != 25 || d1.tm_mon != 4 - 1 || d1.tm_year != 2019 - 1900 ||
        d2.tm_mday != 23 || d2.tm_mon != 4 - 1 || d2.tm_year != 2018 - 1900) {
        printf("FAILED\n");
    } else
        printf("PASSED\n");

    // STEP 3 - changes
    printf("-- STEP #3: ");
    task_set_state(t1, 2);
    task_set_priority(t1, 4);
    if (t1->state != DONE || t1->priority != 4) {
        printf("FAILED\n");
    } else {
        printf("PASSED\n");
    }

    // STEP 4 - save and load
    printf("-- STEP #4: ");
    KanbanTask* t2 = create_task(1, "Test", 5);
    FILE* fp = fopen("test.bin", "wb");
    task_save(t2, fp);
    fclose(fp);
    fp = fopen("test.bin", "rb");
    KanbanTask* t3 = task_load(fp);
    fclose(fp);
    if (t3->state != TODO || t3->priority != 5 || t3->id != 1 ||
        strcmp("Test", t3->description) != 0) {
        printf("FAILED\n");
    } else {
        printf("PASSED\n");
    }

    printf("Serialized task (t1): %s\n", task_serialize(t1));

    free(t1);
    free(t2);
    free(t3);
}

int main(int argc, char const* argv[]) {
    putchar('\n');
    printf("############### TESTING KANBAN TASK ########\n");
    run_test_task();
    printf("############################################\n\n");

    printf("########### TESTING LINKED LISTS ###########\n");
    run_test_linked_list();
    printf("############################################\n\n");
    putchar('\n');
    return 0;
}
