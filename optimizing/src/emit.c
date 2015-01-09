#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "emit.h"

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


struct instruction *make_instruction(instruction_tag tag) {
    assert(tag != NOT_AN_INSTRUCTION);
    struct instruction *ins = calloc(1, sizeof *ins);
    ins->tag = tag;
    if (tag == BEG_LOOP) {
        ins->data.loop.loop = calloc(1, sizeof *(ins->data.loop.loop));
    }
    else {
        ins->data.repeats.num_times = 1;
    }
    return ins;
}

/* This does not take a reference because it is not publicly exposed. */
void free_loop(struct loop *loop) {
    struct instruction *next;
    struct instruction *ins = loop->instructions;
    while (ins) {
        next = ins->next;
        free_instruction(&ins);
        ins = next;
    }
    free(loop);
}
void free_instruction(struct instruction **instruction_ref) {
    struct instruction *ins;
    if (instruction_ref && *instruction_ref) {
        ins = *instruction_ref;
        if (ins->tag == BEG_LOOP) {
            free_loop(ins->data.loop.loop);
        }
        free(*instruction_ref);
        *instruction_ref = NULL;
    }
}

void emitter_init(FILE *out) {
    OUT = out;
}
void emitter_denit(void) {
    if (OUT) {
        fclose(OUT);
        OUT = NULL;
    }
}

void emit_header(void) {
    fputs_tabbed("#include <stdio.h>\n", OUT);
    fputs_tabbed("const int MEMSIZE = " MEMSIZE_sz ";\n", OUT);
    fputs_tabbed("int main() {\n", OUT);
    indent();
    fputs_tabbed("char " MEM_NAME "[MEMSIZE] = {0};\n", OUT);
    fputs_tabbed("char *" PTR_NAME " = mem;\n", OUT);
}
void emit_footer(void) {
    fputs_tabbed("return 0;\n", OUT);
    dedent();
    fputs_tabbed("}\n\n", OUT);
}
void emit_add(int num) {
    fprintf_tabbed(OUT, "*" PTR_NAME " += %d;\n", num);
}
void emit_sub(int num) {
    fprintf_tabbed(OUT, "*" PTR_NAME " -= %d;\n", num);
}
void emit_left(int num) {
    fprintf_tabbed(OUT, PTR_NAME " -= %d;\n", num);
}
void emit_right(int num) {
    fprintf_tabbed(OUT, PTR_NAME " += %d;\n", num);
}
void emit_input(void) {
    fputs_tabbed("*" PTR_NAME " = getchar();\n", OUT);
}
void emit_output(void) {
    fputs_tabbed("putchar(*" PTR_NAME ");\n", OUT);
}
void emit_loop(struct loop *loop) {
    fputs_tabbed("while (*" PTR_NAME ") {\n", OUT);
    indent();

    emit_instructions(loop->instructions);

    dedent();
    fputs_tabbed("}\n", OUT);
}

void emit_instructions(struct instruction *instructions) {
    struct instruction *ins = instructions;
    while (ins) {
        switch (ins->tag) {
            case LEFT: emit_left(ins->data.repeats.num_times); break;
            case RIGHT: emit_right(ins->data.repeats.num_times); break;
            case ADD: emit_add(ins->data.repeats.num_times); break;
            case SUB: emit_sub(ins->data.repeats.num_times); break;
            case INPUT: emit_input(); break;
            case OUTPUT: emit_output(); break;
            case BEG_LOOP: emit_loop(ins->data.loop.loop);
            case END_LOOP:
            case NOT_AN_INSTRUCTION: break;
        }
        ins = ins->next;
    }
}

