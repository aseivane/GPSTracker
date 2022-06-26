/*
 * SDlogging.c
 *
 *  Created on: Jan 7, 2021
 *      Author: z0042kvk
 */
#include "fatfs.h"
#include "GPS/GPSmodel.h"
#include <string.h>

#define SIGN_CHAR 1
#define INTEGER_CHARS 3
#define POINT_CHAR 1
#define APTERPOINT_CHARS 6
#define FLOAT_STRING_SIZE ( SIGN_CHAR + INTEGER_CHARS +\
							POINT_CHAR + APTERPOINT_CHARS +1)

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

FRESULT log_data( GPSdata *_gps, RTC_HandleTypeDef * _hrtc )
{
  FRESULT fr;
  FATFS fs;
  FIL fil;
  uint8_t time_string[11] = "  :  :   ,";
  uint8_t date_string[10] = "  /  /  ";
  uint8_t aux[10] = ",        ";
  uint8_t aux1[13] = ",          \n";
  RTC_TimeTypeDef Time;
  RTC_DateTypeDef Date;

	/* Open or create a log file and ready to append */
  fr = f_mount(&fs, "", 0);
  if ( fr != FR_OK )
  {
		//send_uart ("ERROR!!! in mounting SD CARD...\r\n");
		return fr;
  }
  //else send_uart("SD CARD mounted successfully...\r\n");

  fr = open_append(&fil, "logfile.txt");
  if (fr != FR_OK)
  {
		 // send_uart ("ERROR!!! openning file...\r\n");
		  return fr;
  }

	//send_uart ("logfile.txt opened\r\n");

	HAL_RTC_GetTime(_hrtc, &Time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(_hrtc, &Date, RTC_FORMAT_BIN);

	get_time_string(&Time, time_string);
	get_date_string(&Date, date_string);
	/* Append a line */
	f_printf(&fil, time_string);
	f_printf(&fil, date_string);
	float_to_ascii(_gps->latitude , aux+1, 6) ;
	f_printf(&fil, aux);
	float_to_ascii(_gps->longitude , aux1+1, 6) ;
	//aux1[11] = '\n';
	f_printf(&fil, aux1);
	f_printf(&fil, "\n");
	/* Close the file */
	f_close(&fil);
	return FR_OK;
}
