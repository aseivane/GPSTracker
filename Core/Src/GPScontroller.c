/*
 * GPScontroller.c
 *
 *  Created on: Dec 30, 2020
 *      Author: z0042kvk
 */

#include "GPScontroller.h"
#include "Screens.h"
#include "GPSmodel.h"
#include "main.h"
#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern GPSdata gps;

extern RTC_HandleTypeDef hrtc;

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern uint8_t time_updated;
extern uint8_t date_updated;

void update_GPS_from_NMEA()
{
	uint8_t f_to_char[100];
	uint8_t fields[FIELD_BUFF][FIELD_BUFF];
	uint8_t* ptrSentence;
	uint8_t field_count;
	RTC_TimeTypeDef Time;
	RTC_DateTypeDef Date;
	extern uint8_t usart_rx_dma_buffer[];
	uint8_t copy_buffer1[MAX_NMEA_LEN];

	memcpy(copy_buffer1, usart_rx_dma_buffer, MAX_NMEA_LEN);

	ptrSentence = get_sentence_ptr(copy_buffer1, "GNGGA", NULL);
	if( ptrSentence != NULL )
	{
		if( is_sentence_complete(copy_buffer1, ptrSentence) )
		{
			field_count = coma_count(ptrSentence);
			get_fields(ptrSentence , fields);
			GPS_update(&gps, fields, GNGGA);
		}
	}
	//memcpy(copy_buffer1, usart_rx_dma_buffer, MAX_NMEA_LEN);
	if(!(time_updated && date_updated))
	{
		ptrSentence = get_sentence_ptr(copy_buffer1, "GNZDA", NULL);
		if( ptrSentence != NULL )
		{
			if( is_sentence_complete(copy_buffer1, ptrSentence) )
			{
				//get_fields(copy_buffer1,fields);
				get_fields(ptrSentence , fields);
				if(strcmp(fields[0],""))
				{
				  memcpy(f_to_char,fields[0],6);
				  f_to_char[6] = '\0';
				  Time.Seconds = (uint8_t) strtol((char*)f_to_char+4, NULL, 10);
				  f_to_char[4] = '\0';
				  Time.Minutes = (uint8_t) strtol((char*)f_to_char+2, NULL, 10);
				  f_to_char[2] = '\0';
				  Time.Hours = (uint8_t) strtol((char*)f_to_char, NULL, 10) - 3;

				  HAL_RTC_SetTime(&hrtc, &Time,  RTC_FORMAT_BIN);
				  time_updated=1;
				}

				if(strcmp(fields[1],""))
				{
				  Date.Date = (uint8_t) strtol(fields[1], NULL, 10);
				  Date.Month = (uint8_t) strtol(fields[2], NULL, 10);
				  Date.Year = (uint8_t) strtol(fields[3]+2, NULL, 10);


				  HAL_RTC_SetDate(&hrtc, &Date,  RTC_FORMAT_BIN);
				  date_updated=1;
				}
			}
		}
	}
}
