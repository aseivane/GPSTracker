/*
 * file.h
 *
 *  Created on: 4 ene. 2021
 *      Author: z0042kvk
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <stdint.h>
#include "main.h"

int pow_10(uint8_t n);
int ascii_to_int(uint8_t *str);
void ascii_to_float(uint8_t* str, float* num);
void reverse(char* str, int len);
__uint8_t int_to_ascii(int x, char dest[], __uint8_t d);
void float_to_ascii(float n, uint8_t* res, uint8_t afterpoint);
void insert_chars(uint8_t *dest, uint8_t *src, uint8_t d);
void get_time_string(RTC_TimeTypeDef *Time, uint8_t *time_string);

#endif /* TOOLS_H_ */
