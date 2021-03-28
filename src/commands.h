#include <linked_list.c>
#include <kanban_task.c>


int command_insert_new_task(Node *todo, int id, char *desc, int priority);

int command_move_to_doing(Node* todo, Node* doing, lond task_id, char *worker, time_t deadline);

int command_move_to_todo(Node* todo, Node* doing, long task_id);

int command_change_responsable(Node *doing, long task_id, char *worker);

int command_end_task(Node *doing,Node *done, long task_id);

int command_reopen(Node *todo,Node *done, long task_id);
