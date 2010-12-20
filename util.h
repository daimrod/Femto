#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdlib.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define FREE_INS(instr)\
  free(instr->ins);				\
  instr->ins = NULL;				\
  free(instr);					\
  instr = NULL 

void* xmalloc(size_t size);

#endif
