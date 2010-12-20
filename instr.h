/**
 * \file instr.h
 */
#ifndef _INSTR_H_
#define _INSTR_H_

#include <stdint.h>

/**
 * \df INC_IP(instr) mod_ip(instr, 1)
 * \brief Incrémente le pointeur instruction pour passer à l'instruction
 * suivante
 *
 * \param instr le contexte de l'émulateur
 */
#define INC_IP(instr_sa) mod_ip(instr_sa, 1)

/**
 * \struct instr_s
 * \brief Structure correspondant au contexte de l'émulateur
 *
 * \param ins correspond à un tableau contenant toutes les instructions
 * du programme en cours d'exécution
 * \param nb correspond au nombre d'instructions
 * \param ip correspond à l'instruction courante
 * \param flags correspond au résultat de la dernière comparaison
 */
typedef struct {
  uint64_t *ins;
  uint32_t nb;
  uint32_t ip;
  uint8_t flags;
} instr_s;

/**
 * \df typedef void (*fp_instr)(float*, uint8_t*, instr_s*)
 * \brief Définit un alias pour un pointeur de fonction
 */
typedef void (*fp_instr)(float*, uint8_t*, instr_s*);

fp_instr*	f_init(void);
void mod_ip(instr_s *instr_sa, int32_t offset);

void	f_add	(float *reg, uint8_t *ra, instr_s *instr_sa);
void	f_b	(float *reg, uint8_t *ra, instr_s *instr_sa);
void	f_cmp	(float *reg, uint8_t *ra, instr_s *instr_sa);
void	f_load	(float *reg, uint8_t *ra, instr_s *instr_sa);
void	f_move	(float *reg, uint8_t *ra, instr_s *instr_sa);
void	f_mul	(float *reg, uint8_t *ra, instr_s *instr_sa);
void	f_pow	(float *reg, uint8_t *ra, instr_s *instr_sa);
void	f_print	(float *reg, uint8_t *ra, instr_s *instr_sa);
void	f_read	(float *reg, uint8_t *ra, instr_s *instr_sa);
void	f_stop	(float *reg, uint8_t *ra, instr_s *instr_sa);
void	f_sub	(float *reg, uint8_t *ra, instr_s *instr_sa);

#define AL 0
#define EQ 1
#define LT 2
#define LE 3
#define NE 4

#endif
