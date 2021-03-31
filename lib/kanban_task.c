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

    task->worker = NULL;
    task->deadline = -1;
    task->finish_date = -1;

    task->creation_date = curr_time;

    task->description = malloc(sizeof(char) * strlen(desc) + 1);
    strcpy(task->description, desc);

    return task;
}

/**
 * Frees the dynamically allocated memory for the given task.
 **/
void delete_task(KanbanTask *task) {
    if (task->worker != NULL) free(task->worker);
    free(task->description);

    free(task);
}

/**
 * Assigns task to worker
 **/
KanbanTask *task_assign(KanbanTask *task, char *worker_name) {
    if (task->worker != NULL) free(task->worker);
    task->worker = malloc(sizeof(char) * strlen(worker_name) + 1);
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
char *task_serialize(void *task_raw) {
    KanbanTask *task = (KanbanTask *)task_raw;

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
    if (task->creation_date != -1) {
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
    if (task->deadline != -1) {
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
    if (task->finish_date != -1) {
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
void task_save(void *task_raw, FILE *fp) {
    KanbanTask *task = (KanbanTask *)task_raw;
    fwrite(&task->id, sizeof(long), 1, fp);
    fwrite(&task->priority, sizeof(int), 1, fp);
    fwrite(&task->creation_date, sizeof(time_t), 1, fp);
    int ldesc = strlen(task->description);
    fwrite(&ldesc, sizeof(int), 1, fp);
    fwrite(task->description, sizeof(char), ldesc, fp);
    int lworker = 0;

    if (task->worker != NULL) {
        lworker = strlen(task->worker);
        fwrite(&lworker, sizeof(int), 1, fp);
        fwrite(task->worker, sizeof(char), lworker, fp);
    } else {
        fwrite(&lworker, sizeof(int), 1, fp);
    }

    fwrite(&task->deadline, sizeof(time_t), 1, fp);
    fwrite(&task->finish_date, sizeof(time_t), 1, fp);
    fwrite(&task->state, sizeof(kanban_state), 1, fp);
}

/**
 * Loads a KanbanTask from the given file pointer. The file pointer must have
 * read binary permissions.
 **/
void *task_load(FILE *fp) {
    KanbanTask *task = malloc(sizeof(KanbanTask));
    int rlen = fread(&task->id, sizeof(long), 1, fp);
    if (rlen < 1) {
        free(task);
        return NULL;
    }

    fread(&task->priority, sizeof(int), 1, fp);
    fread(&task->creation_date, sizeof(time_t), 1, fp);

    int ldesc;
    fread(&ldesc, sizeof(int), 1, fp);
    if (ldesc > 0) {
        char *description = malloc(sizeof(char) * ldesc + 1);
        fread(description, sizeof(char), ldesc, fp);
        description[ldesc] = '\0';
        task->description = description;
    } else {
        task->description = "";
    }

    int lworker;
    fread(&lworker, sizeof(int), 1, fp);
    if (lworker > 0) {
        char *worker = malloc(sizeof(char) * lworker + 1);
        fread(worker, sizeof(char), lworker, fp);
        worker[lworker] = '\0';
        task->worker = worker;
    } else {
        task->worker = NULL;
    }

    fread(&task->deadline, sizeof(time_t), 1, fp);
    fread(&task->finish_date, sizeof(time_t), 1, fp);
    fread(&task->state, sizeof(kanban_state), 1, fp);

    return task;
}

/**
 * Comparator for KanbanTask (by priority and then by creation_date)
 **/
int task_todo_comparator(void *task1_raw, void *task2_raw) {
    KanbanTask *task1 = (KanbanTask *)task1_raw;
    KanbanTask *task2 = (KanbanTask *)task2_raw;

    int result = task1->priority - task2->priority;

    if (result == 0) {
        result = task1->creation_date - task2->creation_date;
    }

    return result;
}

/**
 * Comparator for KanbanTask (by worker_name)
 **/
int task_doing_comparator(void *task1_raw, void *task2_raw) {
    KanbanTask *task1 = (KanbanTask *)task1_raw;
    KanbanTask *task2 = (KanbanTask *)task2_raw;

    return strcmp(task1->worker, task2->worker);
}

/**
 * Comparator for KanbanTask (by finish_date)
 **/
int task_done_comparator(void *task1_raw, void *task2_raw) {
    KanbanTask *task1 = (KanbanTask *)task1_raw;
    KanbanTask *task2 = (KanbanTask *)task2_raw;

    return task1->finish_date - task2->finish_date;
}

/**
 * Comparator for KanbanTask (by creation_date)
 **/
int task_all_comparator(void *task1_raw, void *task2_raw) {
    KanbanTask *task1 = (KanbanTask *)task1_raw;
    KanbanTask *task2 = (KanbanTask *)task2_raw;

    return task1->creation_date - task2->creation_date;
}