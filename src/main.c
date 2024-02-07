#include <stdio.h>
#include <string.h>

#define INPUT_BUFFER_SIZE 5

char input_buffer[INPUT_BUFFER_SIZE];

void read_input() {
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
            input_buffer[INPUT_BUFFER_SIZE - 1] = '\n';
        }
        break;
    }
}

int main() {
    printf("Hello world!\n");
    while (1) {
        read_input();
        printf(": %s\n", input_buffer);
    }
    return 0;
}