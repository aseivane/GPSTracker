/*
 * GPScontroller.c
 *
 *  Created on: Dec 30, 2020
 *      Author: z0042kvk
 */

#include "GPS/GPScontroller.h"

extern UART_HandleTypeDef huart1;

#ifdef DEBUG
extern UART_HandleTypeDef huart3;
#endif


/**
  * @brief  Init GPS attributes.
  * @param self pointer to GPSdata object
  * @retval -
  */
void initGPS(GPSdata * _gps)
{
	initGPSmodel(_gps);

	configGPS();	// Sends directives for selected NMEA talkers
}




/**
  * @brief  Update GPS attributes.
  * @param self pointer to GPSdata object
  * @param bufferDMA pointer to UART DMA buffer
  * @retval -
  */
void updateGPS(GPSdata* _gps, const uint8_t bufferDMA[MAX_NMEA_LEN], uint16_t* msgSize)
{

	uint8_t fields[FIELD_BUFF][FIELD_BUFF];
	for(uint8_t i = 0; i<FIELD_BUFF; i++) // initializes all the pinters
		memset(fields[i], END_OF_STRING, FIELD_BUFF);
#ifdef DEBUG
	HAL_UART_Transmit(&huart3, bufferDMA, *msgSize,1000);
#endif
	getMessageFields( bufferDMA, msgSize, (uint8_t*) "GPGGA", fields );

	setGPSdata(_gps, fields, GPGGA);

#ifdef DEBUG2
	HAL_UART_Transmit(&huart3, (uint8_t *)"***EXIT updateGPS***\r\n\r\n",
						strlen("***EXIT updateGPS***\r\n\r\n"),1000);
	HAL_UART_Transmit(&huart3, copy_buffer, *msgSize,1000);
#endif

}
/**
  * @brief  Update RTC date and time.
  * @param self pointer to GPSdata object
  * @param bufferDMA pointer to UART DMA buffer
  * @retval -
  */
void updateDateTime( RTC_HandleTypeDef* hrtc, const uint8_t bufferDMA[], uint16_t* msgSize)
{
	static uint8_t updatedTime = FALSE;
	static uint8_t updatedDate = FALSE;
#ifdef DEBUG2
	HAL_UART_Transmit(&huart3, bufferDMA, *msgSize,1000);
#endif
	if ( TRUE == updatedTime && TRUE == updatedDate ) return;

	uint8_t fields[FIELD_BUFF][FIELD_BUFF];

	//for(uint8_t i = 0; i<FIELD_BUFF; i++) // initializes all the pinters
	//memset(fields[i], END_OF_STRING, FIELD_BUFF);

	//memcpy(copy_buffer, bufferDMA, DMA_BUFF_SIZE);

	getMessageFields( bufferDMA, msgSize, (uint8_t*)"GPZDA", fields );

	if ( '\0' == *(fields[TIME]) ) return;

	if ( !updatedTime )
	updatedTime = setTime( hrtc, fields[TIME] );

	if ( '\0' == *(fields[DAY]) ) return;

	if ( !updatedDate )
	updatedDate = setDate( hrtc, fields );

#ifdef DEBUG2
	HAL_UART_Transmit(&huart3, (uint8_t *)"***EXIT updateDateTime***\r\n\r\n",
						strlen("***EXIT updateDateTime***\r\n\r\n"),1000);
	HAL_UART_Transmit(&huart3, bufferDMA, *msgSize,1000);
#endif
}

uint8_t setDate(RTC_HandleTypeDef* hrtc, uint8_t fields[FIELD_BUFF][FIELD_BUFF])
{
	RTC_DateTypeDef Date;
	int aux;

	ascii_to_int(fields[1], &aux);
  	Date.Date = (uint8_t) aux;

  	ascii_to_int(fields[2], &aux);
	Date.Month = (uint8_t) aux;

	ascii_to_int(fields[3]+2, &aux);
	Date.Year = (uint8_t) aux;

	HAL_RTC_SetDate( hrtc, &Date,  RTC_FORMAT_BIN);
	return 1U;
}

uint8_t setTime(RTC_HandleTypeDef* hrtc, uint8_t fields[FIELD_BUFF])
{
	RTC_TimeTypeDef Time;
	int aux;

	fields[6] = '\0';
	ascii_to_int(fields+4, &aux);
	Time.Seconds = (uint8_t) aux;
	fields[4] = '\0';
	ascii_to_int(fields+2, &aux);
	Time.Minutes = (uint8_t) aux;
	fields[2] = '\0';
	ascii_to_int(fields, &aux);
	Time.Hours = (uint8_t) aux - 3;

	HAL_RTC_SetTime(hrtc, &Time,  RTC_FORMAT_BIN);
	return 1U;
}

/**
  * @brief  Config NMEA talkers
  * @retval -
  */
void configGPS()
{
	const uint8_t enable_ZDA [ZDA_PAYLOAD] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x08, 0x01, 0x03, 0x20};
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
	//HAL_UART_Transmit(&huart1, power_save, RXM_PAYLOAD,1000);
}

void GPS_1min_sleep(UART_HandleTypeDef *huart1)
{
	uint8_t UBLOX_GPSStandby[] = {0xB5, 0x62, 0x02, 0x41, 0x08, 0x00, 0x60, 0xEA, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x97, 0xA1};
	HAL_UART_Transmit(huart1, UBLOX_GPSStandby, 16,1000);
}
