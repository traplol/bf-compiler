#include "pass1.h"


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


void pass1(struct instruction *instructions) {
    fold(instructions);
}

