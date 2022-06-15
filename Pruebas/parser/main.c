#include <stdio.h>
#include <stdint.h> 
#include "parser.h"

uint8_t buff[] = "$GNGGA,,,,,,0,00,25.5,,,,,,*64\r\n$GNGLL,,,,,,V,M*79\r\n$GPGSA,A,1,,,,,,,,,,,,,25.5,25.5,25.5*02\r\n$BDGSA,A,1,,,,,,,,,,,,,25.5,25.5,25.5*13\r\n$GPGSV,1,1,00*79\r\n$BDGSV,1,1,00*68\r\n$GNRMC,,V,,,,,,,,,,M*4E\r\n$GNVTG,,,,,,,,,M*2D\r\n$GNZDA,,,,,,*56\r\n$GPTXT,01,01,01,ANTENNA OK*35\r\n";

enum FUNCTION {
	COMA_COUNT, START_CHAR, GET_MESSAGE};

int main()
{
    enum FUNCTION functionSelect = GET_MESSAGE;
    char str[100];
    uint8_t* aux;
    uint8_t diff=0;

    printf("***BUFFER***\r\n%s\r\n",buff);

    switch (functionSelect)
    {
    case COMA_COUNT:
        sprintf(str, "***coma_count*** %u\r\n", coma_count(buff));
        printf(str);
        break;

    case START_CHAR:
        aux = findStartChar(buff + 40);
        //sprintf(str, "start_sentence_ptr %x\r\n", aux);
        //printf(str);
        printf(aux);

    case GET_MESSAGE:
        aux = getMessageptr(buff, "GNGLL2", NULL);
        //sprintf(str, "start_sentence_ptr %x\r\n", aux);
        //printf(str);
        printf(aux);
    
    default:
        break;
    }
	return 0;
}