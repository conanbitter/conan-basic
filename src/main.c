#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "lexer.h"

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
    working = true;
    while (working) {
        get_input();
        uint8_t* line = shrink_line(input_buffer);
        print_shrinked_line(line);
        printf("\n");
    }
    return 0;
}