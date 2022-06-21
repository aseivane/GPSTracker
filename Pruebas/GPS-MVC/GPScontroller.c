
#include "GPScontroller.h"
#include "GPSmodel.h"
#include "parser.h"

#include "main.h"
#undef DEBUG

/**
  * @brief  Init GPS attributes.
  * @param self pointer to GPSdata object
  * @retval -
  */
void initGPS(GPSdata * _gps)
{
	initGPSmodel(_gps);
}

/**
  * @brief  Update GPS attributes.
  * @param self pointer to GPSdata object
  * @param bufferDMA pointer to UART DMA buffer
  * @retval -
  */
void updateGPS(GPSdata* _gps, uint8_t* bufferDMA)
{
	uint8_t* ptrSentence;
	uint8_t copy_buffer[DMA_BUFF_SIZE];

	uint8_t fields[FIELD_BUFF][FIELD_BUFF];
	for(uint8_t i = 0; i<FIELD_BUFF; i++) // initializes all the pinters
			        memset(fields[i], END_OF_STRING, FIELD_BUFF);

	memcpy(copy_buffer, bufferDMA, DMA_BUFF_SIZE);

	getMessageFields( copy_buffer, "GPGGA", fields );
}