#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>

#define STRING_BUFFER_SIZE 256
#define SHRINK_LINE_SIZE 256
#define KEYWORD_COUNT 49

typedef enum TokenType {
    TOKEN_COMMA = 0,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_INTDIV,
    TOKEN_EXP,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EQ,
    TOKEN_NEQ,
    TOKEN_BIG,
    TOKEN_SML,
    TOKEN_BIGEQ,
    TOKEN_SMLEQ,
    TOKEN_NAME,
    TOKEN_INT,
    TOKEN_STR,
    TOKEN_FLOAT,
    TOKEN_DOUBLE,
    TOKEN_ERROR,
    TOKEN_EOL,
    TOKEN_KEYWORD,
} TokenType;

typedef enum TokenKeyword {
    KW_AND,
    KW_AUTO,
    KW_CLOSE,
    KW_DATA,
    KW_DEF,
    KW_DEFDBL,
    KW_DEFINT,
    KW_DEFSNG,
    KW_DEFSTR,
    KW_DELETE,
    KW_DIM,
    KW_EDIT,
    KW_ELSE,
    KW_END,
    KW_EQV,
    KW_ERASE,
    KW_FOR,
    KW_GOSUB,
    KW_GOTO,
    KW_IF,
    KW_IMP,
    KW_INPUT,
    KW_LET,
    KW_LIST,
    KW_LOAD,
    KW_MOD,
    KW_NEW,
    KW_NEXT,
    KW_NOT,
    KW_ON,
    KW_OPEN,
    KW_OR,
    KW_PRINT,
    KW_RANDOMIZE,
    KW_READ,
    KW_REM,
    KW_RENUM,
    KW_RESTORE,
    KW_RETURN,
    KW_RUN,
    KW_SAVE,
    KW_STEP,
    KW_SWAP,
    KW_THEN,
    KW_TO,
    KW_USING,
    KW_WEND,
    KW_WHILE,
    KW_XOR
} TokenKeyword;

uint8_t* shrink_line(const char* input, size_t* length);
void print_shrinked_line(const uint8_t* line);
const uint8_t* get_int(const uint8_t* line, uint32_t* value);

#endif