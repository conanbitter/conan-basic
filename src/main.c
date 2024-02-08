#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define INPUT_BUFFER_SIZE 256
#define STRING_BUFFER_SIZE 256
#define KEYWORD_COUNT 49

char input_buffer[INPUT_BUFFER_SIZE];
char string_buffer[STRING_BUFFER_SIZE];
int string_buffer_length;

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
    TOKEN_KEYWORD,
    TOKEN_INT,
    TOKEN_STR,
    TOKEN_FLOAT,
    TOKEN_DOUBLE,
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

bool get_token() {
    if (*caret == '\0') {
        printf(" [EOF]");
        return true;
    }
    if (*caret == '"') {
        if (!read_string()) {
            printf(" [STR: \"%s\" (%d)]", string_buffer, string_buffer_length);
            return false;
        } else {
            printf(" [Error]");
            return true;
        }
    }
    if (is_number(*caret)) {
        int data = read_int();
        if (is_divider()) {
            printf(" [INT: %d]", data);
            return false;
        } else {
            printf(" [Error]");
            return true;
        }
    }
    // TODO float numbers
    if (is_alpha(*caret)) {
        read_name();
        char sigil = '\0';
        if (*caret == '$' || *caret == '#' || *caret == '!' || *caret == '%') {
            sigil = *caret;
            caret++;
        }
        if (!is_divider()) {
            printf(" [Error]");
            return true;
        }
        for (int i = 0; i < KEYWORD_COUNT; i++) {
            if (strcmp(string_buffer, keywords[i]) == 0) {
                printf(" [KEYWORD: \"%s\"]", keywords[i]);
                return false;
            }
        }
        if (sigil != '\0') {
            printf(" [NAME: \"%s\" %c (%d)]", string_buffer, sigil, string_buffer_length);
        } else {
            printf(" [NAME: \"%s\" (%d)]", string_buffer, string_buffer_length);
        }
        return false;
    }
    int smltk;
    if ((smltk = is_small_token()) >= 0) {
        printf(" [ %s ]", small_tokens[smltk]);
        return false;
    }
    printf(" [Error]");
    return true;
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
    while (true) {
        if (get_token()) {
            break;
        }
        skip_spaces();
    }
    printf("\n");
}

int main() {
    printf("Hello world!\n");
    working = true;
    while (working) {
        get_input();
        process_input();
    }
    return 0;
}