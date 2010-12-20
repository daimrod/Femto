#include "instr.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <math.h>


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

void	f_add	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  reg[ra[0]]	= reg[ra[1]] + reg[ra[2]];
  INC_IP(instr_sa);
}

void	f_b	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  int32_t offset;

  offset = instr_sa->ins[instr_sa->ip] >> 32;
  mod_ip(instr_sa, offset);
}

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

void	f_load	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  uint32_t tmp;
  
  tmp = instr_sa->ins[instr_sa->ip] >> 32;
  reg[ra[0]] = * (float*) &tmp;
  INC_IP(instr_sa);
}

void	f_move	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  reg[ra[0]] = reg[ra[1]];
  INC_IP(instr_sa);
}

void	f_mul	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  reg[ra[0]] = reg[ra[1]] * reg[ra[2]];
  INC_IP(instr_sa);
}

void	f_pow	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  uint32_t tmp;

  tmp = instr_sa->ins[instr_sa->ip] >> 32;
  reg[ra[0]] = powf(reg[ra[1]], * (float*) &tmp);
  INC_IP(instr_sa);
}

void	f_print	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  printf("%f\n", reg[ra[0]]);
  INC_IP(instr_sa);
}

void	f_read	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  scanf("%f", &(reg[ra[0]]));
  while (fgetc(stdin) != '\n');
  INC_IP(instr_sa);
}

void	f_stop	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  exit(0);
}

void	f_sub	(float *reg, uint8_t *ra, instr_s *instr_sa) {
  reg[ra[0]] = reg[ra[1]] - reg[ra[2]];
  INC_IP(instr_sa);
}
