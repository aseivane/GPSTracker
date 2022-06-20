/** @file parser.c
 *  @brief NMEA parser.
 *
 *  This library contains tools for parsing NMEA code
 *  for embedded systems. Insted of chars, it uses uint8_t.
 *
 *  @author Andres Seivane
 *  @bug Sin corroborar stop de los loop segun tamaño de la string
 * o bloque de memoria
 */

#include "parser.h"
#include <stdint.h> 
#include <stdio.h> 

#define WINDOWS 

#ifdef algo
#ifndef strsep

char *strsep (char **stringp, const char *delim)
{
  char *begin, *end;
  begin = *stringp;
  if (begin == NULL)
    return NULL;
  /* Find the end of the token.  */
  end = begin + strcspn (begin, delim);
  if (*end)
    {
      /* Terminate the token and set *STRINGP past NUL character.  */
      *end++ = END_OF_STRING;
      *stringp = end;
    }
  else
    /* No more delimiters; this is the last token.  */
    *stringp = NULL;
  return begin;
}
#endif



uint8_t checksum(const char *s) {
    int c = 0;

    while (*s)
        c ^= *s++;

    return c;
}

uint8_t CRC_check( unsigned char* mystring)
{
    unsigned char *tok = mystring;
    char str_crc[3];

    tok = (unsigned char *)strtok( (char*) tok,END_STAR);

    //printf("CRC_check tok %s\r\n",tok);

    uint8_t crc_calc = checksum((char*) tok);

    sprintf(str_crc, "%x",crc_calc );
    sscanf(str_crc, "%u",&crc_calc );

    //printf("CRC_calc %d\r\n",crc_calc);

    if (! (tok = (unsigned char *) strtok( NULL,END_STAR)) )
        return 0;
    int crc = atoi((char*) tok);
    //printf("CRC %d\r\n",crc);

    if(crc == crc_calc)
    {
        //printf("CRC correcto\r\n");
        return 1;

    }
    else
    {
        //printf("CRC incorrecto\r\n");
        return 0;
    }
    return 0;
}
#endif


void get_value(uint8_t** fields, uint8_t field_count , uint8_t dest[FIELD_BUFF][FIELD_BUFF])
{
	uint8_t index;
	for(index = 0; index<field_count;index++)
	{
		//limpia el buffer
		memset(dest[index], END_OF_STRING, FIELD_BUFF);

		if( fields[index] == NULL) return;

		if (! fields[index+1] ) //si el siguiente es nulo busca hasta el asterisco
		{
			for (uint8_t i=0; fields[index][i] != END_STAR; i++)
				dest[index][i] = fields[index][i];
			return;
		}
		//si no hay valor sale
		if( ( fields[index+1] - fields[index] ) < 1 ) return;
		//copia lo que esta entre las dos comas al buffer
		memcpy(dest[index], fields[index], fields[index+1] - fields[index] -1 );
		dest[index][fields[index+1] - fields[index]] = '\0';//agrega el fin de cadena
	}
}

/** @brief Returns the message fields in a given matrix.
 *
 *  @param ptrMessage pointer to the start of the message.
 *  @param ptrTalker pointer to the talker from wich get the message.
 *  @param fieldsArray matrix to copy the values
 *  @return -
 */
void  getMessageFields(uint8_t* ptrMessage, uint8_t* ptrTalker, uint8_t **fields_array )
{

	uint8_t i;
	/* define an array of pointer to reference where each value 
	* is allocated in the message */
	uint8_t* fields[FIELD_BUFF];
	uint8_t* auxPtrMessage;

	for(i = 0; i<FIELD_BUFF; i++) // initializes all the pinters
		fields[i] = NULL;

	auxPtrMessage = getMessageptr( ptrMessage, ptrTalker, NULL );
	if (NULL == auxPtrMessage) return; // if the talkers is not found, return

	if ( ! isSentenceComplete( ptrMessage, auxPtrMessage ) ) return;
	

	uint8_t field_count = coma_count(auxPtrMessage);
	auxPtrMessage++; //avanza la primer coma

	for(i = 0; i< field_count; i++)
	{
		for(auxPtrMessage; !( (*auxPtrMessage) == COMA || (*auxPtrMessage) == END_STAR); auxPtrMessage++);
		auxPtrMessage++;
		fields[i] = auxPtrMessage;
	}
	fields[i] = NULL;

	//get_value(fields, field_count , fields_array);
}

