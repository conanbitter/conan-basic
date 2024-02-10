#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "lexer.h"
#include "document.h"

#define INPUT_BUFFER_SIZE 256

char input_buffer[INPUT_BUFFER_SIZE];

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

bool working;

int main() {
    printf("Hello world!\n");
    doc_clear();
    working = true;
    while (working) {
        get_input();
        size_t line_length;
        const uint8_t* line = shrink_line(input_buffer, &line_length);
        if (*line == TOKEN_INT) {
            uint32_t number;
            line = get_int(line, &number);
            line_length -= 5;
            doc_set_line(number, line, line_length);
        } else if (*line == TOKEN_KEYWORD + KW_LIST) {
            doc_list();
        } else {
            print_shrinked_line(line);
            printf("\n");
        }
    }
    return 0;
}