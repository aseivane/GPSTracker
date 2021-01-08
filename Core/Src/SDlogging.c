/*
 * SDlogging.c
 *
 *  Created on: Jan 7, 2021
 *      Author: z0042kvk
 */
#include "fatfs.h"
#include "GPSmodel.h"

FRESULT open_append (
    FIL* fp,            /* [OUT] File object to create */
    const char* path    /* [IN]  File name to be opened */
)
{
    FRESULT fr;

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS);
    if (fr == FR_OK) {
        /* Seek to end of the file to append data */
        fr = f_lseek(fp, f_size(fp));
        if (fr != FR_OK)
            f_close(fp);
    }
    return fr;
}

void send_uart (char *string)
{
	extern UART_HandleTypeDef huart1;
	uint8_t len = strlen (string);
	HAL_UART_Transmit(&huart1, (uint8_t *) string, len, HAL_MAX_DELAY);  // transmit in blocking mode
}

FRESULT log_data()
{
  FRESULT fr;
  FATFS fs;
  FIL fil;
  char log[150];
  RTC_TimeTypeDef Time;
  RTC_DateTypeDef Date;
  extern RTC_HandleTypeDef hrtc;
  //extern GPSdata gps;

	/* Open or create a log file and ready to append */
  fr = f_mount(&fs, "", 0);
	if ( fr != FR_OK )
	{
		send_uart ("ERROR!!! in mounting SD CARD...\r\n");
		return fr;
	}
	else send_uart("SD CARD mounted successfully...\r\n");

	fr = open_append(&fil, "logfile.txt");
	if (fr != FR_OK)
	{
		  send_uart ("ERROR!!! openning file...\r\n");
		  return fr;
	}

	send_uart ("logfile.txt opened\r\n");

	HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);

	//sprintf(log, "%i:%i:%i %i/%i/%i, %f, %f, %d\r\n",Time.Hours,Time.Minutes, Time.Seconds, Date.Date, Date.Month, Date.Year, gps.latitude, gps.longitude, gps.altitude);
	sprintf(log, "%i:%i:%i %i/%i/%i, Latitud, Longitud, Altura\n",Time.Hours,Time.Minutes, Time.Seconds, Date.Date, Date.Month, Date.Year);
	/* Append a line */
	f_printf(&fil, log);

	/* Close the file */
	f_close(&fil);
}
