#include <stdio.h>
#include <stdint.h> 

#include "main.h"
#include "GPSmodel.h"
#include "GPScontroller.h"

uint8_t empty_buff[] = "$GPRMC,183507.00,V,,,,,,,,,,N*75\r\n$GPVTG,,,,,,,,,N*30\r\n$GPGGA,183507.00,,,,,0,00,99.99,,,,,,*6E\r\n$GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30\r\n$GPGSV,2,1,05,14,,,27,17,,,29,19,,,22,28,,,29*7B\r\n$GPGSV,2,2,05,30,,,32*7E\r\n$GPGLL,,,,,183507.00,V,N*42";
uint8_t full_buff[] = "$GPRMC,183819.00,A,3434.53161,S,05825.20382,W,1.615,358.93,200622,,,A*69\r\n$GPVTG,358.93,T,,M,1.615,N,2.990,K,A*38\r\n$GPGGA,183819.00,3434.53161,S,05825.20382,W,1,03,4.60,0.3,M,13.7,M,,*5F\r\n$GPGSA,A,2,19,05,17,,,,,,,,,,4.71,4.60,1.00*09\r\n$GPGSV,2,1,06,05,23,302,27,13,73,229,16,14,56,141,10,17,44,051,27*70\r\n$GPGSV,2,2,06,19,33,026,25,28,,,21*4D\r\n$GPGLL,3434.53161,S,05825.20382,W,183819.00,A,A*6D";

volatile uint8_t* usart_rx_dma_buffer = full_buff;

volatile GPSdata gps;

int main()
{
    initGPS(&gps);

    printf("Latitud: %f\r\n", gps.latitude);
    printf("Longitud: %f\r\n", gps.longitude);
    printf("Altitud: %u\r\n", gps.altitude);
    printf("Satelites: %u\r\n", gps.satellites_in_view);

    updateGPS(&gps, usart_rx_dma_buffer );
    
	return 0;
}