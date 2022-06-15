#include <stdio.h>
#include <stdint.h> 
#include "parser.h"

uint8_t buff[] = "$GNGGA,,,,,,0,00,25.5,,,,,,*64\r\n$GNGLL,,,,,,V,M*79\r\n$GPGSA,A,1,,,,,,,,,,,,,25.5,25.5,25.5*02\r\n$BDGSA,A,1,,,,,,,,,,,,,25.5,25.5,25.5*13\r\n$GPGSV,1,1,00*79\r\n$BDGSV,1,1,00*68\r\n$GNRMC,,V,,,,,,,,,,M*4E\r\n$GNVTG,,,,,,,,,M*2D\r\n$GNZDA,,,,,,*56\r\n$GPTXT,01,01,01,ANTENNA OK*35\r\n";

int main()
{
    char str[100];
    uint8_t* aux;
    uint8_t diff=0;
	
    sprintf(str, "coma_count %u\r\n", coma_count(buff));
    printf(str);

    aux = start_sentence_ptr(buff, (uint8_t *)"GNVTG");
    sprintf(str, "start_sentence_ptr %x\r\n", aux);
    printf(str);
    //diff = buff - aux;
    //sprintf(str, "distance from start: %d, %c\r\n", diff, *(aux+1));
    printf(aux);
    
	return 0;
}