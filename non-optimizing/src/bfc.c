#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMSIZE_sz "30000"
#define PTR_NAME "p"
#define MEM_NAME "mem"

#define fputs_tabbed(str, file) do { \
    char *__secret_scoped_string__ = tabify(str); \
    fputs(__secret_scoped_string__, file); \
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
void left(void) {
    fputs_tabbed("--" PTR_NAME ";\n", OUT);
}
void right(void) {
    fputs_tabbed("++" PTR_NAME ";\n", OUT);
}
void add(void) {
    fputs_tabbed("++*" PTR_NAME ";\n", OUT);
}
void sub(void) {
    fputs_tabbed("--*" PTR_NAME ";\n", OUT);
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
void header(void) {
    fputs_tabbed("#include <stdio.h>\n", OUT);
    fputs_tabbed("unsigned char " MEM_NAME "[" MEMSIZE_sz "];\n", OUT);
    fputs_tabbed("int main(void) {\n", OUT);
    indent();
    fputs_tabbed("unsigned char *" PTR_NAME " = " MEM_NAME ";\n", OUT);
}
void footer(void) {
    fputs_tabbed("return 0;\n", OUT);
    dedent();
    fputs_tabbed("}\n\n", OUT);
}
void parse() {
    int c;
    header();
    while ((c = getc(IN)) != EOF) {
        switch (c) {
            case '>': right(); break;
            case '<': left(); break;
            case '+': add(); break;
            case '-': sub(); break;
            case '.': output(); break;
            case ',': input(); break;
            case '[': open_bracket(); break;
            case ']': close_bracket(); break;
            default: break;
        }
    }
    fclose(IN);
    IN = NULL;
    footer();
}
int main(void) {
    IN = stdin;
    OUT = stdout;
    parse();
    return 0;
}

