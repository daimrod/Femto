#include "emula.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * \fn void	split(uint64_t instr, uint8_t *op, uint8_t *suf, uint8_t *ra)
 * \brief Décompose l'instruction donnée en sous-instructions interprétables
 *
 * \param instr l'instruction à décomposer
 * \param op l'opération à effectuer
 * \param suf le suffixe lié à l'opération
 * \param ra un tableau indiquant les registres à utiliser durant l'opération
 *
 * Décompose la partie de l'instruction pour en permettre l'interprétation
 */
void	split(uint64_t instr, uint8_t *op, uint8_t *suf, uint8_t *ra) {
  *op	= (instr & 0xFF000000) >> 24;
  *suf	= (instr & 0x00E00000) >> 21;
  ra[0]	= (instr & 0x001FC000) >> 14;
  ra[1]	= (instr & 0x00003F80) >> 7;
  ra[2]	= (instr & 0x0000007F);
}

/**
 * \fn instr_s*	read_file(char *name)
 * \brief Lit le fichier spécifié et initialise le contexte de l'émulateur
 *
 * \param name le nom fichier à lire
 * \return le contexte de l'émulateur
 *
 * Lit le fichier ligne par ligne en convertissant chaque ligne en une instruction
 * qui pourra être interprétée.
 * Cette fonction ne vérifie pas que le fichier a une forme correcte.
 */
instr_s*	read_file(char *name) {
  FILE *fd;
  char line[255];
  instr_s *ret;
  uint32_t i, size;
  uint64_t *mem;

  /*  reservation de la memoire pour stocker toutes les instructions  */
  size = 10;
  if ((mem = (uint64_t*) malloc(sizeof(uint64_t) * size)) == NULL) {
    fputs("erreur: impossible d'allouer la memoire pour la lecture\n", stderr);
    exit(-1);
  }

  /*  ouverture le fichier specifie en lecture seule  */
  if ((fd = fopen(name, "r")) == NULL) {
    fprintf(stderr, "erreur: impossible d'ouvrir le fichier %s\n", name);
    exit(-1);
  }
  
  /*  lecture fichier ligne par ligne et stocke chaque instruction dans mem  */
  i = 0;
  while (fgets(line, 255, fd) != NULL) {
    mem[i] = str_to_uint64(line);
    i++;
    if (i >= size) {
      size *= 2;
      /*  si on n'a pas assez de place on double la taille  */
      if ((mem = (uint64_t*) realloc(mem, sizeof(uint64_t) * size)) == NULL) {
	fputs("erreur: impossible d'allouer la memoire pour la lecture\n", stderr);
	exit(-1);
      }
    }
  }

  /*  fermeture du fichier  */
  fclose(fd);
  fd = NULL;

  /*  reservation de la memoire pour la structure a retourner  */
  if ((ret = (instr_s*) malloc(sizeof(instr_s))) == NULL) {
	fputs("erreur: impossible d'allouer la memoire\n", stderr);
	exit(-1);
  }

  /*  
   *  allocation pile poil la bonne taille pour le tableau d'instruction de la structure plus
   *  une instruction vide a la fin
   */
  if((ret->ins = (uint64_t*) malloc(sizeof(uint64_t) * i)) == NULL) {
	fputs("erreur: impossible d'allouer la memoire\n", stderr);
	exit(-1);
  }

  /*  remplissage de la structure  */
  ret->ins = (uint64_t*) memcpy(ret->ins, mem, sizeof(uint64_t) * i);
  ret->ip = 0;
  ret->nb = i;
  ret->flags = 0; 

  /* liberation de la memoire allouee temporairement */
  free(mem);
  mem = NULL;

  return ret;
}

/**
 * \fn uint64_t str_to_uint64(char *str)
 * \brief Converti une chaine de caractères en un entier codé sur 64 bits
 *
 * \param str la chaine de caractères à convertir
 * \return la chaine de caractères convertie en entier
 */
uint64_t str_to_uint64(char *str) {
  uint64_t ret;
  size_t i;

  ret = 0;
  /* on lit tant que le caractere courant est un nombre */
  for (i = 0; str[i] >= '0' && str[i] <= '9'; ++i)
    ret = ret * 10 + (str[i] - '0');
  
  return ret;
}

/**
 * \fn void emula(instr_s *instr)
 * \brief Algorithme principal émulant le processeur femto,
 * il va exécuter toutes les instructions les unes après les autres
 *
 * \param instr la structure de donnée correspondant au contexte courant
 */
