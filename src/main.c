#include <kanban_task.h>
#include <linked_list.h>
#include <stdio.h>
#include <unistd.h>

#define TASKS_FILE "data/all_tasks.data"

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
        FILE* fp = fopen(TASKS_FILE, "r");

        char buffer[LIST_SERIALIZE_BUFFER_SIZE];
        if (!fp || feof(fp) || !fgets(buffer, sizeof(buffer), fp)) {
            printf("There was an error loading the data file. (%s)\n",
                   TASKS_FILE);
            fclose(fp);
            return NULL;
        }

        fclose(fp);
        return list_deserialize(buffer, TASK_SERIALIZE_BUFFER_SIZE,
                                task_deserialize);
    }

    return NULL;
}

/**
 * Saves the given list in the data file
 **/
void save_data(Node* all_tasks) {
    char* str = task_deserialize(all_tasks);
    FILE* fp = fopen(TASKS_FILE, "w");
    fputs(str, fp);
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
