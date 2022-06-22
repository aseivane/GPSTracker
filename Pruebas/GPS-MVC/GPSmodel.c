#include "GPSmodel.h"
#include "tools.h"

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

/**
  * @brief  Saves data in GPSdata object from array of fields
  * @param self pointer to GPSdata object
  * @param fields array of uint8_t strings
  * @param talker NMEAtalker 
  * @retval -
  */
void setGPSdata( GPSdata * self, uint8_t fields[][FIELD_BUFF], enum NMEAtalker talker )
{
    switch(talker)
    {
    case GNGGA:
    	updateGNGGA(self, fields);
    	break;
    case GNGLL:

    	break;
    default:
      break;
    }
}

/**
  * @brief  Saves data in GPSdata from the fields of GNGGA talker
  * @param self pointer to GPSdata object
  * @param fields array of uint8_t strings
  * @retval -
  */
void updateGNGGA(GPSdata * self, uint8_t fields[][FIELD_BUFF])
{
  enum GNGGAfields {
  TIME, LATITUDE, NoS, LONGITUDE, EoW,
  FIX, SIV, HDOP, ALTTUDE_GEOID, ALTTUDE_METER,
  GEO_SEP, GEO_SEP_METER, DIFF, DIFF_REF};

	ascii_to_float(fields[LATITUDE], &(self->latitude) );
	NMEA_deg2dec(&(self->latitude));
	if(*(fields[NoS]) == 'S')
			self->latitude = -self->latitude;

	//self->longitude = NMEA_deg2dec( fields[LONGITUDE], 3 );
	ascii_to_float(fields[LONGITUDE], &(self->longitude) );
	NMEA_deg2dec(&(self->longitude));
	if(*(fields[EoW])== 'W')
			self->longitude = -self->longitude;

	self->satellites_in_view = (uint8_t) ascii_to_int(fields[SIV]);

	self->altitude = (uint8_t) ascii_to_int(fields[ALTTUDE_METER]);

}