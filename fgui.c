/**
 * \file fgui.c
 * \brief Ce fichier contient les fonctions utilisées lors de
 * l'utilisation du mode graphique avec ncurses
 */

#include "fgui.h"

#include "gui.h"
#include "emula.h"
#include "util.h"
#include "instr.h"

#include <ncurses.h>

/**
 * \fn fp_instr*	f_init_fgui(void)
 * \brief Initialise le tableau d'instruction utilisé dans le mode
 * graphique
 *
 * \return le talbeau d'instruction initialisé
 *
 * Cette fonction permet de supprimer les entrées print et read du
 * tableau habituellement utilisé car on en utilisera d'autres.
 */
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

/**
 * \fn void	f_print_fgui(float *reg, uint8_t *ra, instr_s *instr_sa, window *w)
 * \brief Remplace la fonction f_print et affiche le registre indiqué
 * dans la fenêtre prévue.
 *
 * \param reg un talbeau contenant les 16 registres du processeur Femto
 * \param ra un talbeau indiquant les registres à utilisé pour l'opération
 * \param instr_sa le contexte courant
 * \param w la fenêtre où afficher
 */
void	f_print_fgui(float *reg, uint8_t *ra, instr_s *instr_sa, window *w) {
  clear_line_from_to(w, 1, get_window_width(w)-2, 1, 0);
  wattron(w, A_BOLD);
  printfXY(w, 1, 1, 0, "%f", reg[ra[0]]);
  wattroff(w, A_BOLD);
  redraw_window(w);
  redraw_window(w);

  INC_IP(instr_sa);
}

/**
 * \fn void	f_print_fgui(float *reg, uint8_t *ra, instr_s *instr_sa, window *w)
 * \brief Remplace la fonction f_read gèrant la saisie utilisateur
 *
 * \param reg un talbeau contenant les 16 registres du processeur Femto
 * \param ra un talbeau indiquant les registres à utilisé pour l'opération
 * \param instr_sa le contexte courant
 * \param w la fenêtre où se passe la saisie
 *
 * On affiche un message dans une fenêtre demandant à l'utilisateur
 * de rentrer un nombre.
 */
void	f_read_fgui(float *reg, uint8_t *ra, instr_s *instr_sa, window *w) {
  clear_line_from_to(w, 1, get_window_width(w)-2, 1, 0);
  wattron(w, A_BOLD);
  printfXY(w, 1, 1, 0, "Entrez un nombre: ");
  wattroff(w, A_BOLD);
  redraw_window(w);
  echo();
  wscanw(w, "%f", &(reg[ra[0]]));
  printfXY(w, 1, 1, 0, "Entrez un nombre: ");
  noecho();
  redraw_window(w);

  INC_IP(instr_sa);
}

/**
 * \fn void fgui_print_reg(float *reg, window *w)
 * \brief Affiche le contenu des 16 registres du processeur Femto dans
 * la fenêtre indiquée
 *
 * \param reg le tableau correspondant aux 16 registres du processeur
 * Femto
 * \param w la fenêtre où afficher
 */
void fgui_print_reg(float *reg, window *w) {
  size_t i;

  for (i = 0; i < 16; ++i)
    clear_line_from_to(w, 2, get_window_width(w)-2, 2+i, 0);
  for (i = 0; i < 16; ++i) {
    printfXY(w, 2, 2+i, 0, "r%d: %f", i, reg[i]);
  }

}

/**
 * \fn void fgui_print_prog(instr_s *instr_sa, window *w)
 * \brief Affiche le programme courant désassemblé dans la fenêtre
 * indiquée
 *
 * \param instr_sa le contexte courant
 * \param w la fenêtre où afficher
 *
 * Cette fonction affiche le programme courant désassemblé en gérant
 * le cas où la fenêtre ne serait pas assez grande pour afficher le code
 * en entier.
 * De plus, une flèche indique la ligne allant être exécutée. La ligne
 * est également affichée en gras.
 */
void fgui_print_prog(instr_s *instr_sa, window *w) {
  char *line;
  size_t size, i, start, end;

  if (instr_sa->nb <= get_window_height(w) - 2) {
    size  = instr_sa->nb;
    start = 0;
    end	  = size;
  } else {
    size  = get_window_height(w) - 4;
    end = MIN(instr_sa->ip + size, instr_sa->nb);
    start = end - size;
  }

  line = (char*) xmalloc(sizeof(char) * 255);

  for (i = 0; i < (end-start); ++i)
    clear_line_from_to(w, 2, get_window_width(w)-2, 2+i, 0);

  for (i = 0; i < (end-start); ++i) {
    line = desa_line(line, instr_sa->ins[i+start]);
    if (i+start == instr_sa->ip) {
      wattron(w, A_BOLD);
      printfXY(w, 2, i+2, 0, "-> ");
    } else {
      wattroff(w, A_BOLD);
      printfXY(w, 2, i+2, 0, "++ ", i);
    }
    printfXY(w, 5, i+2, 0, line);
  }

  free(line);
  line = NULL;
}

/**
 * \fn
 * void fgui_init(window **w_prog, window **w_reg, window **w_out, window **w_in)
 * \brief Initialise les différentes fenêtres gérées durant l'exécution
 * du programme ainsi que d'autres paramètres de l'interface ncurses.
 *
 * \param w_prog fenêtre dans laquelle le code désassemblé sera affiché
 * \param w_reg fenêtre dans laquelle les registres seront affichés
 * \param w_out fenêtre servant à l'affichage des opérations de sorties
 * du programme
 * \param w_in fenêtre servant à l'affichage des opérations d'entrées
 * du programme (les saisies utilisateurs)
 */
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

/**
 * \fn void fgui_end(window **w_prog, window **w_reg, window **w_out, window **w_in)
 * \brief Détruit les différentes fenêtres gérées durant l'exécution
 * du programme et restaure le terminal dans son état initial.
 *
 * \param w_prog fenêtre dans laquelle le code désassemblé sera affiché
 * \param w_reg fenêtre dans laquelle les registres seront affichés
 * \param w_out fenêtre servant à l'affichage des opérations de sorties
 * du programme
 * \param w_in fenêtre servant à l'affichage des opérations d'entrées
 * du programme (les saisies utilisateurs)
 */
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

/**
 * \fn void fgui_emula(instr_s *instr_sa)
 * \brief Fonction principale réalisant l'exécution du programme indiqué
 *
 * \param instr_sa le contexte courant
 *
 * Cette fonction exécute le exécute le code en mode pas-à-pas et
 * affiche le contenu des registres après chaque instruction
 */
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

    /* Affichage des registres */
    fgui_print_reg(reg, w_reg);
    redraw_window(w_reg);

    getchar();

    /*  Decomposition de l'instruction  */
    split(ins_cur, &op, &suf, ra);

    /*  On execute l'instruction demandee si necessaire  */
    if ((suf & instr_sa->flags) | !suf) {
      switch (op) {
      case 7:
	f_print_fgui(reg, ra, instr_sa, w_out);
	break;
      case 8:
	f_read_fgui(reg, ra, instr_sa, w_in);
	break;
      default:
	fp_instr_a[op](reg, ra, instr_sa);
	break;
      }
    /* sinon on passe a l'instruction suivante */
    } else
      INC_IP(instr_sa);
  }

  /* Attente de l'appuie d'une touche avant de terminer le programme */
  getchar();

  free(fp_instr_a);
  fp_instr_a = NULL;

  fgui_end(&w_prog, &w_reg, &w_out, &w_in);
}
