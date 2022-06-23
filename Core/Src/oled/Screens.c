/*
 * Screens.c
 *
 *  Created on: Dec 30, 2020
 *      Author: z0042kvk
 */
#include <OLED/Screens.h>


extern I2C_HandleTypeDef hi2c1;
extern u8g2_t u8g2;
extern RTC_HandleTypeDef hrtc;
extern uint8_t screen_number;

uint8_t u8x8_byte_stm32hal_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	static uint8_t buffer[32];		/* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
	static uint8_t buf_idx;
	uint8_t *data;


  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
    {
        data = (uint8_t *)arg_ptr;
        while( arg_int > 0 )
        {
			buffer[buf_idx++] = *data;
			data++;
			arg_int--;
        }
    }
    	break;
    case U8X8_MSG_BYTE_INIT:
    	break;
    case U8X8_MSG_BYTE_SET_DC:
    	break;
    case U8X8_MSG_BYTE_START_TRANSFER:
		{
			buf_idx = 0;
		}
		break;
    case U8X8_MSG_BYTE_END_TRANSFER:
	{
		uint8_t iaddress = I2C_ADDRESS;
		HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)iaddress<<1, &buffer[0], buf_idx, 20u);
		volatile uint32_t i;
		for (i = 1; i <= 500; i++);
	}
    	break;
    default:
      return 0;
  }
  return 1;
}

uint8_t psoc_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      /* only support for software I2C*/

      break;
    case U8X8_MSG_DELAY_NANO:
      /* not required for SW I2C */
    {
		volatile uint32_t i;
		for (i = 1; i <= arg_int*10; i++);
    }
      break;

    case U8X8_MSG_DELAY_10MICRO:
      /* not used at the moment */
      break;

    case U8X8_MSG_DELAY_100NANO:
      /* not used at the moment */
      break;

    case U8X8_MSG_DELAY_MILLI:
      HAL_Delay(arg_int);
      break;
    case U8X8_MSG_DELAY_I2C:
      /* arg_int is 1 or 4: 100KHz (5us) or 400KHz (1.25us) */
      //delay_micro_seconds(arg_int<=2?5:1);
      break;

    case U8X8_MSG_GPIO_I2C_CLOCK:

      break;
/*
    case U8X8_MSG_GPIO_MENU_SELECT:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_SELECT_PORT, KEY_SELECT_PIN));
      break;
    case U8X8_MSG_GPIO_MENU_NEXT:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_NEXT_PORT, KEY_NEXT_PIN));
      break;
    case U8X8_MSG_GPIO_MENU_PREV:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_PREV_PORT, KEY_PREV_PIN));
      break;
    case U8X8_MSG_GPIO_MENU_HOME:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_HOME_PORT, KEY_HOME_PIN));
      break;
*/
    default:
      u8x8_SetGPIOResult(u8x8, 1);
      break;
  }
  return 1;
}


uint8_t draw_button_line(u8g2_t *u8g2, u8g2_uint_t y, u8g2_uint_t w, uint8_t cursor, const char *s)
{
  u8g2_uint_t button_line_width;

  uint8_t i;
  uint8_t cnt;
  uint8_t is_invert;

  u8g2_uint_t d;
  u8g2_uint_t x;

  cnt = u8x8_GetStringLineCnt(s);


  /* calculate the width of the button line */
  button_line_width = 0;
  for( i = 0; i < cnt; i++ )
  {
    button_line_width += u8g2_GetUTF8Width(u8g2, u8x8_GetStringLineStart(i, s));
  }
  button_line_width += (cnt-1)*SPACE_BETWEEN_BUTTONS_IN_PIXEL;	/* add some space between the buttons */

  /* calculate the left offset */
  d = 0;
  if ( button_line_width < w )
  {
    d = w;
    d -= button_line_width;
    d /= 2;
  }

  /* draw the buttons */
  x = d;
  for( i = 0; i < cnt; i++ )
  {
    is_invert = 0;
    if ( i == cursor )
      is_invert = 1;

    u8g2_DrawUTF8Line(u8g2, x, y, 0, u8x8_GetStringLineStart(i, s), 1, is_invert);
    x += u8g2_GetUTF8Width(u8g2, u8x8_GetStringLineStart(i, s));
    x += SPACE_BETWEEN_BUTTONS_IN_PIXEL;
  }

  /* return the number of buttons */
  return cnt;
}

/**
  * @brief  Init and setup Sreen
  * @retval -
  */
