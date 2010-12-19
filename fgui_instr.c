#include "fgui_instr.h"

fp_instr*	f_init_fgui(void) {
  fp_instr *fp_instr_a;

  fp_instr_a = (fp_instr*) xmalloc(sizeof(fp_instr_a) * 11);
  fp_instr_a[0] = &f_add;
  fp_instr_a[1] = &f_b;
  fp_instr_a[2] = &f_cmp;
  fp_instr_a[3] = &f_load;
  fp_instr_a[4] = &f_move;
  fp_instr_a[5] = &f_mul;
  fp_instr_a[6] = &f_pow;
  fp_instr_a[7] = NULL;
  fp_instr_a[8] = NULL;
  fp_instr_a[9] = &f_stop;
  fp_instr_a[10] = &f_sub;
  return fp_instr_a;
}

void	f_print_fgui(float *reg, uint8_t *ra,
		     instr_s *instr_sa, int x, int y,
		     window *w_out) {
  wattron(w_out, A_BOLD);
  printfXY(w_out, x, y, 0, "%f", reg[ra[0]]);
  wattroff(w_out, A_BOLD);
  redraw_window(w_out);

  INC_IP(instr_sa);
}

void	f_read_fgui(float *reg, uint8_t *ra,
		    instr_s *instr_sa, int x, int y,
		    window *w_in) {
  clear_line_from_to(w_in, x, get_window_width(w_in)-2, y, 0);
  wattron(w_in, A_BOLD);
  printfXY(w_in, x, y, 0, "Entrez un nombre: ");
  echo();
  wscanw(w_in, "%f", &(reg[ra[0]]));
  noecho();
  wattroff(w_in, A_BOLD);
  redraw_window(w_in);

  INC_IP(instr_sa);
}
