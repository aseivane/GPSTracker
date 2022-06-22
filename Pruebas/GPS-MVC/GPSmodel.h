
#ifndef GPSMODEL_H_
#define GPSMODEL_H_

#include <stdint.h>
#include <stdlib.h>
#define FIELD_BUFF 20

typedef struct GPS {
   float latitude;
   float longitude;
   float altitude;
   uint8_t satellites_in_view;
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
	GPGGA, GPGLL, GPGSA, BDGSA, GPGSV,
	BDGSV, GPRMC,GPVTG, GPZDA, GPTXT};

enum GPGSVfields {
	TOTAL_MESSAGE, MESSAGE, SIV, SAT_NUMBER,
	ELEVATION, AZIMUTH, SNR};



void initGPSmodel(GPSdata * self);
void setGPSdata( GPSdata * self, uint8_t fields[][FIELD_BUFF], enum NMEAtalker talker );
void updateGPGGA(GPSdata * self, uint8_t fields[][FIELD_BUFF]);
void NMEA_deg2dec(float* number);

#endif /* GPSMODEL_H_ */
