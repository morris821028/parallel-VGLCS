#ifndef __ENCRYPT_H
#define __ENCRYPT_H
#include <stdint.h>
uint32_t seed = 0;
void p_srand(uint32_t x) { seed = x;}
uint32_t p_func(uint32_t x) {return (x*9301+49297)%233280;}
uint32_t p_rand() {return seed = (seed*9301 + 49297);}

#endif
