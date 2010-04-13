/*
** Made by fabien le mentec <texane@gmail.com>
** 
** Started on  Tue Apr 13 10:48:35 2010 texane
** Last update Tue Apr 13 11:45:52 2010 texane
*/


#ifndef MS5534_H_INCLUDED
# define MS5534_H_INCLUDED



#include "inttypes.h"


typedef uint16_t ms5534_word_t;


typedef struct ms5534_prom
{
  ms5534_word_t word1;
  ms5534_word_t word2;
  ms5534_word_t word3;
  ms5534_word_t word4;
} ms5534_prom_t;


typedef struct ms5534_context
{
  ms5534_prom_t prom;
} ms5534_context_t;


int ms5534_initialize(void);
int ms5534_read_uint16(ms5534_word_t*);



#endif /* ! MS5534_H_INCLUDED */
