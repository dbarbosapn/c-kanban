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
    print_n_char((PRINT_WIDTH - 28) / 2, ' ');
    Date* d = localtime(&task->creation_date);
    printf("Creation Date: %02d/%02d/%04d", d->tm_mday, d->tm_mon + 1,
           d->tm_year + 1900);
    print_n_char((PRINT_WIDTH - 29) / 2, ' ');
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
        render_task_creation_date(task);

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