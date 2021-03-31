#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "commands.h"
#include "rendering.h"

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

/**
 * Gets the current max id
 **/
int get_max_id(Node* all_tasks) {
    Node* curr = all_tasks;
    int max = 0;

    while (curr != NULL) {
        KanbanTask* task = (KanbanTask*)curr->value;

        max = task->id > max ? task->id : max;

        curr = curr->next;
    }

    return max;
}

/**
 * Filters the tasks with the given state. Returns a newly allocated list.
 * Assumes that the values of the linked_list are of the type KanbanTask
 **/
Node* filter_tasks(Node* all_tasks, kanban_state filter) {
    Node* new_list = NULL;
    Node* curr = all_tasks;

    while (curr != NULL) {
        KanbanTask* task = (KanbanTask*)curr->value;
        if (task->state == filter) {
            switch (filter) {
                case TODO:
                    new_list = list_add_inorder(new_list, task, 0,
                                                task_todo_comparator);
                    break;
                case DOING:
                    new_list = list_add_inorder(new_list, task, 0,
                                                task_doing_comparator);
                    break;
                case DONE:
                    new_list = list_add_inorder(new_list, task, 0,
                                                task_done_comparator);
                    break;
            }
        }

        curr = curr->next;
    }

    return new_list;
}

/**
 * Renders a task list. Assumes the given linked list contains KanbanTask
 * values.
 **/
void render_list(kanban_state state, Node* list) {
    render_list_header(state);
    render_list_padding();
    render_list_content(list);
    render_list_footer();
}

/**
 * Renders the command list
 **/
void render_commands(kanban_state state) {
    switch (state) {
        case TODO:
            printf("a: Add new task\n");
            printf("m: Move task to doing\n");
            break;

        case DOING:
            printf("m: Move task to todo\n");
            printf("c: Change responsible\n");
            printf("e: End task\n");
            break;

        case DONE:
            printf("o: Reopen task\n");
            break;
    }
    printf("l: Change current list\n");
    printf("r: Remove task\n");
    printf("q: Quit (and save)\n");

    // TODO: Implement all commands
}

/**
 * Prints the result of a command
 **/
void print_result(int result) {
    if (result >= 0) {
        printf("Success!\n");
    } else {
        printf("Error\n");
    }
}

/**
 * Reads a string from the input, ended by a line break. Returns a pointer to
 * the string.
 * NOTE: Since 'realloc' uses a lazy strategy where it tries to grow the
 * space instead of copying everything and allocating again, and since we're
 * running it in a loop with no other allocations, it will be efficient.
 **/
char* read_string_input() {
    char* str = malloc(sizeof(char));
    int counter = 0;

    char c;
    while ((c = getchar()) != '\n') {
        if (counter > 0) str = realloc(str, (counter + 2) * sizeof(char));
        str[counter] = c;
        counter++;
    }
    str[counter] = '\0';

    return str;
}

int main(int argc, char const* argv[]) {
    system("clear");

    int curr_id = 0;

    Node* all_tasks;
    Node* todo_list;
    Node* doing_list;
    Node* done_list;

    /* Init Phase */
    printf("Loading data...\n");
    all_tasks = load_data();
    todo_list = filter_tasks(all_tasks, TODO);
    doing_list = filter_tasks(all_tasks, DOING);
    done_list = filter_tasks(all_tasks, DONE);
    curr_id = get_max_id(all_tasks) + 1;
    printf("Loaded!\n");

    char chosen_command = 0;
    kanban_state chosen_state = TODO;
    Node** curr_list;

    while (1) {
        putchar('\n');

        switch (chosen_state) {
            case TODO:
                curr_list = &todo_list;
                break;
            case DOING:
                curr_list = &doing_list;
                break;
            case DONE:
                curr_list = &done_list;
                break;
        }

        render_list(chosen_state, *curr_list);

        putchar('\n');

        render_commands(chosen_state);
        printf("\nInsert command: ");
        int items_read = scanf(" %c", &chosen_command);
        getchar();
        if (items_read != 1) {
            system("clear");
            printf("Invalid input. Please try again.\n");
        } else {
            int result;
            long id;
            switch (chosen_command) {
                case 'm':
                    printf(
                        "Insert the task ID (must be in the selected list): ");
                    scanf("%ld", &id);
                    getchar();
                    switch (chosen_state) {
                        case TODO:
                            printf("Insert the worker's name: ");
                            char* worker = read_string_input();
                            int d, m, y;
                            printf("Insert the deadline (dd/mm/yyyy): ");
                            scanf("%d/%d/%d", &d, &m, &y);
                            time_t curr_time;
                            time(&curr_time);
                            Date* dead = localtime(&curr_time);
                            dead->tm_year = y - 1900;
                            dead->tm_mon = m - 1;
                            dead->tm_mday = d;
                            result =
                                command_move_to_doing(&todo_list, &doing_list,
                                                      id, worker, mktime(dead));
                            break;

                        case DOING:
                            result = command_move_to_todo(&todo_list,
                                                          &doing_list, id);
                            break;
                        case DONE:
                            result = -1;
                    }
                    system("clear");
                    print_result(result);
                    break;
                case 'l':
                    printf("Insert the list (0:TODO/1:DOING/2:DONE): ");
                    int l = -1;
                    scanf("%d", &l);
                    system("clear");
                    if (l >= 0 && l <= 2) {
                        chosen_state = l;
                        print_result(0);
                    } else {
                        print_result(-1);
                    }
                    break;

                case 'a':
                    printf("Insert the task description: ");
                    char* desc = read_string_input();
                    printf("Insert the task priority: ");
                    int priority;
                    scanf("%d", &priority);
                    system("clear");
                    result = command_add_new_task(&all_tasks, &todo_list,
                                                  curr_id, desc, priority);
                    print_result(result);
                    curr_id++;
                    break;

                case 'r':
                    printf(
                        "Insert the task ID (must be in the selected list): ");
                    scanf("%ld", &id);
                    system("clear");
                    result = command_remove_task(&all_tasks, curr_list, id);
                    print_result(result);
                    break;

                case 'q':
                    system("clear");
                    printf("Saving data...\n");
                    save_data(all_tasks);
                    printf("Leaving...\n");
                    return 0;

                default:
                    system("clear");
                    printf("Invalid command (%c). Please try again.\n",
                           chosen_command);
                    break;
            }
        }
    }
    return 0;
}
