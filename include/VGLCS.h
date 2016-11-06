#ifndef VGLCS_H
#define VGLCS_H

#include <stdint.h>

int serial_VGLCS(int nA, char A[], uint16_t GA[],
					int nB, char B[], uint16_t GB[]);

int parallel_VGLCS(int nA, char A[], uint16_t GA[],
					int nB, char B[], uint16_t GB[]);

int serial_ELVGLCS(int nA, char A[], uint16_t GA[][2],
					int nB, char B[], uint16_t GB[][2]);

int parallel_ELVGLCS(int nA, char A[], uint16_t GA[][2],
					int nB, char B[], uint16_t GB[][2]);

#endif
