#include "document.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"

static CodeLine* root = NULL;

static void free_code_line(CodeLine* code_line) {
    free(code_line->code);
    free(code_line);
}

void doc_clear() {
    CodeLine* line = root;
    while (line != NULL) {
        CodeLine* next = line->next;
        free_code_line(line);
        line = next;
    }
    root = NULL;
}

static uint8_t* copy_code(const uint8_t* line, size_t length) {
    uint8_t* newline = (uint8_t*)malloc(length);
    memcpy(newline, line, length);
    return newline;
}

void doc_set_line(uint32_t number, const uint8_t* line, size_t length) {
    CodeLine* new_code_line = malloc(sizeof(CodeLine));
    new_code_line->line_number = number;
    new_code_line->code = copy_code(line, length);
    new_code_line->prev = NULL;
    new_code_line->next = NULL;

    if (root == NULL) {
        root = new_code_line;
        return;
    }

    CodeLine* code_line;

    for (code_line = root; (code_line->next != NULL) && (code_line->line_number < number); code_line = code_line->next) {
    }

    if (code_line->line_number == number) {
        // Replace
        new_code_line->prev = code_line->prev;
        new_code_line->next = code_line->next;
        if (code_line->prev != NULL) {
            code_line->prev->next = new_code_line;
        }
        if (code_line->next != NULL) {
            code_line->next->prev = new_code_line;
        }
        free_code_line(code_line);
    } else if (code_line->line_number < number) {
        // Insert after
        new_code_line->next = code_line->next;
        new_code_line->prev = code_line;
        if (code_line->next != NULL) {
            code_line->next->prev = new_code_line;
        }
        code_line->next = new_code_line;
    } else {
        // Insert before
        new_code_line->prev = code_line->prev;
        new_code_line->next = code_line;
        if (code_line->prev != NULL) {
            code_line->prev->next = new_code_line;
        }
        code_line->prev = new_code_line;
    }
}

void doc_list() {
    for (CodeLine* code_line = root; code_line != NULL; code_line = code_line->next) {
        printf("%d ", code_line->line_number);
        print_shrinked_line(code_line->code);
        printf("\n");
    }
}