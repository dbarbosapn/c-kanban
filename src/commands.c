// #include <commands.h>
#include <linked_list.c>
#include <kanban_task.c>

void command_insert_new_task(Node *todo, int id, char *desc, int priority) {
     KanbanTask *task = create_task(id,desc,priority);

     //    needs comparator
     list_add_inorder(todo, task, sizeof(Node), todo_comparator);
}

void command_move_to_doing(Node* todo, Node* doing, lond task_id, char *worker, time_t deadline) {
     Node *node = list_get_with_id(todo, task_id);
     KanbanTask *task = node->value;

     task->worker = malloc(strlen(worker)+1);
     strcpy(task->worker, worker);

     task->deadline = deadline;
     list_add_inorder(doing, task, sizeof(Node), doing_comparator);
     list_remove(todo, node);
}


void command_move_to_todo(Node* todo, Node* doing, long task_id) {
     Node *node = list_get_with_id(doing, task_id);
     KanbanTask *task = node->value;
     free(task->worker);
     task->deadline = NULL;
     list_add_inorder(todo, task, sizeof(Node), doing_comparator);
     list_remove(doing, node);
}

void command_change_responsable(Node *doing, long task_id, char *worker) {
     Node *node = list_get_with_id(doing, task_id);
     KanbanTask *task = node->value;
     free(task->worker);
     task->worker = malloc(strlen(worker)+1);
     strcpy(task->worker, worker);
}

void command_end_task(Node *doing,Node *done, long task_id) {
     Node *node = list_get_with_id(doing, task_id);
     KanbanTask *task = node->value;

     time_t curr_time;
     time(&curr_time);
     task->finish_date = curr_time;

     list_add_inorder(done, task, sizeof(Node), done_comparator);
     list_remove(doing, node);
}

void command_reopen(Node *todo,Node *done, long task_id) {
     Node *node = list_get_with_id(doing, task_id);
     KanbanTask *task = node->value;

     free(task->worker);
     task->deadline = NULL;
     task->finish_date = NULL;

     list_add_inorder(todo, task, sizeof(Node), todo_comparator);
     list_remove(done, node);
}
