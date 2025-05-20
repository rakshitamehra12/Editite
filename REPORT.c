#include <stdio.h>
#include <string.h>

#define MAX_TEXT_SIZE 1000
#define MAX_STACK_SIZE 100

typedef struct {
    char operation[10];
    int position;
    char text[MAX_TEXT_SIZE];
} Change;

char text_array[MAX_TEXT_SIZE] = "";
Change undo_stack[MAX_STACK_SIZE];
int undo_top = -1;
Change redo_stack[MAX_STACK_SIZE];
int redo_top = -1;

void push_undo(Change change) {
    undo_stack[++undo_top] = change;
}

Change pop_undo() {
    return undo_stack[undo_top--];
}

void push_redo(Change change) {
    redo_stack[++redo_top] = change;
}

Change pop_redo() {
    return redo_stack[redo_top--];
}

void add_text(int position, const char* text) {
    Change change = {"add", position, ""};
    strcpy(change.text, text);
    push_undo(change);

    memmove(text_array + position + strlen(text), text_array + position, strlen(text_array) - position + 1);
    memcpy(text_array + position, text, strlen(text));
}

void delete_text(int position, int length) {
    Change change = {"delete", position, ""};
    strncpy(change.text, text_array + position, length);
    push_undo(change);

    memmove(text_array + position, text_array + position + length, strlen(text_array) - position - length + 1);
}

void undo() {
    if (undo_top < 0) {
        printf("Nothing to undo.\n");
        return;
    }
    Change change = pop_undo();
    if (strcmp(change.operation, "add") == 0) {
        delete_text(change.position, strlen(change.text));
        push_redo(change);
    } else if (strcmp(change.operation, "delete") == 0) {
        add_text(change.position, change.text);
        push_redo(change);
    }
}

void redo() {
    if (redo_top < 0) {
        printf("Nothing to redo.\n");
        return;
    }
    Change change = pop_redo();
    if (strcmp(change.operation, "add") == 0) {
        add_text(change.position, change.text);
    } else if (strcmp(change.operation, "delete") == 0) {
        delete_text(change.position, strlen(change.text));
    }
    push_undo(change);
}

void display_text() {
    printf("\nIndex: ");
    for (int i = 0; i < strlen(text_array); i++) {
        printf("%3d ", i);
    }
    printf("\nText:  %s\n", text_array);
}

void command_line_interface() {
    char command[20];
    int position, length;
    char text[MAX_TEXT_SIZE];

    printf("Welcome to the CLI Text Editor! Type 'help' for commands.\n");

    while (1) {
        printf("\n> ");
        scanf("%s", command);

        if (strcmp(command, "add") == 0) {
            scanf("%d %[^\n]", &position, text);
            add_text(position, text);
        } else if (strcmp(command, "delete") == 0) {
            scanf("%d %d", &position, &length);
            delete_text(position, length);
        } else if (strcmp(command, "undo") == 0) {
            undo();
        } else if (strcmp(command, "redo") == 0) {
            redo();
        } else if (strcmp(command, "display") == 0) {
            display_text();
        } else if (strcmp(command, "help") == 0) {
            printf("Commands:\n");
            printf("  add <pos> <text>    - Insert text at position start from 0\n");
            printf("  delete <pos> <len>  - Remove text of given length\n");
            printf("  undo                - Undo last operation\n");
            printf("  redo                - Redo last operation\n");
            printf("  display             - Show text with indexing\n");
            printf("  exit                - Quit editor\n");
        } else if (strcmp(command, "exit") == 0) {
            printf("Exiting CLI Text Editor. Goodbye!\n");
            break;
        } else {
            printf("Invalid command! Type 'help' for a list of commands.\n");
        }
    }
}

int main() {
    command_line_interface();
    return 0;
}
