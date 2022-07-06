/*
 * SDlogging.c
 *
 *  Created on: Jan 7, 2021
 *      Author: z0042kvk
 */
#include "SDlogging.h"



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

#ifdef DEBUG
void send_uart (char *string)
{
	extern UART_HandleTypeDef huart3;
	uint8_t len = strlen (string);
	HAL_UART_Transmit(&huart3, (uint8_t *) string, len, HAL_MAX_DELAY);  // transmit in blocking mode
}
#endif

FRESULT log_data( GPSdata *_gps, RTC_HandleTypeDef * _hrtc )
{
  FRESULT fr;
  FATFS fs = {0};
  FIL fil = {0};
  uint8_t time_string[TIME_STRING_SIZE] = "  :  :  ,";
  uint8_t date_string[DATE_STRING_SIZE] = "  /  /  ,";
  uint8_t aux[FLOAT_STRING_SIZE] = "";
  RTC_TimeTypeDef Time = {0};
  RTC_DateTypeDef Date = {0};


	/* Open or create a log file and ready to append */
  if (!fs.id)
  {
    fr = f_mount(&fs, "", 0);
    if ( fr != FR_OK )
    {
      //send_uart ("ERROR!!! in mounting SD CARD...\r\n");
      return fr;
    }
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
	f_puts(date_string, &fil);
	f_puts(time_string, &fil);

	float_to_ascii(_gps->latitude , aux, 6) ;
	f_puts(aux, &fil);
	f_puts(",",&fil);
	//send_uart (aux);

	float_to_ascii(_gps->longitude , aux, 6) ;
	f_puts(aux, &fil);
	//send_uart (aux);
	f_puts("\n", &fil);
	f_close(&fil);
	return FR_OK;
}
