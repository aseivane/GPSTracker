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

void ascii_to_int(uint8_t *str, int *result)
{
    uint8_t negative = 0;
    /* if it's empty returns 0 */
    if(!str || (*str) == END_OF_STRING) 
    {
        (*result) = 0;
        return ;
    }

    /* checks if it's negative*/
    if(*str == '-')
    {
        str++;
        negative = 1;
    }

    /*calculates the length of the string */
    uint8_t size = myStrlen(str);
    (*result) = 0;


    for ( uint8_t i = 0 ; i < size; i++)
        (*result) += (str[size-i-1] - ASCII_OFFSET)*pow_10(i);

    if(negative) (*result) = -(*result);
}

/**
  * @brief  Return the length of a string.
  * It has no checking
  * @param ptrStr pointer to string to convert
  * @retval strlen(ptrStr)
  */
uint8_t myStrlen(const uint8_t *ptrStr)
{
    if(!ptrStr) return 0;

    uint8_t size;
    for (size = 0; ptrStr[size] != END_OF_STRING; size++);
    return size;
}

/**
  * @brief  Converts uint8_t in ASCII code to float
  * @param ptrStr pointer to string to convert
  * @param ptrFloat pointer to float converted
  * @retval -
  */
void ascii_to_float(const uint8_t* ptrStr, float* ptrFloat)
{
    uint8_t coma;
    uint8_t size;
    uint8_t entero;
    const uint8_t* str=ptrStr;

    *ptrFloat = 0;  // cleans the float

    if(str[0] == '-') str++;    // if it's negative, go on, will see it later

    /* checks the length to save the string library */
    size = myStrlen(str);

    /* makes all the number after the point an integer in ptrFloat*/
    for ( coma = 1; str[size-coma] != '.'; coma++)
        *ptrFloat += ( str[size-coma] - ASCII_OFFSET ) * pow_10( coma -1 );
    
    *ptrFloat = (*ptrFloat)/pow_10(coma-1); //moves all the numbers after the coma
    coma++;

    /* makes all the number before the point an integer in ptrFloat*/
    for ( entero = coma ; (size+1) != entero; entero++)
        *ptrFloat += (str[size-entero] - ASCII_OFFSET)*pow_10(entero-coma);

    /* checks if its negative */
    if(ptrStr[0]=='-') *ptrFloat = -(*ptrFloat);
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
 /*
void UBX_checksum(__uint8_t Buffer[], uint8_t *CK_A, uint8_t *CK_B)
{
	uint16_t N = __8BITCAT(Buffer[3], Buffer[2]);
	*CK_A = 0, *CK_B = 0;
	for(int i=0;i<N;i++)
	{
		*CK_A = *CK_A + Buffer[i];
		*CK_B = *CK_B + *CK_A;
	}
}*/
