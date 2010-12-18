#ifndef _FGUI_INSTR_H_
#define _FGUI_INSTR_H_

#include "instr.h"
#include "gui.h"
#include <ncurses.h>

fp_instr*	f_init_fgui(void);

void	f_print_fgui(float *reg, uint8_t *ra,
		     instr_s *instr_sa, int x, int y,
		     window *w_out);

void	f_read_fgui(float *reg, uint8_t *ra,
		    instr_s *instr_sa, int x, int y,
		    window *w_in);

#endif
