#include "util.h"

void* xmalloc(size_t size) {
  void* ret = NULL;

  if ((ret = malloc(size)) == NULL) {
    fputs("erreur: impossible d'allouer la memoire\n", stderr);
    exit(-1);
  }
  return ret;
}
