/**
 * \file instr.c
 * \brief Ce fichier contient les fonctions d'émulation de la
 * machine Femto.
 */

#include "instr.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <math.h>


/**
 * \fn void mod_ip(instr_s *instr_sa, int32_t offset)
 * \brief Modifie le pointeur d'instruction selon la valeur passée
 * en paramètre.
 *
 * \param instr_sa le contexte courant
 * \param offset la valeur dont on veut modifier le pointeur d'instruction
 * (peut être positif ou négatif)
 *
 * Cette fonction modifie le pointeur d'instruction et vérifie qu'une
 * fois le sauf effectué le pointeur d'instruction pointe toujours sur
 * une instruction, afin d'éviter une eventuelle erreur de segmentation.
 */
void mod_ip(instr_s *instr_sa, int32_t offset) {
  int32_t new_ip;

  new_ip = instr_sa->ip + offset;
  if (new_ip >= 0 && new_ip <= instr_sa->nb)
    instr_sa->ip = new_ip;
  else {
    fputs("erreur: pointeur instruction hors de portee\n", stderr);
    exit(-1);
  }
}

/**
 * \fn fp_instr*	f_init(void)
 * \brief Cette fonction initialise un talbeau contenant un pointeur
 * vers les différentes instructions de la machine Femto.
 *
 * \return le tableau initialisé
 */
fp_instr*	f_init(void) {
  fp_instr *fp_instr_a;

  fp_instr_a = (fp_instr*) xmalloc(sizeof(fp_instr_a) * 11);
  fp_instr_a[0] = &f_add;
  fp_instr_a[1] = &f_b;
  fp_instr_a[2] = &f_cmp;
  fp_instr_a[3] = &f_load;
  fp_instr_a[4] = &f_move;
  fp_instr_a[5] = &f_mul;
  fp_instr_a[6] = &f_pow;
  fp_instr_a[7] = &f_print;
  fp_instr_a[8] = &f_read;
  fp_instr_a[9] = &f_stop;
  fp_instr_a[10] = &f_sub;
  return fp_instr_a;
}

/**
 * \fn void	f_add	(float *reg, uint8_t *ra, instr_s *instr_sa)
 * \brief Fonction réalisant l'opération d'addition
 *
 * \param reg un talbeau contenant les 16 registres du processeur Femto
 * \param ra un talbeau indiquant les registres à utilisé pour l'opération
 * \param instr_sa le contexte courant
 *
 * add rs, rd, rt |-> rs = rd + rt
 */
void	f_add	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  reg[ra[0]]	= reg[ra[1]] + reg[ra[2]];
  INC_IP(instr_sa);
}

/**
 * \fn void	f_b	(float *reg, uint8_t *ra, instr_s *instr_sa)
 * \brief Fonction réalisant un sauf à l'instruction indiquée
 *
 * \param reg un talbeau contenant les 16 registres du processeur Femto
 * \param ra un talbeau indiquant les registres à utilisé pour l'opération
 * \param instr_sa le contexte courant
 *
 * b offset
 * modifie le pointeur d'instruction selon la valeur
 * indiquée dans la partie haute de l'instruction courante.
 */
void	f_b	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  int32_t offset;

  offset = instr_sa->ins[instr_sa->ip] >> 32;
  mod_ip(instr_sa, offset);
}

/**
 * \fn void	f_cmp	(float *reg, uint8_t *ra, instr_s *instr_sa)
 * \brief Fonction réalisant une comparaison
 *
 * \param reg un talbeau contenant les 16 registres du processeur Femto
 * \param ra un talbeau indiquant les registres à utilisé pour l'opération
 * \param instr_sa le contexte courant
 *
 * cmp rs, rt
 * Compare les deux registres indiqués et met à jour
 * la valeur des drapeaux.
 */
void	f_cmp	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  instr_sa->flags = 0x00;

  if (reg[ra[0]] == reg[ra[1]])
    instr_sa->flags = instr_sa->flags | 0x01;
  else
    instr_sa->flags = instr_sa->flags | 0x04;
  if (reg[ra[0]] < reg[ra[1]])
    instr_sa->flags = instr_sa->flags | 0x02;
  INC_IP(instr_sa);
}

/**
 * \fn void	f_load	(float *reg, uint8_t *ra, instr_s *instr_sa)
 * \brief Fonction affectant une valeur à un registre
 *
 * \param reg un talbeau contenant les 16 registres du processeur Femto
 * \param ra un talbeau indiquant les registres à utilisé pour l'opération
 * \param instr_sa le contexte courant
 *
 * load rd, imm
 * Affecte la valeur indiquée dans la partie haute
 * de l'instruction courante au registre indiqué
 */
