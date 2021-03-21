#include <kanban_task.h>
#include <stdlib.h>
#include <string.h>

/**
 * Creates new KanbanTask without an apointed worker nor deadline
 * id parameter must be unique
 **/
KanbanTask *create_task(char *desc, int priority) {
    KanbanTask *task = malloc(sizeof(KanbanTask));

    time_t rawtime;
    time(&rawtime);

    // The ID is the long value of rawtime. This will ensure it is unique
    task->id = rawtime;
    task->priority = priority;
    task->state = TODO;

    task->worker = NULL;
    task->deadline = NULL;
    task->finish_date = NULL;

    Date *d = localtime(&rawtime);
    task->creation_date = malloc(sizeof(Date));
    memcpy(task->creation_date, d, sizeof(Date));

    size_t desc_len = strlen(desc);
    task->description = malloc((desc_len + 1) * sizeof(char));
    strncpy(task->description, desc, desc_len);
    task->description[desc_len] = '\0';

    return task;
}

/**
 * Deletes task from memory
 **/
void task_free(KanbanTask *task) {
    if (task->creation_date != NULL) free(task->creation_date);

    if (task->description != NULL) free(task->description);

    if (task->worker != NULL) free(task->worker);

    if (task->deadline != NULL) free(task->deadline);

    task->creation_date = NULL;
    task->deadline = NULL;
    task->finish_date = NULL;
    task->worker = NULL;

    free(task);
}

/**
 * Assigns task to worker
 **/
KanbanTask *task_assign(KanbanTask *task, char *worker_name) {
    if (task->worker != NULL) {
        free(task->worker);
    }

    size_t name_len = strlen(worker_name);
    task->worker = malloc((name_len + 1) * sizeof(char));
    strncpy(task->worker, worker_name, name_len);
    task->worker[name_len] = '\0';

    return task;
}

/**
 * Sets task deadline
 **/
KanbanTask *task_set_deadline(KanbanTask *task, int day, int month, int year) {
    task->deadline = malloc(sizeof(Date));
    task->deadline->tm_mday = day;
    task->deadline->tm_mon = month;
    task->deadline->tm_year = year;
    return task;
}

/**
 * Gives conclusion date to KanbanTask
 **/
KanbanTask *task_set_finish(KanbanTask *task, int day, int month, int year) {
    task->finish_date = malloc(sizeof(Date));
    task->finish_date->tm_mday = day;
    task->finish_date->tm_mon = month;
    task->finish_date->tm_year = year;
    return task;
}

/**
 * Updates state
 **/
KanbanTask *task_set_state(KanbanTask *task, kanban_state state) {
    task->state = state;
    return task;
}

/**
 * Changes priority
 **/
KanbanTask *task_set_priority(KanbanTask *task, int priority) {
    task->priority = priority;
    return task;
}

/**
 * Reopens a done task
 *    DONE -> TODO
 **/
KanbanTask *task_reopen(KanbanTask *task) {
    free(task->deadline);
    free(task->finish_date);
    free(task->worker);

    task->deadline = NULL;
    task->finish_date = NULL;
    task->worker = NULL;

    task->state = TODO;
    return task;
}
