#include <time.h>

typedef struct tm
    Date;  // defined in time.h; hours, minutes and seconds aren't initialized

typedef struct task {
    int id;
    int priority;  // value varies from [1-10]
    Date *creation;
    char *description;
    char *worker;  // name of the person working on the task
    Date *deadline;
    Date *conclusion;  // date of conclusion
    int state;         // 0,1,2 -> todo, doing, done;  default value = 0
} Task;

Task *create_task(int id, char *descr, int priority);

void free_task(Task *task);

Task *assign_task(Task *task, char *workerName);

Task *deadline(Task *task, int day, int month, int year);

Task *conclusion(Task *task, int day, int month, int year);

Task *update_state(Task *task, int state);  // state value varies from [0-2]

Task *update_priority(Task *task, int priority);

Task *reopen(Task *task);

// TODO: compare dates between 2 tasks
