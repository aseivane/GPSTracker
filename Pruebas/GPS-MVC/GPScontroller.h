#ifndef INC_GPSCONTROLLER_H_
#define INC_GPSCONTROLLER_H_

#include "GPSmodel.h"

#ifndef MAX_ARGUMENTS
#define MAX_ARGUMENTS 19
#endif

void initGPS(GPSdata * _gps);
void updateGPS(GPSdata* _gps, uint8_t* bufferDMA);

#endif /* INC_GPSCONTROLLER_H_ */