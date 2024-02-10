#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <stdint.h>

typedef struct CodeLine CodeLine;

struct CodeLine {
    uint32_t line_number;
    uint8_t* code;
    CodeLine* next;
    CodeLine* prev;
};

void doc_clear();
void doc_set_line(uint32_t number, const uint8_t* line, size_t length);
void doc_list();

#endif