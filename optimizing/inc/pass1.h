#ifndef _BF_COMPILER_OPTIMIZING_INC_PASS_1_H
#define _BF_COMPILER_OPTIMIZING_INC_PASS_1_H

/********************************************/
/* Pass 1:                                  */
/*  This pass handles constant folding for  */
/*  the '<', '>', '+', and '-' instructions */
/********************************************/

#include "emit.h"

void pass1(struct instruction *instructions);

#endif

