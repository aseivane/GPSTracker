#ifndef INC_GPSCONTROLLER_H_
#define INC_GPSCONTROLLER_H_

#include "GPSmodel.h"

#ifndef MAX_ARGUMENTS
#define MAX_ARGUMENTS 19
#endif

enum GNZDAfields {
	TIME, DAY, MONTH, YEAR};

void initGPS(GPSdata * _gps);
void updateGPS(GPSdata* _gps, uint8_t* bufferDMA);
void updateDateTime( uint8_t* bufferDMA);
uint8_t setDate(uint8_t fields[FIELD_BUFF][FIELD_BUFF]);
uint8_t setTime(uint8_t fields[FIELD_BUFF][FIELD_BUFF]);


#endif /* INC_GPSCONTROLLER_H_ */