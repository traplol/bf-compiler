#ifndef _BF_COMPILER_OPTIMIZING_INC_PASS_1_H
#define _BF_COMPILER_OPTIMIZING_INC_PASS_1_H

/********************************************************************/
/* Pass 1:                                                          */
/*  This pass handles constant folding for the '<', '>', '+', and   */
/*  '-'.                                                            */
/*                                                                  */
/*  '>>>' -> 'p += 3'                                               */
/*  '++++--' -> '*p += 2'                                           */
/*                                                                  */
/*  TODO: Things like '>><<' and '++--' are removed because they    */
/*          are no-ops.                                             */
/********************************************************************/

#include "emit.h"

void pass1(struct instruction *instructions);

#endif

