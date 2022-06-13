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

void fields_to_GPS(GPSdata * self, uint8_t fields[][FIELD_BUFF])
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

	self->GPS_fix = (uint8_t) ascii_to_int(fields[FIX]);

	self->satellites_in_view = (uint8_t) ascii_to_int(fields[SIV]);

	self->HDOP = (uint8_t) ascii_to_int(fields[HDOP]);

	self->altitude = (uint8_t) ascii_to_int(fields[ALTTUDE_METER]);
}



void GPS_update_GNGGA(GPSdata * self, uint8_t fields[][FIELD_BUFF])
{
	static uint8_t count = 0;
	GPSdata aux_data;

	fields_to_GPS(&aux_data, fields);

	if(!count)
	{
		self->latitude = aux_data.latitude;
		self->longitude = aux_data.longitude;
	}
	else
	{
		self->latitude = (self->latitude + aux_data.latitude)/2;
		self->longitude = (self->longitude + aux_data.longitude)/2;
		self->altitude = (self->altitude + aux_data.altitude)/2;
	}
	self->GPS_fix = aux_data.GPS_fix;
	self->satellites_in_view = aux_data.satellites_in_view;
	self->HDOP = aux_data.HDOP;

	count == 10 ? count = 0 : count++;
}

void NMEA_deg2dec(float* number)
{
	//if(!*number) return;
	*number/=100;
	int integer = (int) *number;//grados
	*number = (*number - (float)integer)*100;// quedan solo los minutos y sus decimales
	*number /=60;
	*number = (float) integer + (*number); //a los grados le agrega los minutos /60 para que sean grados
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
