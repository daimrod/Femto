/*
  gui.h --

  Structures de données et méthodes de manipulation de l'interface 
  utilisateurs.

  La couche "bas niveau" utilise ncurses.

  Coordonnées:
  ============
  On utilise le système de coordonnées "standard" avec l'origine (0,0) 
  en haut à gauche:
               
  +------------> x
  |
  |
  |
  |
  v 
  y

  Couleurs:
  =========
  On a défini 8 couleurs, identifiées par leur numéro:
  0 : blanc sur noir
  1 : rouge sur blanc
  2 : vert sur blanc
  3 : jaune sur blanc
  4 : bleu sur blanc
  5 : magenta sur blanc
  6 : cyan sur blanc
  7 : blanc sur blanc
*/
#ifndef __gui_h__
#define __gui_h__

#include <stdbool.h>

/*  Les couleurs disponibles  */
typedef enum {
  noir,
  rouge,
  vert,
  jaune,
  bleu,
  magenta,
  cyan,
  blanc
} couleur;

struct _win_st;
/*
  Type contenant toutes les informations relatives à une fenêtre de 
  l'interface.
  Ce type est transparent et inaccessible à l'utilisateur. 
  Une fenêtre se manipule donc uniquement à partir d'un pointeur 
  'window*' et de méthodes dédiées.
*/
typedef struct _win_st window;

/*
  Initialisation de l'interface utilisateur.

  L'appel à cette fonction est indispensable avant toute création de fenêtre.

  La fonction arrête le programme avec une erreur 1 si le terminal utilisé
  est incapable d'afficher des couleurs.
  La fonction initialise aussi le générateur de nombres aléatoires avec
  un noyau qui change à chaque lancement du programme.
*/
void init_gui(void);

/*
  Nettoyage de l'écran et remise du terminal dans l'état initial.
  
  Appel indispensable avant de quitter le programme ou d'afficher un 
  message d'erreur fatale à l'utilisateur.
*/
void cleanup_gui();

/*
  Crée une fenêtre dans le terminal.

  Entoure la fenêtre d'un cadre si 'cadre' vaut 'true'.
  Effectue aussi l'appel à redraw_window() (voir plus loin). La fenêtre
  est donc affichée immédiatement à l'écran.
*/
window *create_window(int largeur, int hauteur, int posX, int posY, bool cadre);

/*
  Détruit une fenêtre et récupère l'espace mémoire qui lui était alloué.
*/
void delete_window(window *w);

/*
  Resynchronize l'affichage d'une fenêtre avec sa représentation interne. 
  A appeler après  putcharXY() ou printfXY() pour que le résultat apparaisse 
  immédiatement à l'écran.
*/
void redraw_window(window *w);

/*  Retourne les dimensions du terminal dans lequel est lancée
    l'application  */
void get_term_size(int *largeur, int *hauteur);

/*  Retourne la largeur du terminal (nombre de colonnes)  */
int get_term_width();
/*  Retourne la hauteur du terminal (nombre de lignes)  */
int get_term_height();
/*  Retourne les dimensions de la fenêtre w  */
void get_window_size(window *w, int *largeur, int *hauteur);
/*  Retourne la largeur de la fenêtre w (nombre de colonnes)  */
int get_window_width(window *w);
/*  Retourne la hauteur de la fenêtre w (nombre de lignes)  */
int get_window_height(window *w);

/* 
   Affiche le caractère 'c' avec la couleur col (voir ci-dessus) dans 
   la fenêtre w en position (posX,posY).
   Note : le caractère n'apparaîtra qu'à la prochaine resynchronisation 
   de la fenêtre avec redraw_window().
*/
void putcharXY(window *w, int posX, int posY, char c, int col);

/*
  Lit un caractère au clavier de façon non bloquante (si aucun caractère 
  n'est disponible, retourne immédiatement ERR)
*/
char pollchar(window *w);

/*
  Affiche une chaîne formatée (comme avec printf) dans la fenêtre w à 
  partir de la position (posX,posY) dans la couleur col.
*/
int printfXY(window *w, int posX, int posY, int col, const char *patron, ...);

void clear_line_from_to(window *w, int from, int to, int line, int col);
#endif
