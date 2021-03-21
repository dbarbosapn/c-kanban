#include <kanban_task.h>
#include <stdlib.h>
#include <string.h>

/**
 * Creates new KanbanTask without an apointed worker nor deadline
 * id parameter must be unique
 **/
KanbanTask *create_task(int id, char *desc, int priority) {
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

/**
 * Serializes the given task
 */
char *task_serialize(KanbanTask *task) {
    char *buffer = malloc(TASK_SERIALIZE_BUFFER_SIZE * sizeof(char));
    char value_buffer[50];
    buffer[0] = '{';
    int i = 1;

    // ID
    sprintf(value_buffer, "%d", task->id);
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
        time_t cd_raw = mktime(task->creation_date);
        sprintf(value_buffer, "%d", cd_raw);
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
        time_t deadline_raw = mktime(task->deadline);
        sprintf(value_buffer, "%d", deadline_raw);
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
        time_t finish_raw = mktime(task->finish_date);
        sprintf(value_buffer, "%d", finish_raw);
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
 * Deserializes the given input. Returns a task.
 */
KanbanTask *task_deserialize(char *input) {
    KanbanTask *task = malloc(sizeof(KanbanTask));
    char value_buffer[50];
    int i = 1;
    int j;

    // ID
    for (j = 0; input[i] != ','; j++, i++) {
        value_buffer[j] = input[i];
    }
    value_buffer[j] = '\0';
    i++;
    task->id = atoi(value_buffer);

    // PRIORITY
    for (j = 0; input[i] != ','; j++, i++) {
        value_buffer[j] = input[i];
    }
    value_buffer[j] = '\0';
    i++;
    task->priority = atoi(value_buffer);

    // CREATION DATE
    for (j = 0; input[i] != ','; j++, i++) {
        value_buffer[j] = input[i];
    }
    value_buffer[j] = '\0';
    i++;
    time_t creation_raw = atoi(value_buffer);
    Date *cd = localtime(&creation_raw);
    task->creation_date = malloc(sizeof(Date));
    memcpy(task->creation_date, cd, sizeof(Date));

    // DESCRIPTION
    for (j = 0; input[i] != ','; j++, i++) {
        value_buffer[j] = input[i];
    }
    value_buffer[j] = '\0';
    i++;
    size_t desc_len = strlen(value_buffer);
    task->description = malloc((desc_len + 1) * sizeof(char));
    strncpy(task->description, value_buffer, desc_len);
    task->description[desc_len] = '\0';

    // WORKER
    for (j = 0; input[i] != ','; j++, i++) {
        value_buffer[j] = input[i];
    }
    value_buffer[j] = '\0';
    i++;
    task->worker = NULL;
    task_assign(task, value_buffer);

    // DEADLINE
    for (j = 0; input[i] != ','; j++, i++) {
        value_buffer[j] = input[i];
    }
    value_buffer[j] = '\0';
    i++;
    time_t deadline_raw = atoi(value_buffer);
    Date *d = localtime(&deadline_raw);
    task->deadline = malloc(sizeof(Date));
    memcpy(task->deadline, d, sizeof(Date));

    // FINISH DATE
    for (j = 0; input[i] != ','; j++, i++) {
        value_buffer[j] = input[i];
    }
    value_buffer[j] = '\0';
    i++;
    time_t finish_raw = atoi(value_buffer);
    Date *fd = localtime(&finish_raw);
    task->finish_date = malloc(sizeof(Date));
    memcpy(task->finish_date, fd, sizeof(Date));

    // STATE
    for (j = 0; input[i] != '}'; j++, i++) {
        value_buffer[j] = input[i];
    }
    value_buffer[j] = '\0';
    i++;
    if (strcmp(value_buffer, "TODO") == 0) {
        task->state = TODO;
    } else if (strcmp(value_buffer, "DOING") == 0) {
        task->state = DOING;
    } else {
        task->state = DONE;
    }

    return task;
}