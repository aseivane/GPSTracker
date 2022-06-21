#include "GPSmodel.h"

/**
  * @brief  Init GPS attributes.
  * @param self pointer to GPSdata object
  * @retval -
  */
void initGPSmodel(GPSdata * self)
{
	  self->latitude = 0;
	  self->longitude = 0;
	  self->satellites_in_view = 0;
	  self->altitude = 0;
}

