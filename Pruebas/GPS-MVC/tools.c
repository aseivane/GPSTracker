/*
 * file.c
 *
 *  Created on: 4 ene. 2021
 *      Author: z0042kvk
 */
#include "tools.h"
#include <string.h>


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

/**
  * @brief  Converts uint8_t in ASCII code to float
  * @param ptrStr pointer to string to convert
  * @param ptrFloat pointer to float converted
  * @retval -
  */
void ascii_to_float(uint8_t* ptrStr, float* ptrFloat)
{
    uint8_t coma;
    uint8_t size;
    uint8_t entero;
    uint8_t* str=ptrStr;

    *ptrFloat = 0;  // cleans the float

    if(str[0] == '-') str++;    // if it's negative, go on, will see it later

    /* checks the length to save the string library */
    for (size = 0; str[size] != '\0'; size++);
    size--;

    for ( coma = 0; str[size-coma] != '.'; coma++)
        *ptrFloat += ( str[size-coma] - ASCII_OFFSET ) * pow_10( coma );
    coma++;

    *ptrFloat = (*ptrFloat)/pow_10(coma-1);

    for ( entero = coma ; (size+1) != entero; entero++)
        *ptrFloat += (str[size-entero] - 48)*pow_10(entero-coma);

    if(ptrStr[0]=='-') *ptrFloat = -*ptrFloat;
}


// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
uint8_t int_to_ascii(int x, char dest[], uint8_t d)
{
    uint8_t digit;
    uint8_t negative = 0;
    char * str;

    if(x == 0)
    {
        if(d)
        {
          uint8_t d_aux = d;
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

    for( uint8_t decimal = 0; decimal < digit; decimal++)
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
