#ifndef _EMULA_H_
#define _EMULA_H_

#include "instr.h"

void split(uint64_t instr, uint8_t *op, uint8_t *suf, uint8_t *ra);
instr_s* read_file(char *name);
uint64_t str_to_uint64(char *str);
void emula(instr_s *instr_sa);
void emula_sbs(instr_s *instr_sa);
void desa(instr_s *instr_sa);
char* desa_line(char *line, uint64_t ins_cur);


#endif
