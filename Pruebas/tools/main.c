#include <stdio.h>
#include <stdint.h> 

#include "main.h"
#include "tools.h"

uint8_t empty_buff[] = "$GPRMC,183507.00,V,,,,,,,,,,N*75\r\n$GPVTG,,,,,,,,,N*30\r\n$GPGGA,183507.00,,,,,0,00,99.99,,,,,,*6E\r\n$GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30\r\n$GPGSV,2,1,05,14,,,27,17,,,29,19,,,22,28,,,29*7B\r\n$GPGSV,2,2,05,30,,,32*7E\r\n$GPGLL,,,,,183507.00,V,N*42";
uint8_t full_buff[] = "$GPRMC,183819.00,A,3434.53161,S,05825.20382,W,1.615,358.93,200622,,,A*69\r\n$GPVTG,358.93,T,,M,1.615,N,2.990,K,A*38\r\n$GPGGA,183819.00,3434.53161,S,05825.20382,W,1,03,4.60,0.3,M,13.7,M,,*5F\r\n$GPGSA,A,2,19,05,17,,,,,,,,,,4.71,4.60,1.00*09\r\n$GPGSV,2,1,06,05,23,302,27,13,73,229,16,14,56,141,10,17,44,051,27*70\r\n$GPGSV,2,2,06,19,33,026,25,28,,,21*4D\r\n$GPGLL,3434.53161,S,05825.20382,W,183819.00,A,A*6D";



int main()
{
    enum FUNCTION {	POW, ASCII_TO_INT, ASCII_TO_FLOAT};

    enum FUNCTION functionSelect = ASCII_TO_FLOAT;
    switch (functionSelect)
    {
        case POW:
            for (int i = -4; i < 3; i++)
                printf( "10^%i = %i\r\n",i,pow_10(i) );
            break;
        
        case ASCII_TO_INT:
            uint8_t numero_int[] = "-56";
            int resultado_int;
            ascii_to_int(numero_int, &resultado_int);
            printf( "String: %s -> int: %i", numero_int, resultado_int );

        case ASCII_TO_FLOAT:
            uint8_t numero_float[] = "-0.00";
            float resultado_float;
            ascii_to_float(numero_float, &resultado_float); 
            printf( "String: %s -> float: %f", numero_float, resultado_float );
            
    }
            /*
void reverse(char* str, int len);
uint8_t int_to_ascii(int x, char dest[], uint8_t d);
void float_to_ascii(float n, uint8_t* res, uint8_t afterpoint);
void insert_chars(uint8_t *dest, uint8_t *src, uint8_t d);*/
    
	return 0;
}