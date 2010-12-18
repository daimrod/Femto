#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ncurses.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include "gui.h"

void cleanup_gui_shadow(int out)
{
  endwin();
  exit(out);
}

void init_gui(void)
{
  signal(SIGINT, cleanup_gui_shadow);

  srand(getpid());
  
  initscr();
  raw();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  nodelay(stdscr,TRUE); // Lecture non bloquante d'une touche au clavier
  
  if (!has_colors()) {
    fprintf(stderr,"Terminal incapable d'afficher des couleurs\n");
    cleanup_gui();
    exit(1);
  }
  start_color();
  init_pair(1,COLOR_RED,COLOR_WHITE);
  init_pair(2,COLOR_GREEN,COLOR_WHITE);
  init_pair(3,COLOR_YELLOW,COLOR_WHITE);
  init_pair(4,COLOR_BLUE,COLOR_WHITE);
  init_pair(5,COLOR_MAGENTA,COLOR_WHITE);
  init_pair(6,COLOR_CYAN,COLOR_WHITE);
  init_pair(7,COLOR_WHITE,COLOR_WHITE);
}

void cleanup_gui(void)
{
  endwin();
}


void get_term_size(int *largeur, int *hauteur)
{
  getmaxyx(stdscr,*hauteur,*largeur);
}

int get_term_height(void)
{
  int largeur, hauteur;
  getmaxyx(stdscr,hauteur,largeur);
  return hauteur;
}

int get_term_width(void)
{
  int hauteur, largeur;
  getmaxyx(stdscr,hauteur,largeur);
  return largeur;
}

void get_window_size(window *w, int *largeur, int *hauteur)
{
  getmaxyx(w,*hauteur,*largeur);
}

int get_window_height(window *w)
{
  int largeur, hauteur;
  getmaxyx(w,hauteur,largeur);
  return hauteur;
}

int get_window_width(window *w)
{
  int hauteur, largeur;
  getmaxyx(w,hauteur,largeur);
  return largeur;
}

window *create_window(int largeur, int hauteur, int posX, int posY, bool cadre)
{
  window *w = newwin(hauteur,largeur,posY,posX);
  if (cadre) {
    box(w,0,0);
  }
  wrefresh(w);
  return w;
}

void delete_window(window *w)
{
  delwin(w);
}

void redraw_window(window *w)
{
  wrefresh(w);
}

void putcharXY(window *w, int posX, int posY, char c, int col)
{
  wattron(w,COLOR_PAIR(col));
  mvwaddch(w,posY,posX,c);
  wattroff(w,COLOR_PAIR(col));
}

int printfXY(window *w, int posX, int posY, int col, const char *patron, ...)
{
  va_list ap;
  int res;
  
  wattron(w,COLOR_PAIR(col));
  
  va_start(ap,patron);
  wmove(w,posY,posX);
  res = vwprintw(w,patron,ap);
  va_end(ap);

  wattroff(w,COLOR_PAIR(col));
  return res;
}

char pollchar(window *w)
{
  nodelay(w,TRUE);
  return wgetch(w);
}