void emula(instr_s *instr) {
  fp_instr *fp_instr_a;
  uint64_t ins_cur;
  uint8_t op, suf, ra[3];
  float reg[16];
  size_t i;

  fp_instr_a = f_init();
  for (i = 0; i < 16; ++i)
    reg[i] = 0;

  while (instr->ip < instr->nb) {
    /*  Recuperation de l'instruction courante  */
    ins_cur = instr->ins[instr->ip];

    /*  Decomposition de l'instruction  */
    split(ins_cur, &op, &suf, ra);

    /*  On execute l'instruction demandee si necessaire  */
    if ((suf & instr->flags) | !suf)
      fp_instr_a[op](reg, ra, instr);
    /* sinon on passe a l'instruction suivante */
    else
      INC_IP(instr);
  }

  free(fp_instr_a);
  fp_instr_a = NULL;
}

/**
 * \fn void emula(instr_s *instr)
 * \brief Algorithme principal émulant le processeur femto,
 * il va exécuter toutes les instructions en mode pas à pas
 *
 * \param instr la structure de donnée correspondant au contexte courant
 *
 * En mode pas à pas, toutes les instructions sont affichées désassemblées
 * et le programme attend que l'utilisateur appuie sur la touche *entrée*
 * pour exécuter l'instruction courante.
 */
void emula_sbs(instr_s *instr) {
  fp_instr *fp_instr_a;
  uint64_t ins_cur;
  uint8_t op, suf, ra[3];
  float reg[16];
  size_t i;

  fp_instr_a = f_init();
  for (i = 0; i < 16; ++i)
    reg[i] = 0;

  while (instr->ip < instr->nb) {
    /*  Recuperation de l'instruction courante  */
    ins_cur = instr->ins[instr->ip];

    /*  Affichage de l'instruction desassemblee */
    desa_print_line(ins_cur);
    getchar();

    /*  Decomposition de l'instruction  */
    split(ins_cur, &op, &suf, ra);

    /*  On execute l'instruction demandee si necessaire  */
    if ((suf & instr->flags) | !suf)
      fp_instr_a[op](reg, ra, instr);
    /* sinon on passe a l'instruction suivante */
    else
      INC_IP(instr);
    printf("\n");
  }

  free(fp_instr_a);
  fp_instr_a = NULL;
}

/**
 * \fn void desa(instr_s *instr)
 * \brief Désassemble le programme chargé en mémoire selon
 * le jeu d'instruction femto
 *
 * \param instr la structure de donnée correspondant au contexte courant
 *
 * Cette fonction parcourt le tableau d'instruction et affiche au fur et
 * à mesure le code qui est désassemblé.
 */
void desa(instr_s *instr) {
  size_t i;
  uint64_t ins_cur;

  for (i = 0; i < instr->nb; ++i) {
    /*  Recuperation de l'instruction courante  */
    ins_cur = instr->ins[i];

    /*  Affichage de l'instruction desassemblee */
    desa_print_line(ins_cur);
    printf("\n");
  }
}

/**
 * \fn void desa_print_line(uint64_t ins_cur)
 * \brief Affiche le code désassemblé de l'instruction envoyée
 * en paramètre
 *
 * \param instr la structure de donnée correspondant au contexte courant
 */
void desa_print_line(uint64_t ins_cur) {
  uint32_t tmp;
  uint8_t op, suf, ra[3];
  char *suf_a[] = { "AL", "EQ", "LT", "LE", "NE" };
  char *ins_a[] = { "add", "b", "cmp", "load", "move",
		    "mul", "pow", "print", "read",
		    "stop", "sub" };
  char *ra_a[] = { " r%d, r%d, r%d", "", " r%d, r%d", " r%d",
		   " r%d, r%d", " r%d, r%d, r%d", " r%d, r%d",
		   " r%d", " r%d", "", " r%d, r%d, r%d" };

  /*  Decomposition de l'instruction  */
  split(ins_cur, &op, &suf, ra);

  /*  Affichage de la ligne */
  printf("%s%s", ins_a[op], suf_a[suf]);
  printf(ra_a[op], ra[0], ra[1], ra[2]);

  /* teste les trois cas necessitant la partie haute de l'instruction */
  switch (op) {
  case 1:
    tmp = ins_cur >> 32;
    printf(" %d", * (int*) &tmp);
    break;
  case 3:
    tmp = ins_cur >> 32;
    printf(" %f", * (float*) &tmp);
    break;
  case 6:
    tmp = ins_cur >> 32;
    printf(" %f", * (float*) &tmp);
    break;
  }
}
