#include "parser.h"


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


void  get_fields(uint8_t* line, uint8_t fields_array[FIELD_BUFF][FIELD_BUFF] )
{
	uint8_t i;
	uint8_t* fields[FIELD_BUFF];

	for(i = 0; i<FIELD_BUFF; i++)
		fields[i] = NULL;

	uint8_t field_count = coma_count(line);
	line++; //avanza la primer coma

	for(i = 0; i< field_count; i++)
	{

		fields[i] = line;
		for(line; !( (*line) == COMA || (*line) == END_STAR); line++);
		line++;
	}
	fields[i] = NULL;
/*
	for (i =0; i<FIELD_BUFF; i++)
	{
		memcpy(fields_array[i], "hola", strlen("hola")+1);
		printf("fields_array[%d] %s", i, fields_array[i]);
	}*/

	get_value(fields, field_count , fields_array);
}

uint8_t coma_count(uint8_t* string)
{
	uint8_t count;
	//aumenta el puntero hasta que encuentra una coma. Cuando la encuettra avanza
	//count y se va separando del inicio del puntero. Cuando no encuentra la coma
	//sigue aumentando el puntero hasta lelgar al final.
	for (count=0; string[count] != END_STAR; string[count]==COMA ? count++ : *string++);
	return count;
}

uint8_t* start_sentence_ptr(uint8_t *message)
{
	uint8_t* tok;
	for( tok = message;
			((*tok) != START_PESOS || (*tok) == END_OF_STRING) &&
					(tok - message) < DMA_BUFF_SIZE;
			tok++);
	if((tok-message) == DMA_BUFF_SIZE )
		return NULL;
	else return ++tok;
}

uint8_t is_sentence_complete(uint8_t *message, uint8_t *tok)
{
	uint8_t* aux;
	//checkea que la frase este completa
	for(aux = tok;
			*aux != '\r' && *aux != END_OF_STRING && (aux-message) < DMA_BUFF_SIZE;
			aux++);
	//si llego al final, devuelve NULL
	if((aux-message) == DMA_BUFF_SIZE || *aux == END_OF_STRING)
		return FALSE;
	else return TRUE;

}

uint8_t* get_sentence_ptr(uint8_t *message, const char *type,  uint8_t *init_ptr)
{
	uint8_t* tok;
	uint8_t* aux;
	const char * aux_type;

	if(init_ptr == NULL) tok = message;
	else tok = init_ptr;
	aux_type= type;

	do
	{
	  type = aux_type;
	  if( !(tok = start_sentence_ptr(tok) ) ) return NULL;

	  while((*tok) == (*type))
	  {
		  tok++;
		  type++;
	  }
	}while ((*type) != END_OF_STRING &&
			(tok-message) < DMA_BUFF_SIZE);

	//checkea que haya cortado por coincidencia y no por fin del buffer
	if((*type) != END_OF_STRING) return NULL;

	return tok;

    /*

    if ( !CRC_check(tok))
    {
        //printf("get_sentence CRC incorrecto\r\n");
        return NULL;
    }*/


}
