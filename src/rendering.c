#include "rendering.h"

#include <kanban_task.h>
#include <string.h>

void print_n_char(int n, char c) {
    if (n < 0) return;
    while (n--) putchar(c);
}

void render_list_header(kanban_state state) {
    print_n_char(PRINT_WIDTH, '#');
    putchar('\n');
    char* list_name;
    switch (state) {
        case TODO:
            list_name = "TODO";
            break;
        case DOING:
            list_name = "DOING";
            break;
        case DONE:
            list_name = "DONE";
            break;
    }
    size_t ln_len = strlen(list_name);
    putchar('#');
    print_n_char((PRINT_WIDTH - 2 - ln_len) / 2, ' ');
    printf("%s", list_name);
    print_n_char((PRINT_WIDTH - 1 - ln_len) / 2, ' ');
    putchar('#');
    putchar('\n');
    print_n_char(PRINT_WIDTH, '#');
    putchar('\n');
}

void render_card_top_line() {
    putchar('#');
    print_n_char(PRINT_WIDTH - 2, '_');
    putchar('#');
    putchar('\n');
}

void render_card_bottom_line() {
    putchar('#');
    print_n_char(PRINT_WIDTH - 2, '-');
    putchar('#');
    putchar('\n');
}

void render_card_padding() {
    putchar('#');
    putchar('|');
    print_n_char(PRINT_WIDTH - 4, ' ');
    putchar('|');
    putchar('#');
    putchar('\n');
}

int num_digits(int n) {
    int d = 0;
    while (n != 0) {
        n /= 10;
        d++;
    }
    return d;
}

void render_task_id(KanbanTask* task) {
    putchar('#');
    putchar('|');
    int digits = num_digits(task->id);
    print_n_char((PRINT_WIDTH - digits - 8) / 2, ' ');
    printf("ID: %ld", task->id);
    print_n_char((PRINT_WIDTH - digits - 7) / 2, ' ');
    putchar('|');
    putchar('#');
    putchar('\n');
}

void render_task_creation_date(KanbanTask* task) {
    putchar('#');
    putchar('|');
    print_n_char((PRINT_WIDTH - 29) / 2, ' ');
    Date* d = localtime(&task->creation_date);
    printf("Creation Date: %02d/%02d/%04d", d->tm_mday, d->tm_mon + 1,
           d->tm_year + 1900);
    print_n_char((PRINT_WIDTH - 28) / 2, ' ');
    putchar('|');
    putchar('#');
    putchar('\n');
}

void render_task_description(KanbanTask* task) {
    size_t desc_len = strlen(task->description) + 13;
    size_t line_max_len = PRINT_WIDTH - 6;
    int lines = (desc_len + line_max_len - 1) / line_max_len;
    char* desc_str = "Description: ";

    for (int l = 0; l < lines; l++) {
        putchar('#');
        putchar('|');
        putchar(' ');

        int chars_left = desc_len - (line_max_len * l);
        int n = chars_left < line_max_len ? chars_left : line_max_len;

        for (int i = 0; i < n; i++) {
            if (l == 0 && i < 13) {
                putchar(desc_str[i]);
            } else {
                putchar(task->description[(l * line_max_len) + i - 13]);
            }
        }

        if (chars_left < line_max_len) {
            print_n_char(line_max_len - chars_left, ' ');
        }

        putchar(' ');
        putchar('|');
        putchar('#');
        putchar('\n');
    }
}

void render_task_worker(KanbanTask* task) {
    putchar('#');
    putchar('|');
    printf(" Worker: %s", task->worker);
    size_t worker_len = strlen(task->worker);
    print_n_char((PRINT_WIDTH - worker_len - 13), ' ');
    putchar('|');
    putchar('#');
    putchar('\n');
}

void render_task_deadline(KanbanTask* task) {
    putchar('#');
    putchar('|');
    print_n_char((PRINT_WIDTH - 24) / 2, ' ');
    Date* d = localtime(&task->deadline);
    printf("Deadline: %02d/%02d/%04d", d->tm_mday, d->tm_mon + 1,
           d->tm_year + 1900);
    print_n_char((PRINT_WIDTH - 23) / 2, ' ');
    putchar('|');
    putchar('#');
    putchar('\n');
}

void render_task_finish(KanbanTask* task) {
    putchar('#');
    putchar('|');
    print_n_char((PRINT_WIDTH - 27) / 2, ' ');
    Date* d = localtime(&task->finish_date);
    printf("Finished At: %02d/%02d/%04d", d->tm_mday, d->tm_mon + 1,
           d->tm_year + 1900);
    print_n_char((PRINT_WIDTH - 26) / 2, ' ');
    putchar('|');
    putchar('#');
    putchar('\n');
}

void render_task_priority(KanbanTask* task) {
    putchar('#');
    putchar('|');
    int digits = num_digits(task->priority);
    print_n_char((PRINT_WIDTH - digits - 14) / 2, ' ');
    printf("Priority: %d", task->priority);
    print_n_char((PRINT_WIDTH - digits - 13) / 2, ' ');
    putchar('|');
    putchar('#');
    putchar('\n');
}

void render_list_content(Node* head) {
    Node* curr = head;
    while (curr != NULL) {
        KanbanTask* task = (KanbanTask*)curr->value;

        render_card_top_line();
        render_card_padding();

        render_task_id(task);
        render_task_priority(task);
        render_task_creation_date(task);
        if (task->deadline != -1) render_task_deadline(task);
        if (task->finish_date != -1) render_task_finish(task);

        render_card_padding();

        render_task_description(task);

        render_card_padding();

        if (task->worker != NULL) render_task_worker(task);

        render_card_padding();

        render_card_bottom_line();
        render_list_padding();

        curr = curr->next;
    }
}

void render_list_padding() {
    putchar('#');
    print_n_char(PRINT_WIDTH - 2, ' ');
    putchar('#');
    putchar('\n');
}

void render_list_footer() {
    print_n_char(PRINT_WIDTH, '#');
    putchar('\n');
}