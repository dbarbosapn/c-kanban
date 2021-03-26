#include <linked_list.c>
#include <kanban_task.c>


void command_insert_new_task(Node *todo, int id, char *desc, int priority);

void command_move_to_doing(Node* todo, Node* doing, lond task_id, char *worker, time_t deadline);

void command_move_to_todo(Node* todo, Node* doing, long task_id);

void command_change_responsable(Node *doing, long task_id, char *worker);

void command_end_task(Node *doing,Node *done, long task_id);

void command_reopen(Node *todo,Node *done, long task_id);
