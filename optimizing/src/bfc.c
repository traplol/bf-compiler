#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MEMSIZE_sz "30000"
#define PTR_NAME "p"
#define MEM_NAME "mem"

#define fputs_tabbed(str, file) do { \
    char *__secret_scoped_string__ = tabify(str); \
    fputs(__secret_scoped_string__, file); \
    free(__secret_scoped_string__); \
} while(0)

#define fprintf_tabbed(file, fmt, ...) do { \
    char *__secret_scoped_string__ = tabify(fmt); \
    fprintf(file, __secret_scoped_string__, __VA_ARGS__); \
    free(__secret_scoped_string__); \
} while(0)

FILE *OUT;
FILE *IN;

int indention = 0;

void indent(void) {
    ++indention;
}
void dedent(void) {
    --indention;
}

char *tab(int num_tabs, char *str) {
    int len = 1 + num_tabs + strlen(str);
    char *s = calloc(1, len);
    memset(s, '\t', num_tabs);
    memcpy(s+num_tabs, str, len-num_tabs);
    return s;
}

char *tabify(char *str) {
    return tab(indention, str);
}

int peek(FILE *file) {
    int c = getc(file);
    ungetc(c, file);
    return c;
}

void ch_ptr(int num) {
    fprintf_tabbed(OUT, PTR_NAME " += %d;\n", num);
}
void ch_cell(int num) {
    fprintf_tabbed(OUT, "*" PTR_NAME " += %d;\n", num);
}
void output(void) {
    fputs_tabbed("putchar(*" PTR_NAME ");\n", OUT);
}
void input(void) {
    fputs_tabbed("*" PTR_NAME " = getchar();\n", OUT);
}
void open_bracket(void) {
    fputs_tabbed("while (*" PTR_NAME ") {\n", OUT);
    indent();
}
void close_bracket(void) {
    dedent();
    fputs_tabbed("}\n", OUT);
}


void init(void) {
    fputs_tabbed("#include <stdio.h>\n", OUT);
    fputs_tabbed("const int MEMSIZE = " MEMSIZE_sz ";\n", OUT);
    fputs_tabbed("int main() {\n", OUT);
    indent();
    fputs_tabbed("char " MEM_NAME "[MEMSIZE] = {0};\n", OUT);
    fputs_tabbed("char *" PTR_NAME " = mem;\n", OUT);
}

void denit(void) {
    fputs_tabbed("return 0;\n", OUT);
    dedent();
    fputs_tabbed("}\n", OUT);
}

void optimize_left_right(void) {
    int c, i = 0;
    while ((c = peek(IN)) != EOF) {
        if (c != '<' && c != '>') { break; }
        else if (c == '<') { --i; }
        else if (c == '>') { ++i; }
        getc(IN);
    }
    ch_ptr(i);
}
void optimize_add_sub(void) {
    int c, i = 0;
    while ((c = peek(IN)) != EOF) {
        if (c != '+' && c != '-') { break; }
        else if (c == '-') { --i; }
        else if (c == '+') { ++i; }
        getc(IN);
    }
    ch_cell(i);
}

void parse(char *filename) {
    int c;

    IN = fopen(filename, "r");
    if (!IN) {
        fprintf(stderr, "Could not open file \"%s\"\n", filename);
        exit(1);
    }
    init();
    while ((c = peek(IN)) != EOF) {
        if (c == '>' || c == '<') {
            optimize_left_right();
            continue;
        }
        else if (c == '+' || c == '-') {
            optimize_add_sub();
            continue;
        }
        else if (c == '.') {
            output();
        }
        else if (c == ',') {
            input();
        }
        else if (c == '[') {
            open_bracket();
        }
        else if (c == ']') {
            close_bracket();
        }
        /* We use getc here to advance the stream, because + - < > handle
         * the stream in their own way but , . [ ] do not.
         */
        getc(IN);
    }
    fclose(IN);
    IN = NULL;
    denit();
}

int main(int argc, char **argv) {
    --argc; ++argv;
    if (!argc) {
        fputs("No input file.\n", stderr);
        return 1;
    }
    OUT = stdout;
    parse(*argv);
    return 0;
}

