#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define INPUT_BUFFER_SIZE 256
#define NAME_BUFFER_SIZE 256

char input_buffer[INPUT_BUFFER_SIZE];
char name_buffer[NAME_BUFFER_SIZE];

bool is_number(char symbol) {
    return symbol >= '0' && symbol <= '9';
}

bool is_space(char symbol) {
    return symbol == ' ' || symbol == '\t';
}

bool is_alpha(char symbol) {
    return (symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z');
}

char* caret;

int read_int() {
    int result = 0;
    while (*caret != '\0' && is_number(*caret)) {
        result = result * 10 + *caret - '0';
        caret++;
    }
    return result;
}

int read_name() {
    char* name = name_buffer;
    int length = 0;
    while (*caret != '\0' && is_alpha(*caret) && length < NAME_BUFFER_SIZE) {
        if (*caret >= 'A' && *caret <= 'Z') {
            *name = *caret;
        } else {
            *name = *caret - 'a' + 'A';
        }
        *caret++;
        *name++;
        length++;
    }
    *name = '\0';
}

bool assert_and_skip_spaces() {
    if (!is_space(*caret)) {
        return 1;
    }
    while (*caret != '\0' && is_space(*caret)) {
        caret++;
    }
    return 0;
}

void get_input() {
    while (1) {
        printf("> ");
        fflush(stdout);

        if (fgets(input_buffer, INPUT_BUFFER_SIZE, stdin) == NULL) {
            continue;
        }

        if (input_buffer[0] == '\n') {
            continue;
        }

        if (input_buffer[strlen(input_buffer) - 1] != '\n') {
            int ch;
            while (((ch = getchar()) != '\n') && (ch != EOF)) {
            }
            input_buffer[INPUT_BUFFER_SIZE - 1] = '\0';
            return;
        }

        input_buffer[strlen(input_buffer) - 1] = '\0';
        return;
    }
}

void process_input() {
    caret = input_buffer;
    if (is_number(*caret)) {
        int line_num = read_int();
        printf("number %d\n", line_num);
        if (assert_and_skip_spaces() == 1) {
            printf("Syntax error: no spaces after line number\n");
            return;
        }
    }
    if (is_alpha(*caret)) {
        read_name();
        printf("name \"%s\"\n", name_buffer);
    }
    printf(": \"%s\"\n", caret);
}

int main() {
    printf("Hello world!\n");
    while (1) {
        get_input();
        process_input();
    }
    return 0;
}