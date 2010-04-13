/*
** Made by fabien le mentec <texane@gmail.com>
** 
** Started on  Tue Apr 13 10:48:27 2010 texane
** Last update Tue Apr 13 11:45:02 2010 texane
*/



#include "ms5534.h"



/* global coefficients */

#define MS5534_COEFF_COUNT 6

#define MS5534_COEFF_SENST1 0
#define MS5534_COEFF_OFFT1 1
#define MS5534_COEFF_TCS 2
#define MS5534_COEFF_TCO 3
#define MS5534_COEFF_TREF 4
#define MS5534_COEFF_TEMPSENS 5

static ms5534_word_t coeffs[MS5534_COEFF_COUNT];


static int reset(void)
{
  /* reset the module */

  return 0;
}


static int read_prom(ms5534_word_t* prom)
{
  /* assume module has been reseted */

  return 0;
}


static void prom_to_coeffs(ms5534_word_t* prom)
{
  coeffs[0] = (prom[0] >> 1) & 0x7fff;

  coeffs[1] = (prom[2] << 6) & 0x0fc0;
  coeffs[1] |= prom[3] & 0x003f;

  coeffs[2] = (prom[3] >> 6) & 0x03ff;

  coeffs[3] = (prom[2] >> 6) & 0x03ff;

  coeffs[4] = (prom[0] << 10) & 0x0400;
  coeffs[4] |= (prom[1] >> 6) & 0x03ff;

  coeffs[5] = prom[1] & 0x003f;
}



/* exported */

int ms5534_initialize(void)
{
  ms5534_word_t prom[4];

  reset();
  read_prom(prom);

  prom_to_coeffs(prom);

  return 0;
}


int ms5534_read_uint16(ms5534_word_t* w)
{
  return 0;
}


#if 0 /* unused */

void ms5534_compute_press_and_temp
(
 double* press, double* temp,
 ms5534_word_t d1, ms5534_word_t d2
)
{
  /* actual temp and press according to calibration data */

  double dt, off, sens;
  double fd1, fd2, x;

  fd1 = (double)d1_arg;
  fd2 = (double)d2_arg;

  dt = fd2 - ((8.0 * coeffs[4]) + 20224.0);
  off = coeffs[1] * 4.0 + (((coeffs[3] - 512.0) * dt ) / 4096.0);
  sens = 24576.0 + coeffs[0] + ((coeffs[2] * dt ) / 1024.0);
  x = ((sens * (fd1- 7168.0)) / 16384.0) - off;

  *press = 250.0 + x / 32;
  *temp = 20.0 + (( dt * (coeffs[5] + 50.0)) / 10240.0);
}

#endif /* unused */
