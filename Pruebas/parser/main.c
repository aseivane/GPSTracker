#include <stdio.h>
#include <stdint.h> 
#include "parser.h"

uint8_t empty_buff[] = "$GPRMC,183507.00,V,,,,,,,,,,N*75\r\n$GPVTG,,,,,,,,,N*30\r\n$GPGGA,183507.00,,,,,0,00,99.99,,,,,,*6E\r\n$GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30\r\n$GPGSV,2,1,05,14,,,27,17,,,29,19,,,22,28,,,29*7B\r\n$GPGSV,2,2,05,30,,,32*7E\r\n$GPGLL,,,,,183507.00,V,N*42";
uint8_t full_buff[] = "$GPRMC,183819.00,A,3434.53161,S,05825.20382,W,1.615,358.93,200622,,,A*69\r\n$GPVTG,358.93,T,,M,1.615,N,2.990,K,A*38\r\n$GPGGA,183819.00,3434.53161,S,05825.20382,W,1,03,4.60,0.3,M,13.7,M,,*5F\r\n$GPGSA,A,2,19,05,17,,,,,,,,,,4.71,4.60,1.00*09\r\n$GPGSV,2,1,06,05,23,302,27,13,73,229,16,14,56,141,10,17,44,051,27*70\r\n$GPGSV,2,2,06,19,33,026,25,28,,,21*4D\r\n$GPGLL,3434.53161,S,05825.20382,W,183819.00,A,A*6D";
enum FUNCTION {
	COMA_COUNT, START_CHAR, GET_MESSAGE, COMPLETE_SENTENCE,
    MESSAGE_FIELDS};

int main()
{
    enum FUNCTION functionSelect = MESSAGE_FIELDS;
    char str[100];
    uint8_t* aux;
    uint8_t* buff = empty_buff;
    const char talker[] = "GPVTG";

    switch (functionSelect)
    {
        case COMA_COUNT:
            sprintf(str, "***coma_count*** %u\r\n", coma_count(buff));
            printf(str);
            break;

        case START_CHAR:
            aux = findStartChar(buff, buff + 40);
            printf("*** Start sentence found ***\r\n");
            break;

        case GET_MESSAGE:
            aux = getMessageptr(buff, talker, NULL);
            if (aux)
                printf("*** Talker %s found ***\r\n", talker);
            else
                printf("*** Talker %s not found ***\r\n", talker);
            break;
        
        case COMPLETE_SENTENCE:
            aux = getMessageptr( buff, talker, NULL );
            if (!aux) 
            {
                printf("*** Talker %s not found ***\r\n", talker);
                break;
            }
            if ( isSentenceComplete( buff, aux ) )
            {
                printf("***Complete Sentence***\r\n");
            }
            else
            {
                printf("***Incomplete Sentence***\r\n");
            }
            break;

        case MESSAGE_FIELDS:
            uint8_t fields_array[FIELD_BUFF][FIELD_BUFF];
            for(uint8_t i = 0; i<FIELD_BUFF; i++) // initializes all the pinters
		        memset(fields_array[i], END_OF_STRING, FIELD_BUFF);
                
            getMessageFields(buff, talker, fields_array );

            if (fields_array[0][0])
                for(uint8_t i = 0; fields_array[i][0] != '\0' ||
                 fields_array[i+1][0] != '\0';
                 printf("%s\r\n" , fields_array[i++] ) );
        default:
            break;
    }
	return 0;
}