void	f_load	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  uint32_t tmp;
  
  tmp = instr_sa->ins[instr_sa->ip] >> 32;
  reg[ra[0]] = * (float*) &tmp;
  INC_IP(instr_sa);
}

/**
 * \fn void	f_move	(float *reg, uint8_t *ra, instr_s *instr_sa)
 * \brief Fonction affectant la valeur d'un registre à un autre
 *
 * \param reg un talbeau contenant les 16 registres du processeur Femto
 * \param ra un talbeau indiquant les registres à utilisé pour l'opération
 * \param instr_sa le contexte courant
 *
 * move rd, rs |-> rd = rs
 * rd reste inchangé
 */
void	f_move	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  reg[ra[0]] = reg[ra[1]];
  INC_IP(instr_sa);
}

/**
 * \fn void	f_mul	(float *reg, uint8_t *ra, instr_s *instr_sa)
 * \brief Fonction réalisant l'opération de multiplication
 *
 * \param reg un talbeau contenant les 16 registres du processeur Femto
 * \param ra un talbeau indiquant les registres à utilisé pour l'opération
 * \param instr_sa le contexte courant
 *
 * mul rs, rd, rt |-> rs = rd * rt
 */
void	f_mul	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  reg[ra[0]] = reg[ra[1]] * reg[ra[2]];
  INC_IP(instr_sa);
}

/**
 * \fn void	f_pow	(float *reg, uint8_t *ra, instr_s *instr_sa)
 * \brief Fonction réalisant l'opération de puissance
 *
 * \param reg un talbeau contenant les 16 registres du processeur Femto
 * \param ra un talbeau indiquant les registres à utilisé pour l'opération
 * \param instr_sa le contexte courant
 *
 * pow rd, rs, n |-> rd = rs^n
 * Réalise l'opération de puissance, n est la valeur indiquée dans la
 * partie haute de l'instruction courante.
 */
void	f_pow	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  uint32_t tmp;

  tmp = instr_sa->ins[instr_sa->ip] >> 32;
  reg[ra[0]] = powf(reg[ra[1]], * (float*) &tmp);
  INC_IP(instr_sa);
}

/**
 * \fn void	f_print	(float *reg, uint8_t *ra, instr_s *instr_sa)
 * \brief Fonction affichant le contenu d'un registre à l'écran
 *
 * \param reg un talbeau contenant les 16 registres du processeur Femto
 * \param ra un talbeau indiquant les registres à utilisé pour l'opération
 * \param instr_sa le contexte courant
 *
 * print rs
 * Affiche rs à l'écran
 */
void	f_print	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  printf("%f\n", reg[ra[0]]);
  INC_IP(instr_sa);
}

/**
 * \fn void	f_read	(float *reg, uint8_t *ra, instr_s *instr_sa)
 * \brief Fonction lisant demandant à l'utilisateur de saisir un nombre
 * pour l'affecter à un registre
 *
 * \param reg un talbeau contenant les 16 registres du processeur Femto
 * \param ra un talbeau indiquant les registres à utilisé pour l'opération
 * \param instr_sa le contexte courant
 *
 * read rd
 * Attend que l'utilisateur saisisse un nombre et affecte ce nombre au
 * registre rd
 */
void	f_read	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  scanf("%f", &(reg[ra[0]]));
  /* on "avale" tous les caractères jusqu'au caratère *entree* */
  while (fgetc(stdin) != '\n');
  INC_IP(instr_sa);
}

/**
 * \fn void	f_stop	(float *reg, uint8_t *ra, instr_s *instr_sa)
 * \brief Fonction terminant l'exécution du programme courant
 *
 * \param reg un talbeau contenant les 16 registres du processeur Femto
 * \param ra un talbeau indiquant les registres à utilisé pour l'opération
 * \param instr_sa le contexte courant
 */
void	f_stop	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  exit(0);
}

/**
 * \fn void	f_sub	(float *reg, uint8_t *ra, instr_s *instr_sa)
 * \brief Fonction réalisant l'opération d'addition
 *
 * \param reg un talbeau contenant les 16 registres du processeur Femto
 * \param ra un talbeau indiquant les registres à utilisé pour l'opération
 * \param instr_sa le contexte courant
 *
 * sub rd, rs, rt |-> rd = rs - rt
 */
void	f_sub	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  reg[ra[0]] = reg[ra[1]] - reg[ra[2]];
  INC_IP(instr_sa);
}
