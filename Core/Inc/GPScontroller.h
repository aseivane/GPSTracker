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

void update_GPS_from_NMEA();
void configure_GPS();
void GPS_1min_sleep();

#endif /* INC_GPSCONTROLLER_H_ */
