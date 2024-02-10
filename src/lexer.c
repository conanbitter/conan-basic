#include "lexer.h"
#include <stdbool.h>

static const char* const small_tokens[] = {
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

static const char* const keywords[KEYWORD_COUNT] = {
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

static char string_buffer[STRING_BUFFER_SIZE];
static int string_buffer_length;
static uint8_t shrinked_line[SHRINK_LINE_SIZE];
static int shrinked_line_length;
static int32_t int_data;
static char sigil;

static const char* caret;

static bool is_number(char symbol) {
    return symbol >= '0' && symbol <= '9';
}

static bool is_space(char symbol) {
    return symbol == ' ' || symbol == '\t';
}

static bool is_alpha(char symbol) {
    return (symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z');
}

static int is_small_token() {
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

static int read_int() {
    int result = 0;
    while (*caret != '\0' && is_number(*caret)) {
        result = result * 10 + *caret - '0';
        caret++;
    }
    return result;
}

static bool is_divider() {
    const char* old_caret = caret;
    if (*caret == '\0' || is_space(*caret) || is_small_token() >= 0) {
        caret = old_caret;
        return true;
    }
    return false;
}

static void read_name() {
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

static bool read_string() {
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

static void skip_spaces() {
    while (*caret != '\0' && is_space(*caret)) {
        caret++;
    }
}

static TokenType get_token() {
    if (*caret == '\0') {
        return TOKEN_EOL;
    }
    if (*caret == '"') {
        if (!read_string()) {
            return TOKEN_STR;
        } else {
            return TOKEN_ERROR;
        }
    }
    if (is_number(*caret)) {
        const char* old_caret = caret;
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

uint8_t* shrink_line(const char* input, size_t* length) {
    caret = input;
    char* shcaret = shrinked_line;
    bool process = true;
    while (process) {
        TokenType token = get_token();
        switch (token) {
            case TOKEN_ERROR:
                shrinked_line_length = 0;
                shrinked_line[0] = TOKEN_ERROR;
                printf("Error");
                if (length != NULL) {
                    *length = 0;
                }
                return shrinked_line;
            case TOKEN_EOL:
                *shcaret = token;
                shcaret++;
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
    shrinked_line_length = shcaret - (char*)shrinked_line;
    if (length != NULL) {
        *length = shrinked_line_length;
    }
    return shrinked_line;
}

static void print_string(uint8_t** point) {
    int string_length = **point;
    (*point)++;
    printf("%.*s", string_length, (char*)(*point));
    (*point) += string_length;
}

void print_shrinked_line(const uint8_t* line) {
    const uint8_t* point = line;
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

            case TOKEN_EOL:
                return;

            default:
                if (token >= TOKEN_KEYWORD) {
                    printf("%s", keywords[token - TOKEN_KEYWORD]);
                } else if (token <= TOKEN_SMLEQ) {
                    printf("%s", small_tokens[token]);
                }
                break;
        }
        printf(" ");
    }
}

const uint8_t* get_int(const uint8_t* line, uint32_t* value) {
    if (*line == TOKEN_INT) {
        memcpy(value, line + 1, 4);
        return line + 1 + 4;
    } else {
        return NULL;
    }
}