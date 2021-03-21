#include <linked_list.h>
#include <stdio.h>
#include <unistd.h>

#define TASKS_FILE "data/all_tasks.data"

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
            return NULL;
        }

        // TODO: Use the buffer to deserialize the list. Pass the task
        // deserializer too.
    }

    return NULL;
}

int main(int argc, char const* argv[]) {
    /* Init Phase */
    printf("Loading data...\n");
    Node* all_tasks = load_data();
    printf("Loaded!\n");

    printf("This is the main file.\n");
    return 0;
}
