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
 *
 * Cette fonction va analyser les paramètres envoyés au programme
 * et si le format est correct, réaliser l'opération demandée
 * (exécution en mode normal, exécution en mode pas-à-pas, désassemblage).
 * Si le format est incorrect un message d'aide indiquant le format à
 * respecter est affiché puis le programme se termine.
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

    FREE_INS(instr);
    break;
  default:
    printf("usage: ./femto (-d | -e | -s) nom_fichier\n");
    printf("\t-d desassemble le code\n");
    printf("\t-e execute le code\n");
    printf("\t-s execute le code en mode pas a pas\n");
  }

  return 0;
}
