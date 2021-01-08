/*
 * file.h
 *
 *  Created on: 4 ene. 2021
 *      Author: z0042kvk
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <stdint.h>

//Macro to set nth-bit
/*
Set single bit at pos to '1' by generating a mask
in the proper bit location and ORing (|) x with the mask.
*/
#define SET_BIT(x, pos) (x |= (1U << pos))
/*
Set single bit at pos to '0' by generating a mask
in the proper bit location and Anding x with the mask.
*/
#define CLEAR_BIT(x, pos) (x &= (~(1U<< pos)))
/*
Set single bit at pos to '1' by generating a mask
in the proper bit location and ex-ORing x with the mask.
*/
#define TOGGLE_BIT(x, pos) x ^= (1U<< pos)
/*
Set single bit at pos to '1' by generating a mask
in the proper bit location and Anding x with the mask.
It evaluates 1 if a bit is set otherwise 0.
*/
#define CHECK_BIT(x, pos) (x & (1UL << pos) )
//Macro to swap nibbles
#define SWAP_NIBBLES(x) ((x & 0x0F)<<4 | (x & 0xF0)>>4)
//Macro to Get bit from the given position
#define GET_BITS(x, pos) ((x & ( 1 << pos)) >> pos)
//Macro to swap byte of 32-bit +ve integer variable
#define SWAP_BYTES(u32Value) ((u32Value & 0x000000FF) << 24)\
|((u32Value & 0x0000FF00) << 8) \
|((u32Value & 0x00FF0000) >> 8) \
|((u32Value & 0xFF000000) >> 24)

//macro to swap odd-even bits
#define SWAP_ODD_EVEN_BIT(x) ((x & 0xAAAAAAAA)>>1 | (x & 0x55555555)<<1);
//macro to swap two numbers
#define SWAP(x, y) (x ^= y ^= x)
//macro to get low and high bytes
#define LOWBYTE(v)   ((unsigned char) (x))
#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (x)) >> 8))

int pow_10(uint8_t n);
void ascii_to_float(uint8_t* str, float* num);
int setBit(int n, int k);
int clearBit(int n, int k);
int toggleBit(int n, int k);

#endif /* TOOLS_H_ */
