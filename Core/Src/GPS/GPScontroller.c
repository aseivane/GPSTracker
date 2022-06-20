/*
 * GPScontroller.c
 *
 *  Created on: Dec 30, 2020
 *      Author: z0042kvk
 */

#include <OLED/Screens.h>
#include "GPS/GPScontroller.h"
#include "GPS/GPSmodel.h"
#include "GPS/parser.h"

#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#undef DEBUG

extern GPSdata gps;
extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart1;

#ifdef DEBUG
extern UART_HandleTypeDef huart3;
#endif

extern DMA_HandleTypeDef hdma_usart1_rx;
extern uint8_t time_updated;
extern uint8_t date_updated;


void initGPS(GPSdata * _gps)
{
	initGPSmodel(_gps);



	configGPS();	// Sends directives for selected NMEA talkers
}




/**
  * @brief
  */
void updateGPS()
{
	uint8_t* ptrSentence;
	extern uint8_t usart_rx_dma_buffer[];
	uint8_t copy_buffer[MAX_NMEA_LEN];

	uint8_t fields[FIELD_BUFF][FIELD_BUFF];
	for(uint8_t i = 0; i<FIELD_BUFF; i++) // initializes all the pinters
			        memset(fields[i], END_OF_STRING, FIELD_BUFF);

	memcpy(copy_buffer, usart_rx_dma_buffer, MAX_NMEA_LEN);

	getMessageFields( copy_buffer, "GPGGA", fields );

	//GPS_update(&gps, fields, GNGGA);

	//ptrSentence = get_sentence_ptr(copy_buffer, "GPGGA", NULL);
	/*
	if( ptrSentence != NULL )
	{
	  if( is_sentence_complete(copy_buffer, ptrSentence) )
	  {
		  //field_count = coma_count(ptrSentence);
		  get_fields(ptrSentence , fields);
		  GPS_update(&gps, fields, GNGGA);
	  }
	}*/
	/*
	if(!(time_updated && date_updated))
	{
		updateTime(copy_buffer);
	}*/
}
/*
void updateTime(uint8_t copy_buffer[])
{
	uint8_t* ptrSentence;
	uint8_t fields[FIELD_BUFF][FIELD_BUFF];
	uint8_t f_to_char[100];


	ptrSentence = get_sentence_ptr(copy_buffer, "GPZDA", NULL);
	if( ptrSentence != NULL )
	{
		#ifdef DEBUG
		HAL_UART_Transmit(&huart3, (uint8_t *)"Sentence found\r\n", strlen("Sentence found\r\n"),1000);
		#endif
		if( is_sentence_complete(copy_buffer, ptrSentence) )
		{
			  get_fields(ptrSentence , fields);
			  if( *(fields[0]) != '0' )
			  {
				  memcpy(f_to_char,fields[0],6);
				  time_updated = setTime(f_to_char);
			  }

			  if(  *(fields[1]) != '0')
			  {
				  date_updated = setDate(fields);
			  }
		}
	}
}*/

uint8_t setDate(uint8_t** fields)
{
	RTC_DateTypeDef Date;

  	Date.Date = (uint8_t) ascii_to_int(fields[1]);
	Date.Month = (uint8_t) ascii_to_int(fields[2]);
	Date.Year = (uint8_t) ascii_to_int(fields[3]+2);

	HAL_RTC_SetDate(&hrtc, &Date,  RTC_FORMAT_BIN);
	return 1U;
}

uint8_t setTime(uint8_t* f_to_char)
{
	RTC_TimeTypeDef Time;

	f_to_char[6] = '\0';
	Time.Seconds = (uint8_t) ascii_to_int(f_to_char+4);
	f_to_char[4] = '\0';
	Time.Minutes = (uint8_t) ascii_to_int(f_to_char+2);
	f_to_char[2] = '\0';
	Time.Hours = (uint8_t) ascii_to_int(f_to_char) - 3;

	HAL_RTC_SetTime(&hrtc, &Time,  RTC_FORMAT_BIN);
	return 1U;
}

/**
  * @brief  Config NMEA talkers
  * @retval -
  */
void configGPS()
{
	uint8_t const enable_ZDA [ZDA_PAYLOAD] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x08, 0x01, 0x03, 0x20};
	const uint8_t enable_GGA [GGA_PAYLOAD] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x00, 0x01, 0xFB, 0x10};
	const uint8_t enable_GBS [GBS_PAYLOAD] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x09, 0x01, 0x04, 0x22};
	const uint8_t disable_RMC[RMC_PAYLOAD] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x04, 0x00, 0xFE, 0x17};
	const uint8_t disable_GLL[GLL_PAYLOAD] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x01, 0x00, 0xFB, 0x11};
	const uint8_t power_save [RXM_PAYLOAD] = {0xB5, 0x62, 0x06, 0x11, 0x02, 0x00, 0x08, 0x01, 0x22, 0x92};

	HAL_UART_Transmit(&huart1, enable_ZDA, ZDA_PAYLOAD,1000);
	HAL_UART_Transmit(&huart1, enable_GGA, GGA_PAYLOAD,1000);
	HAL_UART_Transmit(&huart1, enable_GBS, GBS_PAYLOAD,1000);
	HAL_UART_Transmit(&huart1, disable_RMC, RMC_PAYLOAD,1000);
	HAL_UART_Transmit(&huart1, disable_GLL, GLL_PAYLOAD,1000);
	HAL_UART_Transmit(&huart1, power_save, RXM_PAYLOAD,1000);
}

void GPS_1min_sleep()
{
	uint8_t UBLOX_GPSStandby[] = {0xB5, 0x62, 0x02, 0x41, 0x08, 0x00, 0x60, 0xEA, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x97, 0xA1};
	HAL_UART_Transmit(&huart1, UBLOX_GPSStandby, 16,1000);
}
