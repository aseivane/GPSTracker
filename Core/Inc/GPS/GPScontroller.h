/*
 * GPScontroller.h
 *
 *  Created on: Dec 30, 2020
 *      Author: z0042kvk
 */

#ifndef INC_GPSCONTROLLER_H_
#define INC_GPSCONTROLLER_H_

#ifndef MAX_ARGUMENTS
#define MAX_ARGUMENTS 19
#endif

#define ZDA_PAYLOAD 11
#define GGA_PAYLOAD 11
#define GBS_PAYLOAD 11
#define RMC_PAYLOAD 11
#define GLL_PAYLOAD 11
#define RXM_PAYLOAD 10

void initGPS(GPSdata * _gps);
void updateGPS();
void updateTime(uint8_t* copy_buffer);
uint8_t setTime(uint8_t* copy_buffer);
uint8_t setDate(uint8_t** fields);
void configGPS();
void GPS_1min_sleep();

#endif /* INC_GPSCONTROLLER_H_ */
