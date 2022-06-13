/*
 * SDlogging.h
 *
 *  Created on: Jan 7, 2021
 *      Author: z0042kvk
 */

#ifndef INC_SDLOGGING_H_
#define INC_SDLOGGING_H_

FRESULT open_append ( FIL* fp, const char* path);
void send_uart (char *string);
FRESULT log_data();

#endif /* INC_SDLOGGING_H_ */
