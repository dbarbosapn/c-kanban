#ifndef KANBAN_TASK_H

#define KANBAN_TASK_H

#include <stdio.h>
#include <time.h>

#define TASK_SERIALIZE_BUFFER_SIZE 1024
#define TASK_MAX_DOING 5

typedef enum { TODO, DOING, DONE } kanban_state;

typedef struct tm Date;  // defined in time.h; hours, minutes and seconds
                         // aren't initialized

typedef struct kanban_task {
    long id;
    int priority;  // value varies from [1-10]
    time_t creation_date;
    char *description;
    char *worker;  // name of the person working on the task
    time_t deadline;
    time_t finish_date;  // date of conclusion
    kanban_state state;
} KanbanTask;

KanbanTask *create_task(int id, char *desc, int priority);

void delete_task(KanbanTask *task);

KanbanTask *task_assign(KanbanTask *task, char *worker_name);

KanbanTask *task_set_deadline(KanbanTask *task, int day, int month, int year);

KanbanTask *task_set_finish(KanbanTask *task, int day, int month, int year);

KanbanTask *task_set_state(KanbanTask *task, kanban_state state);

KanbanTask *task_set_priority(KanbanTask *task, int priority);

char *task_serialize(void *task);

void task_save(void *task, FILE *fp);

void *task_load(FILE *fp);

int task_todo_comparator(void *task1, void *task2);

int task_doing_comparator(void *task1, void *task2);

int task_done_comparator(void *task1, void *task2);

int task_all_comparator(void *task1, void *task2);

#endif