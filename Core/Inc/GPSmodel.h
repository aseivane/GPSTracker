/*
 * GPSmodel.h
 *
 *  Created on: 24 dic. 2020
 *      Author: z0042kvk
 */

#ifndef GPSMODEL_H_
#define GPSMODEL_H_

#include <stdint.h>
#include <stdlib.h>
#include "tools.h"
#define FIELD_BUFF 20

typedef struct satellite_in_view {
   uint8_t index;
   uint8_t number;
   uint8_t elevation;
   unsigned int azimut;
   uint8_t SNR;
   struct satellite_in_view* next;
} satellite;

typedef struct GPS {
   float latitude;
   float longitude;
   uint8_t GPS_fix;
   uint8_t satellites_in_view;
   uint8_t HDOP;
   uint8_t altitude;
   //satellite* sat_list;

} GPSdata;

typedef struct time {
   uint8_t hour;
   uint8_t minute;
   uint8_t seconds;
   uint8_t year;
   uint8_t month;
   uint8_t day;
   uint8_t UTC;
} GPStime;

enum NMEAtalker {
	GNGGA, GNGLL, GPGSA, BDGSA, GPGSV,
	BDGSV, GNRMC,GNVTG, GNZDA, GPTXT};



enum GPGSVfields {
	TOTAL_MESSAGE, MESSAGE, SIV, SAT_NUMBER,
	ELEVATION, AZIMUTH, SNR};

enum GNZDAfields {
	TIME, DAY, MONTH, YEAR};
/*
GPSdata * CreateGPS();
satellite * CreateSatellite(   uint8_t number, uint8_t elevation,
		unsigned int azimut, uint8_t SNR );
void Satellite_init(satellite * new_sat, uint8_t number, uint8_t elevation,
		unsigned int azimut, uint8_t SNR );*/
void GPS_init(GPSdata * self);
void fields_to_GPS(GPSdata * self, uint8_t fields[][FIELD_BUFF]);
void GPS_update(GPSdata * self, uint8_t fields[][FIELD_BUFF], enum NMEAtalker talker);
void GPS_update_GNGGA(GPSdata * self, uint8_t fields[][FIELD_BUFF]);
/*void freeSatellites(GPSdata * self);
void free_all_satellite(satellite ** chain);
void free_last_satellite(satellite ** currentnode);
void FreeGPS(GPSdata * self);*/
void NMEA_deg2dec(float* number);

#endif /* GPSMODEL_H_ */
