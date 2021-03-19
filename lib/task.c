#include <stdlib.h>
#include <string.h>
#include <task.h>
/**
 * Creates new Task without an apointed worker nor deadline
 * id parameter must be unique
 **/
Task *create_task(int id, char *descr, int priority) {
    Task *task = malloc(sizeof(Task));
    task->id = id;
    task->priority = priority;
    task->state = 0;

    time_t rawtime;
    time(&rawtime);
    Date *d = localtime(&rawtime);
    task->creation = malloc(sizeof(Date));
    task->creation->tm_mday = d->tm_mday;
    task->creation->tm_mon = d->tm_mon;
    task->creation->tm_year = d->tm_year;

    size_t descrLen = strlen(descr);
    task->description = malloc((descrLen + 1) * sizeof(char));
    strncpy(task->description, descr, descrLen);
    task->description[descrLen] = '\0';

    return task;
}

/**
 * Deletes task from memory
 **/
void free_task(Task *task) {
    if (task->creation != NULL) free(task->creation);

    if (task->description != NULL) free(task->description);

    if (task->worker != NULL) free(task->worker);

    if (task->deadline != NULL) free(task->deadline);

    task->creation = NULL;
    task->deadline = NULL;
    task->conclusion = NULL;
    task->worker = NULL;

    free(task);
}

/**
 * Assigns task to worker
 **/
Task *assign_task(Task *task, char *workerName) {
    if (task->worker != NULL) {
        free(task->worker);
    }

    size_t nameLen = strlen(workerName);
    task->worker = malloc((nameLen + 1) * sizeof(char));
    strncpy(task->worker, workerName, nameLen);
    task->worker[nameLen] = '\0';

    return task;
}

/**
 * Gives deadline to Task
 **/
Task *deadline(Task *task, int day, int month, int year) {
    task->deadline = malloc(sizeof(Date));
    task->deadline->tm_mday = day;
    task->deadline->tm_mon = month;
    task->deadline->tm_year = year;
    return task;
}

/**
 * Gives conclusion date to Task
 **/
Task *conclusion(Task *task, int day, int month, int year) {
    task->conclusion = malloc(sizeof(Date));
    task->conclusion->tm_mday = day;
    task->conclusion->tm_mon = month;
    task->conclusion->tm_year = year;
    return task;
}

/**
 * Updates state
 **/
Task *update_state(Task *task, int state) {
    task->state = state;
    return task;
}

/**
 * Changes priority
 **/
Task *update_priority(Task *task, int priority) {
    task->priority = priority;
    return task;
}

/**
 * Reopens a done task
 *    Done->To Do
 **/
Task *reopen(Task *task) {
    free(task->deadline);
    free(task->conclusion);
    free(task->worker);

    task->deadline = NULL;
    task->conclusion = NULL;
    task->worker = NULL;

    task->state = 0;
    return task;
}