void initScreen(u8g2_t* u8g2)
{
	  u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2,
			  U8G2_R0,
			  u8x8_byte_stm32hal_hw_i2c,
			  psoc_gpio_and_delay_cb);	// Configure u8g2 type
	  u8g2_SetI2CAddress(u8g2, I2C_ADDRESS*2);	// Configure i2c address
	  u8g2_InitDisplay(u8g2);
	  u8g2_ClearDisplay(u8g2);	// Clean screen for fist use
	  u8g2_SetPowerSave(u8g2, 0U);	// Disables Energy save

	  screen_number = 1;	// set home screen
}

/**
  * @brief  Updates the screen according the actual screen
  * @retval -
  */
void updateScreen()
{
	switch(screen_number)
	{
	  case SCREEN_TIME:
		  draw_screen_time();
		  break;
	  case SCREEN_UBIC:
		  draw_screen_ubic();
		  break;
	  case SCREEN_ALTITUDE:
		  //draw_screen_altitude();
		  break;
	  case SCREEN_ODOMETER:
		  //draw_screen_odometer();
		  break;
	}
	draw_button_line(&u8g2, u8g2_GetDisplayHeight(&u8g2)-BUTTON_OFFSET, u8g2_GetDisplayWidth(&u8g2), 3, BUTTONS);

	u8g2_SendBuffer(&u8g2);
}
void draw_screen_time()
{
	RTC_TimeTypeDef Time;
	RTC_DateTypeDef Date;
	uint8_t time_string[9] = "  :  :  ";
	uint8_t date_string[9] = "  /  /  ";
	uint8_t y=11;
	uint8_t x=0;

	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFontMode(&u8g2, 1);	// Transparent
	u8g2_SetFont(&u8g2, u8g2_font_t0_11_tf );

	HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);

	get_time_string(&Time, time_string);

	u8g2_DrawStr(&u8g2, x, y, time_string);

	get_date_string(&Date, date_string);
	x+=u8g2_GetStrWidth(&u8g2,time_string) + 5 ;

	u8g2_DrawStr(&u8g2, x, y, date_string);
}

void draw_screen_ubic()
{
	extern GPSdata gps;
	uint8_t f_to_char[10];
	uint8_t y=11;
	uint8_t x=0;

	memset(f_to_char, '\0',10);

	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFontMode(&u8g2, 1);	// Transparent
	u8g2_SetFontDirection(&u8g2, 0);
	u8g2_SetFont(&u8g2, u8g2_font_t0_11_tf );
    u8g2_DrawStr(&u8g2, x, y, "Lat: ");
    x+=u8g2_GetStrWidth(&u8g2,"Lat: ");
    float_to_ascii(gps.latitude , f_to_char, 6) ;
    u8g2_DrawStr(&u8g2, x, y, f_to_char);

    memset(f_to_char, '\0',10);
    x=0;
    y=2*y;
    u8g2_DrawStr(&u8g2, x, y, "Lon: ");
	x+=u8g2_GetStrWidth(&u8g2,"Lon: ");
	float_to_ascii(gps.longitude , f_to_char, 6) ;
    u8g2_DrawStr(&u8g2, x, y, f_to_char);
}

/*
void test_screen()
{
	RTC_TimeTypeDef Time;
	RTC_DateTypeDef Date;
	uint8_t f_to_char[100];
	uint8_t y=11;
	uint8_t x=0;

	u8g2_SetDrawColor(&u8g2, 1);
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFontMode(&u8g2, 1);	// Transparent
	u8g2_SetFontDirection(&u8g2, 0);
	u8g2_SetFont(&u8g2, u8g2_font_t0_11_tf );
    u8g2_DrawStr(&u8g2, x, y, "Lat: ");
    x+=strlen("Lat: ")*6;
    //sprintf(f_to_char, "%f",gps.latitude );
    //u8g2_DrawStr(&u8g2, x, y, f_to_char);

    x=0;
    y=2*y;
    u8g2_DrawStr(&u8g2, x, y, "Lon: ");
	x+=strlen("Lon: ")*6;
    //sprintf(f_to_char, "%f",gps.longitude );
    //u8g2_DrawStr(&u8g2, x, y, f_to_char);

    x=0;
	y+=11;
	HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);
	//sprintf(f_to_char, "%i:%i:%i %i/%i/%i\r\n",Time.Hours,Time.Minutes, Time.Seconds, Date.Date, Date.Month, Date.Year);
	u8g2_DrawStr(&u8g2, x, y, f_to_char);

	u8g2_draw_button_line(&u8g2, u8g2_GetDisplayHeight(&u8g2)-2, u8g2_GetDisplayWidth(&u8g2), 3, BUTTONS);

	u8g2_SendBuffer(&u8g2);
}*/

