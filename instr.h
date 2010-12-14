/**
 * \file instr.h
 */
#ifndef _INSTR_H_
#define _INSTR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/**
 * \df INC_IP(instr) mod_ip(instr, 1)
 * \brief Incrémente le pointeur instruction pour passer à l'instruction
 * suivante
 *
 * \param instr le contexte de l'émulateur
 */
#define INC_IP(instr) mod_ip(instr, 1)

/**
 * \struct instr_s
 * \brief Structure correspondant au contexte de l'émulateur
 *
 * \param ins correspond à un tableau contenant toutes les instructions
 * du programme en cours d'exécution
 * \param nb correspond au nombre d'instructions
 * \param ip correspond a l'instruction courante
 */
typedef struct {
  uint64_t *ins;
  uint32_t nb;
  uint32_t ip;
} instr_s;

/**
 * \df typedef void (*fp_instr)(float*, uint8_t*, instr_s*, uint8_t*)
 * \brief Définit un alias pour un pointeur de fonction
 */
typedef void (*fp_instr)(float*, uint8_t*, instr_s*, uint8_t*);

fp_instr*	f_init(void);

void	f_add	(float *reg, uint8_t *ra, instr_s *instr, uint8_t *flags);
void	f_b	(float *reg, uint8_t *ra, instr_s *instr, uint8_t *flags);
void	f_cmp	(float *reg, uint8_t *ra, instr_s *instr, uint8_t *flags);
void	f_load	(float *reg, uint8_t *ra, instr_s *instr, uint8_t *flags);
void	f_move	(float *reg, uint8_t *ra, instr_s *instr, uint8_t *flags);
void	f_mul	(float *reg, uint8_t *ra, instr_s *instr, uint8_t *flags);
void	f_pow	(float *reg, uint8_t *ra, instr_s *instr, uint8_t *flags);
void	f_print	(float *reg, uint8_t *ra, instr_s *instr, uint8_t *flags);
void	f_read	(float *reg, uint8_t *ra, instr_s *instr, uint8_t *flags);
void	f_stop	(float *reg, uint8_t *ra, instr_s *instr, uint8_t *flags);
void	f_sub	(float *reg, uint8_t *ra, instr_s *instr, uint8_t *flags);

#define AL 0
#define EQ 1
#define LT 2
#define LE 3
#define NE 4

#endif
