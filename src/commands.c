#include "commands.h"

#include <string.h>

Node *get_task_using_id(Node *head, long task_id) {
    Node *curr = head;
    while (curr != NULL) {
        KanbanTask *task = (KanbanTask *)curr->value;
        if (task->id == task_id) break;
        curr = curr->next;
    }

    return curr;
}

int command_add_new_task(Node **all_tasks, Node **todo, long id, char *desc,
                         int priority) {
    KanbanTask *task = create_task(id, desc, priority);
    *all_tasks = list_add_inorder(*all_tasks, task, 0, task_all_comparator);
    *todo = list_add_inorder(*todo, task, 0, task_todo_comparator);
    return 0;
}

int command_remove_task(Node **all_tasks, Node **curr_list, long id) {
    Node *to_remove1 = get_task_using_id(*curr_list, id);
    if (to_remove1 == NULL) return -1;
    Node *to_remove2 = get_task_using_id(*all_tasks, id);
    KanbanTask *to_remove_task = (KanbanTask *)to_remove1->value;

    delete_task(to_remove_task);

    *curr_list = list_remove(*curr_list, to_remove1);
    *all_tasks = list_remove(*all_tasks, to_remove2);

    return 0;
}

/*
int command_move_to_doing(Node *todo, Node *doing, long task_id, char *worker,
                          time_t deadline) {
    Node *node = get_task_using_id(todo, task_id);
    if (node == NULL) return -1;
    KanbanTask *task = node->value;

    task->worker = malloc(strlen(worker) + 1);
    strcpy(task->worker, worker);
    task->state = DOING;

    task->deadline = deadline;
    list_add_inorder(doing, task, sizeof(Node), task_doing_comparator);
    list_remove(todo, node);
    return 0;
}

int command_move_to_todo(Node *todo, Node *doing, long task_id) {
    Node *node = get_task_using_id(doing, task_id);
    if (node == NULL) return -1;
    KanbanTask *task = node->value;

    free(task->worker);
    task->deadline = -1;
    task->state = TODO;

    list_add_inorder(todo, task, sizeof(Node), task_doing_comparator);
    list_remove(doing, node);
    return 0;
}

int command_change_responsable(Node *doing, long task_id, char *worker) {
    Node *node = get_task_using_id(doing, task_id);
    if (node == NULL) return -1;
    KanbanTask *task = node->value;

    free(task->worker);
    task->worker = malloc(strlen(worker) + 1);
    strcpy(task->worker, worker);
    return 0;
}

int command_end_task(Node *doing, Node *done, long task_id) {
    Node *node = get_task_using_id(doing, task_id);
    if (node == NULL) return -1;
    KanbanTask *task = node->value;

    time_t curr_time;
    time(&curr_time);
    task->finish_date = curr_time;
    task->state = DONE;

    list_add_inorder(done, task, sizeof(Node), task_done_comparator);
    list_remove(doing, node);
    return 0;
}

int command_reopen(Node *todo, Node *done, long task_id) {
    Node *node = get_task_using_id(done, task_id);
    if (node == NULL) return -1;
    KanbanTask *task = node->value;

    free(task->worker);
    task->deadline = -1;
    task->finish_date = -1;
    task->state = TODO;

    list_add_inorder(todo, task, sizeof(Node), task_todo_comparator);
    list_remove(done, node);
    return 0;
}*/