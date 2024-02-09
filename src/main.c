#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define INPUT_BUFFER_SIZE 256
#define STRING_BUFFER_SIZE 256
#define SHRINK_LINE_SIZE 256
#define KEYWORD_COUNT 49

char input_buffer[INPUT_BUFFER_SIZE];
char string_buffer[STRING_BUFFER_SIZE];
int string_buffer_length;
uint8_t shrink_line[SHRINK_LINE_SIZE];
int shrink_line_length;
int32_t int_data;
char sigil;

typedef enum TokenType {
    TOKEN_COMMA,
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
    TOKEN_EOF,
    TOKEN_KEYWORD,
} TokenType;

const char* small_tokens[] = {
    ",",
    ":",
    ";",
    "+",
    "-",
    "*",
    "/",
    "\\",
    "^",
    "(",
    ")",
    "=",
    "<>",
    ">",
    "<",
    ">=",
    "<=",
};

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

const char* keywords[KEYWORD_COUNT] = {
    "AND",
    "AUTO",
    "CLOSE",
    "DATA",
    "DEF",
    "DEFDBL",
    "DEFINT",
    "DEFSNG",
    "DEFSTR",
    "DELETE",
    "DIM",
    "EDIT",
    "ELSE",
    "END",
    "EQV",
    "ERASE",
    "FOR",
    "GOSUB",
    "GOTO",
    "IF",
    "IMP",
    "INPUT",
    "LET",
    "LIST",
    "LOAD",
    "MOD",
    "NEW",
    "NEXT",
    "NOT",
    "ON",
    "OPEN",
    "OR",
    "PRINT",
    "RANDOMIZE",
    "READ",
    "REM",
    "RENUM",
    "RESTORE",
    "RETURN",
    "RUN",
    "SAVE",
    "STEP",
    "SWAP",
    "THEN",
    "TO",
    "USING",
    "WEND",
    "WHILE",
    "XOR",
};

char* caret;

bool is_number(char symbol) {
    return symbol >= '0' && symbol <= '9';
}

bool is_space(char symbol) {
    return symbol == ' ' || symbol == '\t';
}

