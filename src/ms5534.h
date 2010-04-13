/*
** Made by fabien le mentec <texane@gmail.com>
** 
** Started on  Tue Apr 13 10:48:35 2010 texane
** Last update Tue Apr 13 13:20:45 2010 texane
*/


#ifndef MS5534_H_INCLUDED
# define MS5534_H_INCLUDED



#include "inttypes.h"


typedef uint16_t ms5534_word_t;


int ms5534_initialize(void);
int ms5534_read_d1(ms5534_word_t*);
int ms5534_read_d2(ms5534_word_t*);
void ms5534_read_coeffs(ms5534_word_t*);



#endif /* ! MS5534_H_INCLUDED */
