#include "fgui.h"

#include "gui.h"
#include "emula.h"
#include "util.h"
#include "instr.h"

#include <ncurses.h>

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
		     instr_s *instr_sa, int x, int y, window *w_out) {
  clear_line_from_to(w_out, x, get_window_width(w_out)-2, y, 0);
  wattron(w_out, A_BOLD);
  printfXY(w_out, x, y, 0, "%f", reg[ra[0]]);
  wattroff(w_out, A_BOLD);
  redraw_window(w_out);
  redraw_window(w_out);

  INC_IP(instr_sa);
}

void	f_read_fgui(float *reg, uint8_t *ra,
		    instr_s *instr_sa, int x, int y,
		    window *w_in) {
  clear_line_from_to(w_in, x, get_window_width(w_in)-2, y, 0);
  wattron(w_in, A_BOLD);
  printfXY(w_in, x, y, 0, "Entrez un nombre: ");
  wattroff(w_in, A_BOLD);
  redraw_window(w_in);
  echo();
  wscanw(w_in, "%f", &(reg[ra[0]]));
  printfXY(w_in, x, y, 0, "Entrez un nombre: ");
  noecho();
  redraw_window(w_in);

  INC_IP(instr_sa);
}

void fgui_print_reg(float *reg, window *w_reg) {
  size_t i;

  for (i = 0; i < 16; ++i)
    clear_line_from_to(w_reg, 2, get_window_width(w_reg)-2, 2+i, 0);
  for (i = 0; i < 16; ++i) {
    printfXY(w_reg, 2, 2+i, 0, "r%d: %f", i, reg[i]);
  }

}

void fgui_print_prog(instr_s *instr_sa, window *w_prog) {
  char *line;
  size_t size, i, start, end;

  if (instr_sa->nb <= get_window_height(w_prog) - 2) {
    size  = instr_sa->nb;
    start = 0;
    end	  = size;
  } else {
    size  = get_window_height(w_prog) - 4;
    end = MIN(instr_sa->ip + size, instr_sa->nb);
    start = end - size;
  }

  line = (char*) xmalloc(sizeof(char) * 255);

  for (i = 0; i < (end-start); ++i)
    clear_line_from_to(w_prog, 2, get_window_width(w_prog)-2, 2+i, 0);

  for (i = 0; i < (end-start); ++i) {
    line = desa_line(line, instr_sa->ins[i+start]);
    if (i+start == instr_sa->ip) {
      wattron(w_prog, A_BOLD);
      printfXY(w_prog, 2, i+2, 0, "-> ");
    } else {
      wattroff(w_prog, A_BOLD);
      printfXY(w_prog, 2, i+2, 0, "++ ", i);
    }
    printfXY(w_prog, 5, i+2, 0, line);
  }

  free(line);
  line = NULL;
}

void fgui_init(window **w_prog,
	       window **w_reg,
	       window **w_out,
	       window **w_in) {
  init_gui();
  curs_set(0);
  *w_prog = create_window(get_term_width() / 3 * 2,
			 get_term_height() / 4 * 3,
			 0, 0, 1);
  *w_reg = create_window(get_term_width() / 3,
			get_term_height() / 4 * 3,
			get_term_width() / 3 * 2, 0, 1);
  *w_out = create_window(get_term_width() / 2,
		       get_term_height() / 4,
		       0, get_term_height() / 4 * 3, 1);
  *w_in = create_window(get_term_width() / 2,
			get_term_height() / 4,
			get_term_width() / 2,
			get_term_height() / 4 * 3, 1);
}

void fgui_end(window **w_prog,
	      window **w_reg,
	      window **w_out,
	      window **w_in) {
  delete_window(*w_prog);
  delete_window(*w_reg);
  delete_window(*w_out);
  delete_window(*w_in);
  cleanup_gui();
}

void fgui_emula(instr_s *instr_sa) {
  window *w_prog, *w_reg, *w_out, *w_in;
  fp_instr *fp_instr_a;
  uint64_t ins_cur;
  uint8_t op, suf, ra[3];
  float reg[16];
  size_t i;

  fgui_init(&w_prog, &w_reg, &w_out, &w_in);

  fp_instr_a = f_init_fgui();
  for (i = 0; i < 16; ++i)
    reg[i] = 0;

  while (instr_sa->ip < instr_sa->nb) {
    /*  Recuperation de l'instruction courante  */
    ins_cur = instr_sa->ins[instr_sa->ip];

    /*  Affichage de l'instruction desassemblee */
    fgui_print_prog(instr_sa, w_prog);
    redraw_window(w_prog);

    fgui_print_reg(reg, w_reg);
    redraw_window(w_reg);

    getchar();

    /*  Decomposition de l'instruction  */
    split(ins_cur, &op, &suf, ra);

    /*  On execute l'instruction demandee si necessaire  */
    if ((suf & instr_sa->flags) | !suf) {
      switch (op) {
      case 7:
	f_print_fgui(reg, ra, instr_sa, 1, 1, w_out);
	break;
      case 8:
	f_read_fgui(reg, ra, instr_sa, 1, 1, w_in);
	break;
      default:
	fp_instr_a[op](reg, ra, instr_sa);
	break;
      }
    /* sinon on passe a l'instruction suivante */
    } else
      INC_IP(instr_sa);
  }

  getchar();

  free(fp_instr_a);
  fp_instr_a = NULL;

  fgui_end(&w_prog, &w_reg, &w_out, &w_in);
}
