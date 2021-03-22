#include <time.h>

#define TASK_SERIALIZE_BUFFER_SIZE 1024

typedef enum { TODO, DOING, DONE } kanban_state;

typedef struct tm Date;  // defined in time.h; hours, minutes and seconds
                         // aren't initialized

typedef struct kanban_task {
    long id;
    int priority;  // value varies from [1-10]
    Date *creation_date;
    char *description;
    char *worker;  // name of the person working on the task
    Date *deadline;
    Date *finish_date;  // date of conclusion
    kanban_state state;
} KanbanTask;

KanbanTask *create_task(int id, char *desc, int priority);

void task_free(KanbanTask *task);

KanbanTask *task_assign(KanbanTask *task, char *worker_name);

KanbanTask *task_set_deadline(KanbanTask *task, int day, int month, int year);

KanbanTask *task_set_finish(KanbanTask *task, int day, int month, int year);

KanbanTask *task_set_state(
    KanbanTask *task,
    kanban_state state);  // state value varies from [0-2]

KanbanTask *task_set_priority(KanbanTask *task, int priority);

KanbanTask *task_reopen(KanbanTask *task);

char *task_serialize(KanbanTask *task);

KanbanTask *task_deserialize(char *input);