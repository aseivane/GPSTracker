/*
 ============================================================================
 Name        : GPSmodel.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 hdma->Instance->CMAR
 */
#include "GPSmodel.h"


/**
  * @brief  Crea una instancia de GPS en caso de que se use de forma dinamica
  * @param
  * @retval puntero a instancia GPSdata
  */
/*
GPSdata * CreateGPS()
{
	GPSdata * newGPS = malloc(sizeof(GPSdata));

	GPS_init(newGPS);

    return newGPS;
}*/

void GPS_init(GPSdata * self)
{
	  self->latitude = 0;
	  self->longitude = 0;
	  self->GPS_fix = 0;
	  self->satellites_in_view = 0;
	  self->HDOP = 0;
	  self->altitude = 0;
	  //self->sat_list = NULL;
}

// "Property" setter:
void GPS_update(GPSdata * self, uint8_t fields[][FIELD_BUFF], enum NMEAtalker talker)
{
    switch(talker)
    {
    case GNGGA:
    	if(fields[1][0] != '\0')  GPS_update_GNGGA(self, fields);
    	break;
    case GNGLL:

    	break;
    case GPGSA:

    	break;
    case BDGSA:

    	break;
    case GPGSV:

    	break;
    case BDGSV:

    	break;
    case GNRMC:

    	break;
    case GNVTG:

    	break;
    case GNZDA:

    	break;
    case GPTXT:

    	break;
    }
}

void GPS_update_GNGGA(GPSdata * self, uint8_t fields[][FIELD_BUFF])
{
	enum GNGGAfields {
		TIME, LATITUDE, NoS, LONGITUDE, EoW,
		FIX, SIV, HDOP, ALTTUDE_GEOID, ALTTUDE_METER,
		GEO_SEP, GEO_SEP_METER, DIFF, DIFF_REF}; //SIV = SATELLITES IN VIEW

	//self->latitude = NMEA_deg2dec( fields[LATITUDE], 2 );
	ascii_to_float(fields[LATITUDE], &(self->latitude) );
	NMEA_deg2dec(&(self->latitude));
	if(*(fields[NoS]) == 'S')
			self->latitude = -self->latitude;

	//self->longitude = NMEA_deg2dec( fields[LONGITUDE], 3 );
	ascii_to_float(fields[LONGITUDE], &(self->longitude) );
	NMEA_deg2dec(&(self->longitude));
	if(*(fields[EoW])== 'W')
			self->longitude = -self->longitude;

	self->GPS_fix = (uint8_t) strtol(fields[FIX], NULL, 10);

	self->satellites_in_view = (uint8_t) strtol(fields[SIV], NULL, 10);

	self->HDOP = (uint8_t) strtol(fields[HDOP], NULL, 10);

	self->altitude = (uint8_t) strtol(fields[ALTTUDE_METER], NULL, 10);
}

void NMEA_deg2dec(float* number)
{
	//if(!*number) return;
	*number/=100;
	int integer = (int) *number;//grados
	*number = *number - integer;// quedan solo los minutos y sus decimales
	*number = (float) integer + (*number)*(10/6); //a los grados le agrega los minutos /60 para que sean grados
}

/*
satellite * CreateSatellite(   uint8_t number, uint8_t elevation,
		unsigned int azimut, uint8_t SNR )
{
	satellite * newSatellite = malloc(sizeof(satellite));

    Satellite_init(newSatellite, number, elevation, azimut, SNR );

    return newSatellite;
}

void Satellite_init(satellite * new_sat, uint8_t number, uint8_t elevation,
		unsigned int azimut, uint8_t SNR )
{
	new_sat->number = number;
	new_sat->elevation = elevation;
	new_sat->azimut = azimut;
	new_sat->SNR = SNR;
	new_sat->next = NULL;
}

void freeSatellites(GPSdata * self)
{
	if(self->sat_list)
		free_all_satellite(&(self->sat_list));
}

void free_all_satellite(satellite ** chain)
{
	if((*chain)->next)
		free_last_satellite(&((*chain)->next));
	free(*chain);
	(*chain) = NULL;

}

void free_last_satellite(satellite ** currentnode)
{
	if((*currentnode)->next)
		free_last_satellite(& ((*currentnode)->next));
	else
	{
		free((*currentnode));
		(*currentnode) = NULL;
	}
}

void FreeGPS(GPSdata * self)
{
    // Do any other freeing required here.
	freeSatellites(self);
    free(self);
}
*/
