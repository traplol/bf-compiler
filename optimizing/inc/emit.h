#ifndef _BF_COMPILER_OPTIMIZING_INC_EMIT_H
#define _BF_COMPILER_OPTIMIZING_INC_EMIT_H

#include <stdio.h>

typedef enum {
    NOT_AN_INSTRUCTION,
    LEFT,      RIGHT,
    ADD,       SUB,
    INPUT,     OUTPUT,
    BEG_LOOP,  END_LOOP
} instruction_tag;

struct instruction {
    instruction_tag tag;
    union {
        struct { int num_times; } repeats;
        struct { struct loop *loop; } loop;
    } data;
    struct instruction *next;
};

struct loop {
    struct instruction *instructions;
};

struct instruction *make_instruction(instruction_tag tag);
void free_instruction(struct instruction **instruction_ref);

void emitter_init(FILE *out);
void emitter_denit(void);

void emit_header(void);
void emit_footer(void);
void emit_add(int num);
void emit_sub(int num);
void emit_left(int num);
void emit_right(int num);
void emit_input(void);
void emit_output(void);
void emit_loop(struct loop *loop);
void emit_instructions(struct instruction *instructions);

#endif

