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
    if (priority < 1 || priority > 10) return -1;

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

int command_move_to_doing(Node **todo, Node **doing, long task_id, char *worker,
                          time_t deadline) {
    Node *to_move = get_task_using_id(*todo, task_id);
    if (to_move == NULL || list_size(*doing) == TASK_MAX_DOING) return -1;
    KanbanTask *task = (KanbanTask *)to_move->value;

    task->worker = malloc(strlen(worker) + 1);
    strcpy(task->worker, worker);
    task->state = DOING;
    task->deadline = deadline;

    *doing = list_add_inorder(*doing, task, 0, task_doing_comparator);
    *todo = list_remove(*todo, to_move);

    return 0;
}

int command_move_to_todo(Node **todo, Node **doing, long task_id) {
    Node *to_move = get_task_using_id(*doing, task_id);
    if (to_move == NULL) return -1;
    KanbanTask *task = to_move->value;

    free(task->worker);
    task->deadline = -1;
    task->state = TODO;

    *todo = list_add_inorder(*todo, task, 0, task_todo_comparator);
    *doing = list_remove(*doing, to_move);

    return 0;
}

int command_change_responsable(Node **doing, long task_id, char *worker) {
    Node *node = get_task_using_id(*doing, task_id);
    if (node == NULL) return -1;
    KanbanTask *to_change = node->value;

    free(to_change->worker);
    to_change->worker = malloc(strlen(worker) + 1);
    strcpy(to_change->worker, worker);

    return 0;
}

int command_end_task(Node **doing, Node **done, long task_id) {
    Node *to_move = get_task_using_id(*doing, task_id);
    if (to_move == NULL) return -1;
    KanbanTask *task = to_move->value;

    time_t curr_time;
    time(&curr_time);
    task->finish_date = curr_time;
    task->state = DONE;

    *done = list_add_inorder(*done, task, 0, task_done_comparator);
    *doing = list_remove(*doing, to_move);
    return 0;
}

int command_reopen(Node **todo, Node **done, long task_id) {
    Node *to_move = get_task_using_id(*done, task_id);
    if (to_move == NULL) return -1;
    KanbanTask *task = to_move->value;

    free(task->worker);
    task->deadline = -1;
    task->finish_date = -1;
    task->state = TODO;

    *todo = list_add_inorder(*todo, task, 0, task_todo_comparator);
    *done = list_remove(*done, to_move);
    return 0;
}
