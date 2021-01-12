/*
 * file.c
 *
 *  Created on: 4 ene. 2021
 *      Author: z0042kvk
 */
#include "tools.h"

int pow_10(uint8_t n)
{
    if(!n) return 1;
    n--;
    int power = 10;
    while(n)
    {
        power*=10;
        n--;
    }
    return power;
}

int ascii_to_int(uint8_t *str)
{
    uint8_t negative = 0;
    if(!str) return 0;
    if(*str == '-')
    {
        str++;
        negative = 1;
    }

    int size = strlen(str);
    int result = 0;
    for ( int i = 0 ; i < size; i++)
        result += (str[size-i-1] - 48)*pow_10(i);
    if(negative) return -result;
    return result;
}

void ascii_to_float(uint8_t* ptrstr, float* num)
{
    uint8_t coma;
    uint8_t size;
    uint8_t entero;
    uint8_t* str=ptrstr;

    *num = 0;

    if(str[0] == '-') str++;

    for (size = 0; str[size] != '\0'; size++);
    size--;

    for ( coma = 0; str[size-coma] != '.'; coma++)
        *num += (str[size-coma] - 48)*pow_10(coma);
    coma++;

    *num = (*num)/pow_10(coma-1);

    for ( entero = coma ; (size+1) != entero; entero++)
        *num += (str[size-entero] - 48)*pow_10(entero-coma);

    if(ptrstr[0]=='-') *num = -*num;
}


// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
__uint8_t int_to_ascii(int x, char dest[], __uint8_t d)
{
    __uint8_t digit;
    __uint8_t negative = 0;
    char * str;

    if(x == 0)
    {
        if(d)
        {
          __uint8_t d_aux = d;
          dest[d] = '\0';
          do
          {
            d--;
            dest[d] = '0';
          }while(d);

          return d_aux;
        }
        else
        {
            dest[0] = '0';
          dest[1] = '\0';
          return 1;
        }
    }

    if(x<0)
    {
        negative = 1;
        x = -x;
        str = dest +1;
    }
    else str = dest;

    for(digit = 0 ; x/pow_10(digit); digit++);

    if(d>digit)
    {
       while(d>digit )
       {
        *str = '0';
        str++;
        d--;
       }

    }
    str[digit] = '\0';

    for( __uint8_t decimal = 0; decimal < digit; decimal++)
        str[digit - decimal -1 ] = (x/pow_10(decimal))%pow_10(1) +'0';

    if(negative)
        dest[0] = '-';

    return digit;

}

// Converts a floating-point/double number to a string.
void float_to_ascii(float n, uint8_t* res, uint8_t afterpoint)
{
    if(n<0)
    {
        *res = '-';
        res++;
        n = -n;
    }
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    //if(fpart<0) fpart = -fpart;

    // convert integer part to string
    int i = int_to_ascii(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0) {
        res[i++] = '.'; // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow_10(afterpoint);

        int_to_ascii((int)fpart, res + i, afterpoint);
    }

}

void insert_chars(uint8_t *dest, uint8_t *src, uint8_t d)
{
	if(!dest || !src || *src == '\0' || !d) return;
    for(uint8_t i = 0; i<d; i++)
        dest[i] = src[i];
}

void get_time_string(RTC_TimeTypeDef *Time, uint8_t *time_string)
{
	uint8_t few_chars[3] = "";
	int_to_ascii( (int) Time->Hours , few_chars, 2);
	insert_chars(time_string, few_chars, 2);
	int_to_ascii( (int) Time->Minutes , few_chars, 2);
	insert_chars(time_string+3, few_chars, 2);
	int_to_ascii( (int) Time->Seconds , few_chars, 2);
	insert_chars(time_string+6, few_chars, 2);
}

void get_date_string(RTC_DateTypeDef *Date, uint8_t *date_string)
{
	uint8_t few_chars[3] = "";
	int_to_ascii( (int) Date->Date , few_chars, 2);
	insert_chars(date_string, few_chars, 2);
	int_to_ascii( (int) Date->Month , few_chars, 2);
	insert_chars(date_string+3, few_chars, 2);
	int_to_ascii( (int) Date->Year, few_chars, 2);
	insert_chars(date_string+6, few_chars, 2);
}

/**
  * @brief UBX checksum
  * @param buffer without sync chars 0xB5, 0x62,
  * @retval None
  */
void UBX_checksum(__uint8_t Buffer[], uint8_t *CK_A, uint8_t *CK_B)
{
	uint16_t N = __8BITCAT(Buffer[3], Buffer[2]);
	*CK_A = 0, *CK_B = 0;
	for(int i=0;i<N;i++)
	{
		*CK_A = *CK_A + Buffer[i];
		*CK_B = *CK_B + *CK_A;
	}
}
