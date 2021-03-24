#include <kanban_task.h>
#include <linked_list.h>
#include <stdio.h>
#include <unistd.h>

#define TASKS_FILE "data/all_tasks.bin"

/**
 * Checks if a file exists
 **/
int file_exists(char* path) { return access(path, F_OK) == 0; }

/**
 * Loads the tasks from the file defined on TASKS_FILE.
 * If the file doesn't exist, an empty list is returned.
 **/
Node* load_data() {
    if (file_exists(TASKS_FILE)) {
        FILE* fp = fopen(TASKS_FILE, "rb");
        Node* n = list_load(fp, task_load, 0);
        fclose(fp);
        return n;
    }

    return NULL;
}

/**
 * Saves the given list in the data file
 **/
void save_data(Node* all_tasks) {
    FILE* fp = fopen(TASKS_FILE, "wb");
    list_save(all_tasks, fp, task_save);
    fclose(fp);
}

int main(int argc, char const* argv[]) {
    /* Init Phase */
    printf("Loading data...\n");
    Node* all_tasks = load_data();
    printf("Loaded!\n");

    // TODO: Graphics routine. Always show data on top, commands on bottom.

    printf("This is the main file.\n");
    return 0;
}
