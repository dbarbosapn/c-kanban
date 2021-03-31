#include <kanban_task.h>
#include <linked_list.h>

int command_add_new_task(Node **all_tasks, Node **todo, long id, char *desc,
                         int priority);

int command_remove_task(Node **all_tasks, Node **curr_list, long id);

int command_move_to_doing(Node **todo, Node **doing, long task_id, char *worker,
                          time_t deadline);

int command_move_to_todo(Node **todo, Node **doing, long task_id);

int command_change_responsable(Node **doing, long task_id, char *worker);

int command_end_task(Node **doing, Node **done, long task_id);

int command_reopen(Node **todo, Node **done, long task_id);
