/**
 * \file util.c
 * \brief Ce fichier contient quelques fonctions facilitant la lecture
 * du code
 */

#include "util.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * \fn void* xmalloc(size_t size)
 * \brief Alloue la mémoire demandée et vérifie le code retour de la
 * fonction malloc
 *
 * \param size taille de la mémoire à allouer
 * \return un pointeur sur la zone allouée
 *
 * S'il y a une erreur lors de l'allocation on affiche un message d'erreur
 * et on quitte le programme.
 */
void* xmalloc(size_t size) {
  void* ret = NULL;

  if ((ret = malloc(size)) == NULL) {
    fputs("erreur: impossible d'allouer la memoire\n", stderr);
    exit(-1);
  }
  return ret;
}
