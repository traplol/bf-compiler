#include <stdlib.h>

#include "emit.h"

FILE *IN;

int peek(FILE *file) {
    int c = getc(file);
    ungetc(c, file);
    return c;
}

instruction_tag get_tag(int c) {
    instruction_tag tag;
    switch (c) {
        case '<': tag = LEFT; break;
        case '>': tag = RIGHT; break;
        case '+': tag = ADD; break;
        case '-': tag = SUB; break;
        case '.': tag = OUTPUT; break;
        case ',': tag = INPUT; break;
        case '[': tag = BEG_LOOP; break;
        case ']': tag = END_LOOP; break;
        default: tag = NOT_AN_INSTRUCTION; break;
    }
    return tag;
}

void build_loop(struct loop *loop) {
    int c;
    struct instruction *tail = NULL;
    instruction_tag tag;

    while ((c = peek(IN)) != EOF) {
        tag = get_tag(c);
        if (tag == NOT_AN_INSTRUCTION) {
            getc(IN);
            continue;
        }
        if (loop->instructions == NULL) {
            loop->instructions = make_instruction(tag);
            tail = loop->instructions;
        }
        else {
            tail->next = make_instruction(tag);
            tail = tail->next;
        }
        /* Progress the input stream. */
        getc(IN);
        if (tag == BEG_LOOP) {
             build_loop(tail->data.loop.loop);
        }
        else if (tag == END_LOOP) {
            return;
        }
    }
}

struct instruction *parse(void) {
    int c;
    struct instruction *head = NULL;
    struct instruction *tail = NULL;
    instruction_tag tag;

    while ((c = peek(IN)) != EOF) {
        tag = get_tag(c);
        if (tag == NOT_AN_INSTRUCTION) {
            getc(IN);
            continue;
        }
        if (head == NULL) {
            head = make_instruction(tag);
            tail = head;
        }
        else {
            tail->next = make_instruction(tag);
            tail = tail->next;
        }
        /* Progress the input stream. */
        getc(IN);
        /* If it's a loop we need to transfer control to the loop builder. */
        if (tag == BEG_LOOP) {
             build_loop(tail->data.loop.loop);
        }
    }
    return head;
}

void fold_add_sub(struct instruction *instructions) {
    struct instruction *head;
    struct instruction *cur;
    struct instruction *next;
    struct instruction *last;
    int i = 0;
    cur = head = instructions;

    /* Count the number of ADD and SUBs */
    while (cur) {
        if (cur->tag != ADD && cur->tag != SUB) { break; }

        if (cur->tag == ADD) {
            ++i;
        }
        else if (cur->tag == SUB) {
            --i;
        }
        cur = cur->next;
    }
    last = cur;
    /* Change operation to ADD and fold number of adds to do. */
    head->tag = ADD;
    head->data.repeats.num_times = i;

    /* Unlink and free the folded instructions. */
    if (head != last) {
        cur = head->next;
        head->next = last;
        while (cur && cur != last) {
            next = cur->next;
            free_instruction(&cur);
            cur = next;
        }
    }
}
void fold_left_right(struct instruction *instructions) {
    struct instruction *head;
    struct instruction *cur;
    struct instruction *next;
    struct instruction *last;
    int i = 0;
    cur = head = instructions;

    /* Count the number of LEFT and RIGHT */
    while (cur) {
        if (cur->tag != RIGHT && cur->tag != LEFT) { break; }

        if (cur->tag == RIGHT) {
            ++i;
        }
        else if (cur->tag == LEFT) {
            --i;
        }
        cur = cur->next;
    }
    last = cur;
    /* Change operation to RIGHT and fold number of rights to do. */
    head->tag = RIGHT;
    head->data.repeats.num_times = i;

    /* Unlink and free the folded instructions. */
    if (head != last) {
        cur = head->next;
        head->next = last;
        while (cur && cur != last) {
            next = cur->next;
            free_instruction(&cur);
            cur = next;
        }
    }
}

void fold(struct instruction *instructions) {
    while (instructions) {
        switch (instructions->tag) {
            case ADD:
            case SUB:
                fold_add_sub(instructions); break;
            case LEFT:
            case RIGHT:
                fold_left_right(instructions); break;
            case BEG_LOOP:
                fold(instructions->data.loop.loop->instructions); break;
            case END_LOOP:
                return;
            default: break;
        }
        instructions = instructions->next;
    }
}

int main(void) {
    struct instruction *instructions;
    struct instruction *next;
    IN = stdin;
    instructions = parse();
    fold(instructions);

    emitter_init(stdout);
    emit_header();
    emit_instructions(instructions);
    emit_footer();
    emitter_denit();

    while (instructions) {
        next = instructions->next;
        free_instruction(&instructions);
        instructions = next;
    }

    return 0;
}