bool is_alpha(char symbol) {
    return (symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z');
}

int is_small_token() {
    switch (*caret) {
        case ',':
            caret++;
            return TOKEN_COMMA;

        case ':':
            caret++;
            return TOKEN_COLON;

        case ';':
            caret++;
            return TOKEN_SEMICOLON;

        case '+':
            caret++;
            return TOKEN_PLUS;

        case '-':
            caret++;
            return TOKEN_MINUS;

        case '*':
            caret++;
            return TOKEN_MUL;

        case '/':
            caret++;
            return TOKEN_DIV;

        case '\\':
            caret++;
            return TOKEN_INTDIV;

        case '^':
            caret++;
            return TOKEN_EXP;

        case '(':
            caret++;
            return TOKEN_LPAREN;

        case ')':
            caret++;
            return TOKEN_RPAREN;

        case '=':
            caret++;
            return TOKEN_EQ;

        case '<':
            caret++;
            if (*caret == '>') {
                caret++;
                return TOKEN_NEQ;
            } else if (*caret == '=') {
                caret++;
                return TOKEN_SMLEQ;
            }
            return TOKEN_SML;

        case '>':
            caret++;
            if (*caret == '=') {
                caret++;
                return TOKEN_BIGEQ;
            }
            return TOKEN_BIG;
        default:
            return -1;
    }
}

int read_int() {
    int result = 0;
    while (*caret != '\0' && is_number(*caret)) {
        result = result * 10 + *caret - '0';
        caret++;
    }
    return result;
}

bool is_divider() {
    char* old_caret = caret;
    if (*caret == '\0' || is_space(*caret) || is_small_token() >= 0) {
        caret = old_caret;
        return true;
    }
    return false;
}

void read_name() {
    char* name = string_buffer;
    string_buffer_length = 0;
    while (*caret != '\0' && (is_alpha(*caret) || is_number(*caret) || *caret == '_' || *caret == '.') && string_buffer_length < STRING_BUFFER_SIZE) {
        if (*caret >= 'a' && *caret <= 'z') {
            *name = *caret - 'a' + 'A';
        } else {
            *name = *caret;
        }
        *caret++;
        *name++;
        string_buffer_length++;
    }
    *name = '\0';
}

bool read_string() {
    caret++;
    char* name = string_buffer;
    string_buffer_length = 0;
    while (*caret != '\0' && *caret != '"' && string_buffer_length < STRING_BUFFER_SIZE) {
        *name = *caret;
        *caret++;
        *name++;
        string_buffer_length++;
    }
    *name = '\0';
    if (*caret == '"') {
        caret++;
        return false;
    }
    return true;
}

void skip_spaces() {
    while (*caret != '\0' && is_space(*caret)) {
        caret++;
    }
}

TokenType get_token() {
    if (*caret == '\0') {
        return TOKEN_EOF;
    }
    if (*caret == '"') {
        if (!read_string()) {
            return TOKEN_STR;
        } else {
            return TOKEN_ERROR;
        }
    }
    if (is_number(*caret)) {
        char* old_caret = caret;
        int_data = read_int();
        if (*caret == '.') {
            caret++;
            while (is_number(*caret)) {
                caret++;
            }
            if (*caret == 'e' || *caret == 'E') {
                caret++;
                if (*caret == '-') {
                    caret++;
                }
                while (is_number(*caret)) {
                    caret++;
                }
            }
            string_buffer_length = caret - old_caret;
            memcpy(string_buffer, old_caret, string_buffer_length);
            string_buffer[string_buffer_length] = '\0';
            return TOKEN_FLOAT;
        }
        if (is_divider()) {
            return TOKEN_INT;
        } else {
            return TOKEN_ERROR;
        }
    }
    if (is_alpha(*caret)) {
        read_name();
        sigil = '\0';
        if (*caret == '$' || *caret == '#' || *caret == '!' || *caret == '%') {
            sigil = *caret;
            caret++;
        }
        if (!is_divider()) {
            return TOKEN_ERROR;
        }
        for (int i = 0; i < KEYWORD_COUNT; i++) {
            if (strcmp(string_buffer, keywords[i]) == 0) {
                return TOKEN_KEYWORD + i;
            }
        }
        return TOKEN_NAME;
    }
    int smltk;
    if ((smltk = is_small_token()) >= 0) {
        return smltk;
    }
    return TOKEN_ERROR;
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

bool working;

void process_input() {
    caret = input_buffer;
    char* shcaret = shrink_line;
    shrink_line_length = 0;
    bool process = true;
    while (process) {
        TokenType token = get_token();
        switch (token) {
            case TOKEN_ERROR:
                printf("Error");
                return;
            case TOKEN_EOF:
                process = false;
                break;
            case TOKEN_NAME:
            case TOKEN_STR:
            case TOKEN_FLOAT:
                *shcaret = token;
                shcaret++;
                if (token == TOKEN_NAME) {
                    *shcaret = sigil;
                    shcaret++;
                }
                *shcaret = string_buffer_length;
                shcaret++;
                memcpy(shcaret, string_buffer, string_buffer_length);
                shcaret += string_buffer_length;
                break;
            case TOKEN_INT:
                *shcaret = token;
                shcaret++;
                memcpy(shcaret, &int_data, 4);
                shcaret += 4;
                break;

            default:
                *shcaret = token;
                shcaret++;
                break;
        }
        skip_spaces();
    }
    shrink_line_length = shcaret - (char*)shrink_line;
}

void print_string(uint8_t** point) {
    int string_length = **point;
    (*point)++;
    printf("%.*s", string_length, (char*)(*point));
    (*point) += string_length;
}

void print_shrink_line() {
    uint8_t* point = shrink_line;
    while (true) {
        TokenType token = *point;
        point++;
        switch (token) {
            case TOKEN_NAME:
                char sigil = *point;
                point++;
                print_string(&point);
                if (sigil != '\0') {
                    printf("%c", sigil);
                }
                break;

            case TOKEN_INT:
                int32_t data;
                memcpy(&data, point, 4);
                printf("%d", data);
                point += 4;
                break;

            case TOKEN_STR:
                printf("\"");
                print_string(&point);
                printf("\"");
                break;

            case TOKEN_FLOAT:
                print_string(&point);
                break;

            default:
                if (token >= TOKEN_KEYWORD) {
                    printf("%s", keywords[token - TOKEN_KEYWORD]);
                } else if (token <= TOKEN_SMLEQ) {
                    printf("%s", small_tokens[token]);
                }
                break;
        }
        if (point - shrink_line < shrink_line_length) {
            printf(" ");
        } else {
            break;
        }
    }
}

int main() {
    printf("Hello world!\n");
    working = true;
    while (working) {
        get_input();
        // strcpy(input_buffer, "12 if total#<>3 then print \"Total is\";   total# else  msg$ = \"Get\"+5");
        process_input();
        print_shrink_line();
        printf("\n");
    }
    return 0;
}