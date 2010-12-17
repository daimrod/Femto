/**
 * \file main.c
 * \brief Programme principale pour l'émulateur femto
 *
 * Programme réalisant l'émulation d'un processeur femto en lisant le fichier indiqué
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emula.h"

/**
 * \fn int main(int argc, char *argv[])
 * \brief point d'entrée du programme
 *
 * \param argc nombre de paramètres envoyés au programme
 * \param argv paramètres envoyés au programme
 * \return un code d'erreur
 */
int main(int argc, char *argv[]) {
  instr_s *instr;

  switch (argc) {
  case 3:
    instr = read_file(argv[2]);

    if (!strncmp(argv[1], "-d", 2))
      desa(instr);
    else if (!strncmp(argv[1], "-e", 2))
      emula(instr);
    else if (!strncmp(argv[1], "-s", 2))
      emula_sbs(instr);

    free(instr->ins);
    instr->ins = NULL;
    free(instr);
    instr = NULL;
    break;
  default:
    printf("usage: ./femto (-d | -e | -s) nom_fichier\n");
    printf("\t-d desassemble le code\n");
    printf("\t-e execute le code\n");
    printf("\t-s execute le code en mode pas a pas\n");
  }

  return 0;
}
