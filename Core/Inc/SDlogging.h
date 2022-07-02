/*
 * SDlogging.h
 *
 *  Created on: Jan 7, 2021
 *      Author: z0042kvk
 */

#ifndef INC_SDLOGGING_H_
#define INC_SDLOGGING_H_

#include "fatfs.h"
#include "GPS/GPSmodel.h"
#include <string.h>

#define SIGN_CHAR 1
#define INTEGER_CHARS 3
#define POINT_CHAR 1
#define APTERPOINT_CHARS 6
#define FLOAT_STRING_SIZE ( SIGN_CHAR + INTEGER_CHARS +\
							POINT_CHAR + APTERPOINT_CHARS +1)
#define TIME_STRING_SIZE 10
#define DATE_STRING_SIZE 10

FRESULT open_append ( FIL* fp, const char* path);
void send_uart (char *string);
FRESULT log_data();

#endif /* INC_SDLOGGING_H_ */
