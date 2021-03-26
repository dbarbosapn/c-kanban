#include <commands.h>

int command_insert_new_task(Node *todo, int id, char *desc, int priority) {
     KanbanTask *task = create_task(id,desc,priority);

     //    needs comparator
     list_add_inorder(todo, task, sizeof(Node), todo_comparator);
     return 0;
}

int command_move_to_doing(Node* todo, Node* doing, lond task_id, char *worker, time_t deadline) {
     Node *node = get_task_using_id(todo, task_id);
     if(node == NULL) return -1;
     KanbanTask *task = node->value;

     task->worker = malloc(strlen(worker)+1);
     strcpy(task->worker, worker);
     task->state = DOING;

     task->deadline = deadline;
     list_add_inorder(doing, task, sizeof(Node), doing_comparator);
     list_remove(todo, node);
     return 0;
}


int command_move_to_todo(Node* todo, Node* doing, long task_id) {
     Node *node = get_task_using_id(doing, task_id);
     if(node == NULL) return -1;
     KanbanTask *task = node->value;

     free(task->worker);
     task->deadline = NULL;
     task->state = TODO;

     list_add_inorder(todo, task, sizeof(Node), doing_comparator);
     list_remove(doing, node);
     return 0;
}

int command_change_responsable(Node *doing, long task_id, char *worker) {
     Node *node = get_task_using_id(doing, task_id);
     if(node == NULL) return -1;
     KanbanTask *task = node->value;

     free(task->worker);
     task->worker = malloc(strlen(worker)+1);
     strcpy(task->worker, worker);
     return 0;
}

int command_end_task(Node *doing,Node *done, long task_id) {
     Node *node = get_task_using_id(doing, task_id);
     if(node == NULL) return -1;
     KanbanTask *task = node->value;

     time_t curr_time;
     time(&curr_time);
     task->finish_date = curr_time;
     task->state = DONE;

     list_add_inorder(done, task, sizeof(Node), done_comparator);
     list_remove(doing, node);
     return 0;
}

int command_reopen(Node *todo,Node *done, long task_id) {
     Node *node = get_task_using_id(doing, task_id);
     if(node == NULL) return -1;
     KanbanTask *task = node->value;

     free(task->worker);
     task->deadline = NULL;
     task->finish_date = NULL;
     task->state = TODO;

     list_add_inorder(todo, task, sizeof(Node), todo_comparator);
     list_remove(done, node);
     return 0;
}


Node *get_task_using_id(Node *head, long task_id) {
     Node *curr = head;
     while (curr != NULL && curr->id != task_id) {
          curr = curr->next;
     }
     if (curr == NULL) return NULL;

     return curr;
}
