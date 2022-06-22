#include <stdio.h>
#include <stdint.h> 

#include "main.h"
#include "GPSmodel.h"
#include "GPScontroller.h"

uint8_t empty_buff[] = "$GPRMC,,V,,,,,,,,,,N*53\r\n$GPVTG,,,,,,,,,N*30\r\n$GPGGA,,,,,,0,00,99.99,,,,,,*48\r\n$GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30\r\n$GPGLL,,,,,,V,N*64\r\n$GPZDA,212500.00,,,,00,00*48\r\n";
uint8_t full_buff[] = "$GPGGA,212500.00,3434.51539,S,05825.20176,W,1,05,3.97,41.0,M,13.7,M,,*6C\r\n$GPGSA,A,3,13,02,19,06,11,,,,,,,,13.22,3.97,12.60*06\r\n$GPGSV,2,1,08,02,27,027,33,06,34,082,31,11,24,037,32,13,15,002,22*7E\r\n$GPGSV,2,2,08,19,40,136,30,25,15,261,,28,16,110,26,32,05,216,*74\r\n$GPGLL,3434.51539,S,05825.20176,W,212500.00,A,A*61\r\n$GPZDA,212500.00,22,06,2022,00,00*66\r\n$GPRMC,212501.00,A,3434.51535,S,05825.20196,W,0.034,,220622,,,A*7A\r\n$GPVTG,,T,,M,0.034,N,0.063,K,A*21\r\n"; 
volatile uint8_t* usart_rx_dma_buffer = empty_buff;

volatile GPSdata gps;

int main()
{
    initGPS(&gps);

    printf("Latitud: %f\r\n", gps.latitude);
    printf("Longitud: %f\r\n", gps.longitude);
    printf("Altitud: %u\r\n", gps.altitude);
    printf("Satelites: %u\r\n", gps.satellites_in_view);

    updateGPS(&gps, usart_rx_dma_buffer );

    printf("Latitud: %f\r\n", gps.latitude);
    printf("Longitud: %f\r\n", gps.longitude);
    printf("Altitud: %u\r\n", gps.altitude);
    printf("Satelites: %u\r\n", gps.satellites_in_view);

    updateDateTime( usart_rx_dma_buffer );
    
	return 0;
}