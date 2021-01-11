/*
 * Screens.h
 *
 *  Created on: Dec 30, 2020
 *      Author: z0042kvk
 */

#ifndef INC_SCREENS_H_
#define INC_SCREENS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"
#include "GPSmodel.h"
#include "u8g2.h"

#define I2C_ADDRESS 0x3C
#define BUTTONS "  Sig  \n  Sel  "
#define SCREEN_TIME 0U
#define SCREEN_UBIC 1U
#define SCREEN_ALTITUDE 2U
#define SCREEN_ODOMETER 3U
#define SPACE_BETWEEN_BUTTONS_IN_PIXEL 6
#define SPACE_BETWEEN_TEXT_AND_BUTTONS_IN_PIXEL 3

#define BUTTON_OFFSET 2

uint8_t u8x8_byte_stm32hal_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t psoc_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t draw_button_line(u8g2_t *u8g2, u8g2_uint_t y, u8g2_uint_t w, uint8_t cursor, const char *s);
void Screen_init(u8g2_t* u8g2);
void Screen_update();
void draw_screen_time();
void draw_screen_ubic();
void test_screen();

#endif /* INC_SCREENS_H_ */
