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
    case GPGGA:
    	updateGPGGA(self, fields);
    	break;
    case GPGLL:

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
void updateGPGGA(GPSdata * self, uint8_t fields[][FIELD_BUFF])
{
  enum GNGGAfields {
  TIME, LATITUDE, NoS, LONGITUDE, EoW,
  FIX, SIV, HDOP, ALTTUDE_GEOID, ALTTUDE_METER,
  GEO_SEP, GEO_SEP_METER, DIFF, DIFF_REF};
  /* converts latitude field to float */
	ascii_to_float(fields[LATITUDE], &(self->latitude) );

  /* if the latitude is 0, it means the fields are empty, return */
  if ( 0 == self->latitude) return;
  
  /* converts degrees to decimal */
	NMEA_deg2dec(&(self->latitude));
	if(*(fields[NoS]) == 'S') // changes the sign if needed
			self->latitude = -self->latitude;

  /* converts longitude field to float */
	ascii_to_float(fields[LONGITUDE], &(self->longitude) );
	NMEA_deg2dec(&(self->longitude));
	if(*(fields[EoW])== 'W')  // changes the sign if needed
			self->longitude = -self->longitude;

	ascii_to_int(fields[SIV], &(self->satellites_in_view));

	ascii_to_int(fields[ALTTUDE_METER], &(self->altitude));

}

void NMEA_deg2dec(float* number)
{
	if(0 == *number) return;
	*number/=100;
	int integer = (int) *number;//grados
	*number = (*number - (float)integer)*100;// quedan solo los minutos y sus decimales
	*number /=60;
	*number = (float) integer + (*number); //a los grados le agrega los minutos /60 para que sean grados
}