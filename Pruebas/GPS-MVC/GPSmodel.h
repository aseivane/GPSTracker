
#ifndef GPSMODEL_H_
#define GPSMODEL_H_

#include <stdint.h>
#include <stdlib.h>
#define FIELD_BUFF 20

typedef struct GPS {
   float latitude;
   float longitude;
   uint8_t satellites_in_view;
   uint8_t altitude;
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

void initGPSmodel(GPSdata * self);

#endif /* GPSMODEL_H_ */
