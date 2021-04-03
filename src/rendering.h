#include <kanban_task.h>
#include <linked_list.h>

#define PRINT_WIDTH 50

void render_list_header(kanban_state state, Node* list);

void render_list_padding();

void render_list_content(Node* head);

void render_list_footer();