/** @brief Checks if the message is complete.
 *
 *   Uses ptrStart to checks if there is al \r o \0 in the message.
 *
 *  @param ptrStart pointer to look where to start from
 *  @param ptrMessage pointer to the start of the message.
 *  @return True or False.
 */
uint8_t isSentenceComplete(uint8_t *ptrMessage, uint8_t *ptrStart)
{
	uint8_t* ptrAux;
	//checkea que la frase este completa
	for(ptrAux = ptrStart;
			*ptrAux != '\r' && *ptrAux != END_OF_STRING && (ptrAux-ptrMessage) < DMA_BUFF_SIZE;
			ptrAux++);
	//si llego al final, devuelve NULL
	if((ptrAux-ptrMessage) == DMA_BUFF_SIZE || *ptrAux == END_OF_STRING)
		return FALSE;
	else return TRUE;

}

/** @brief Returns the number of commas in a sentence.
 *
 *  Starts where the pointer "string" is pointing and stops when an "*" is found.
 *
 *  @param string pointer that refers to where is expected tu start looking for.
 *  @return Number of commas.
 */
uint8_t coma_count(uint8_t* ptrMessage)
{
	uint8_t count=0U;
	//aumenta el puntero hasta que encuentra una coma. Cuando la encuetra avanza
	//count y se va separando del inicio del puntero. Cuando no encuentra la coma
	//sigue aumentando el puntero hasta llegar al final.
	for (count=0U; ptrMessage[count] != END_STAR; 
		 ptrMessage[count]==COMA ? count++ : *ptrMessage++);
	return count;
}

/** @brief Returns a pointer to the first aparition of "$" in prtSartBuff.
 *
 *  Starts at prtSartBuff address and runs through the string until the end or until
 *  it fins a match. Whatever comes first.
 *
 *  @param ptrStartBuff pointer that refers to where is expected tu start looking for.
 *  @return ptrAux. NULL if there is no match.
 */
uint8_t* findStartChar( uint8_t* ptrMessage, uint8_t *ptrStart )
{
	uint8_t* ptrAux ;	// aux pointer for moving through the string
	for( ptrAux = ptrStart ;
		( '$' != *ptrAux ) && ( DMA_BUFF_SIZE > (ptrAux - ptrMessage) );
		ptrAux++);	//starts at the begining. Ends if it matches "$" or end of buffer
	
	if( DMA_BUFF_SIZE == ( ptrAux - ptrMessage ) )
		ptrAux= NULL;	// returns NULL if it didn't find a match
	
	return ptrAux;
}

#ifdef WINDOWS
void printTalker(const char *message)
{
	char talker[6];
	memcpy(talker, message , 5);
	printf("Talker %s\r\n", talker);
}
#endif

/** @brief Returns a pointer to the first aparition of type string.
 *
 *  Starts at message address and runs through the string until the end or until
 *  it fins a match. Whatever comes first.
 *
 *  @param message pointer that refers to where is expected tu start looking for.
 *  @param type string to find. NMEA Talker
 *  @return token to start of message. NULL if there is no match.
 */
uint8_t* getMessageptr(uint8_t *message, const char *type,  uint8_t *init_ptr)
{
	uint8_t* tok;
	const char * type_offset;
	uint8_t* tok_offset;

	/* assign aux pointers */
	if ( NULL == init_ptr ) tok = message;
	else tok = init_ptr;


	do
	{
		if( !(tok = findStartChar(message, tok) ) ) return NULL; //looks for next $ ocurrence
		tok++;	// findStartChar return a pointer to $. moves one more 
		#ifdef WINDOWS
		printTalker(tok);
		#endif

		type_offset= type;
		tok_offset = tok;

		/* compares the chars in type string
		 * until the end or a failed match
		 */
		while( ( *tok_offset ) == ( *type_offset )
				&& END_OF_STRING != ( *type_offset ) )
		{
			tok_offset++;	// moves the pointer
			type_offset++;
		}
	/* loop until reach the end of the buffer or 
	 * type_offset point to '\0', which means the type matched
	 * in the message
	 */ 
	}while ( END_OF_STRING != (*type_offset) &&
			DMA_BUFF_SIZE > (tok-message) );

	//checkea que haya cortado por coincidencia y no por fin del buffer
	if((*type_offset) != END_OF_STRING) return NULL;

	return tok;
}