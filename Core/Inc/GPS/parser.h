/*
 * parser.h
 *
 *  Created on: 24 dic. 2020
 *      Author: z0042kvk
 */

#ifndef PARSER_H_
#define PARSER_H_

#define START_PESOS '$'
#define END_STAR	'*'

#ifndef END_OF_STRING
#define END_OF_STRING '\0'
#endif

#define COMA ','
#define FIELD_BUFF 20
#define NEXT 1

#ifndef DMA_BUFF_SIZE
#define DMA_BUFF_SIZE 267
#endif

#ifndef TRUE
#define TRUE 1U
#endif

#ifndef FALSE
#define FALSE 0U
#endif

#include <stdint.h> //para tener uin8_t en C
#include <stdlib.h> // para tener malloc
#include <string.h> ///fucnioes de str y mem

void copyValues(uint8_t** fields, uint8_t field_count , uint8_t dest[FIELD_BUFF][FIELD_BUFF]);
void  getMessageFields(uint8_t* ptrMessage, uint16_t* msgSize,uint8_t* ptrTalker, uint8_t fields_array[FIELD_BUFF][FIELD_BUFF] );
uint8_t coma_count(uint8_t* string);
uint8_t* findStartChar( uint8_t* ptrMessage, uint16_t* msgSize,uint8_t *ptrStart );
uint8_t isSentenceComplete(uint8_t *ptrMessage, uint16_t* msgSize,uint8_t *ptrStart);
uint8_t* getMessageptr(uint8_t *message, uint16_t* msgSize, const uint8_t *type,  uint8_t *init_ptr);

#endif /* PARSER_H_ */
