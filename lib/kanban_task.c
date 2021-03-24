#include <kanban_task.h>
#include <stdlib.h>
#include <string.h>

/**
 * Creates new KanbanTask without an apointed worker nor deadline
 * id parameter must be unique
 **/
KanbanTask *create_task(int id, char *desc, int priority) {
    KanbanTask *task = malloc(sizeof(KanbanTask));

    time_t curr_time;
    time(&curr_time);

    task->id = id;
    task->priority = priority;
    task->state = TODO;

    strcpy(task->worker, "");
    task->deadline = NULL;
    task->finish_date = NULL;

    task->creation_date = curr_time;

    strcpy(task->description, desc);

    return task;
}

/**
 * Assigns task to worker
 **/
KanbanTask *task_assign(KanbanTask *task, char *worker_name) {
    strcpy(task->worker, worker_name);
    return task;
}

/**
 * Sets task deadline
 **/
KanbanTask *task_set_deadline(KanbanTask *task, int day, int month, int year) {
    time_t curr_time;
    time(&curr_time);
    Date *d = localtime(&curr_time);
    d->tm_mday = day;
    d->tm_mon = month - 1;
    d->tm_year = year - 1900;

    time_t deadline = mktime(d);
    task->deadline = deadline;

    return task;
}

/**
 * Gives conclusion date to KanbanTask
 **/
KanbanTask *task_set_finish(KanbanTask *task, int day, int month, int year) {
    time_t curr_time;
    time(&curr_time);
    Date *d = localtime(&curr_time);
    d->tm_mday = day;
    d->tm_mon = month - 1;
    d->tm_year = year - 1900;

    time_t finish = mktime(d);
    task->finish_date = finish;

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
 * Serializes the given task
 */
char *task_serialize(KanbanTask *task) {
    char *buffer = malloc(TASK_SERIALIZE_BUFFER_SIZE * sizeof(char));
    char value_buffer[50];
    buffer[0] = '{';
    int i = 1;

    // ID
    sprintf(value_buffer, "%ld", task->id);
    for (int j = 0; j < strlen(value_buffer); j++, i++) {
        buffer[i] = value_buffer[j];
    }
    buffer[i] = ',';
    i++;

    // PRIORITY
    sprintf(value_buffer, "%d", task->priority);
    for (int j = 0; j < strlen(value_buffer); j++, i++) {
        buffer[i] = value_buffer[j];
    }
    buffer[i] = ',';
    i++;

    // CREATION DATE
    if (task->creation_date != NULL) {
        sprintf(value_buffer, "%ld", task->creation_date);
    } else {
        strcpy(value_buffer, "NULL");
    }
    for (int j = 0; j < strlen(value_buffer); j++, i++) {
        buffer[i] = value_buffer[j];
    }
    buffer[i] = ',';
    i++;

    // DESCRIPTION
    for (int j = 0; j < strlen(task->description); j++, i++) {
        buffer[i] = task->description[j];
    }
    buffer[i] = ',';
    i++;

    // WORKER
    char *worker = "NULL";
    if (task->worker != NULL) {
        worker = task->worker;
    }

    for (int j = 0; j < strlen(worker); j++, i++) {
        buffer[i] = worker[j];
    }
    buffer[i] = ',';
    i++;

    // DEADLINE
    if (task->deadline != NULL) {
        sprintf(value_buffer, "%ld", task->deadline);
    } else {
        strcpy(value_buffer, "NULL");
    }
    for (int j = 0; j < strlen(value_buffer); j++, i++) {
        buffer[i] = value_buffer[j];
    }
    buffer[i] = ',';
    i++;

    // FINISH DATE
    if (task->finish_date != NULL) {
        sprintf(value_buffer, "%ld", task->finish_date);
    } else {
        strcpy(value_buffer, "NULL");
    }
    for (int j = 0; j < strlen(value_buffer); j++, i++) {
        buffer[i] = value_buffer[j];
    }
    buffer[i] = ',';
    i++;

    // STATE
    char *state_str;
    if (task->state == TODO)
        state_str = "TODO";
    else if (task->state == DOING)
        state_str = "DOING";
    else
        state_str = "DONE";

    for (int j = 0; j < strlen(state_str); j++, i++) {
        buffer[i] = state_str[j];
    }
    buffer[i] = '}';
    buffer[i + 1] = '\0';

    return buffer;
}

/**
 * Saves the given task in a binary file. The file pointer must have write
 * binary permissions.
 **/
void *task_save(KanbanTask *task, FILE *fp) {
    fwrite(task, sizeof(KanbanTask), 1, fp);
}

/**
 * Loads a KanbanTask from the given file pointer. The file pointer must have
 * read binary permissions.
 **/
KanbanTask *task_load(FILE *fp) {
    KanbanTask *task = malloc(sizeof(KanbanTask));
    int rlen = fread(task, sizeof(KanbanTask), 1, fp);

    if (rlen < 1) {
        free(task);
        return NULL;
    }

    return task;
}