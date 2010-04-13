/*
** Made by fabien le mentec <texane@gmail.com>
** 
** Started on  Tue Apr 13 10:48:27 2010 texane
** Last update Tue Apr 13 13:20:41 2010 texane
*/



#include <pic18fregs.h>
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


/* pinouts */

#define MS5534_TRIS_SCLK TRISAbits.TRISA0
#define MS5534_PIN_SCLK PORTAbits.RA0

#define MS5534_TRIS_DIN TRISAbits.TRISA1
#define MS5534_PIN_DIN PORTAbits.RA1

#define MS5534_TRIS_DOUT TRISAbits.TRISA2
#define MS5534_PIN_DOUT PORTAbits.RA2

static void setup_pinouts(void)
{
  MS5534_TRIS_SCLK = 0;
  MS5534_TRIS_DIN = 0;
  MS5534_TRIS_DOUT = 1;
}


/* wrapping macros */

#define set_sclk_low() MS5534_PIN_SCLK = 0
#define set_sclk_high() MS5534_PIN_SCLK = 1
#define set_din(__n) MS5534_PIN_DIN = __n
#define get_dout() MS5534_PIN_DOUT


static void wait_pulse(void)
{
  /* wait for one pulse to occur */

  /* todo: should wait for 20usecs */

  unsigned int i;

  for (i = 0x1000; i; --i)
    ;
}


static void send_bits(unsigned int bits, unsigned int len)
{
  /* send a bit sequence to the module */

  unsigned int i;

  set_sclk_low();
  wait_pulse();

  for (i = 0; i < len; ++i)
  {
    set_din(bits & 1);

    wait_pulse();
    set_sclk_high();

    wait_pulse();
    set_sclk_low();

    bits >>= 1;
  }
}


static ms5534_word_t read_word(void)
{
  /* assume a word is available from a prior conversion */

  ms5534_word_t w = 0;
  unsigned int i;

  set_sclk_low();
  wait_pulse();

  for (i = 0; i < 16; ++i)
  {
    set_sclk_high();
    wait_pulse();
    set_sclk_low();

    w <<= 1;
    w |= get_dout();

    wait_pulse();
  }

  return w;
}


static void wait_dout_down(void)
{
  int is_high = 0;

  wait_pulse();

  while (is_high)
  {
    is_high = get_dout();
    wait_pulse();
  }
}


static void reset_module(void)
{
  /* reset the module */

  send_bits(0x55, 8);
  send_bits(0x55, 8);
  send_bits(0x00, 5);
}


static void read_prom(ms5534_word_t* prom)
{
  /* read the prom containing factory calibration values */

  /* word1 */
  send_bits(0x57, 8);
  send_bits(0x01, 5);
  send_bits(0x00, 1);
  prom[0] = read_word();

  /* word2 */
  send_bits(0xd7, 8);
  send_bits(0x00, 5);
  send_bits(0x00, 1);
  prom[1] = read_word();

  /* word3 */
  send_bits(0x37, 8);
  send_bits(0x01, 5);
  send_bits(0x00, 1);
  prom[2] = read_word();

  /* word4 */
  send_bits(0xb7, 8);
  send_bits(0x00, 5);
  send_bits(0x00, 1);
  prom[3] = read_word();
}


static void prom_to_coeffs(ms5534_word_t* prom)
{
  /* convert factory blob to coeffs */

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

  setup_pinouts();
  reset_module();

  read_prom(prom);
  prom_to_coeffs(prom);

  return 0;
}


int ms5534_read_d1(ms5534_word_t* w)
{
  /* . master clock (MCLK) is 32khz
     . data are sampled/sent by the ms5534 at the rising edge
     . data are sampled/sent by the uc at the falling edge
   */

  /* . 3 bits high START sequence before each SETUP
     . 3 bits low STOP sequence after each SETUP
     . SETUP select wether a pressure, temperature, or
     calibration data is read
     . in case of reading temp or pressure, the module
     ack the start of the conversion by a low to high
     transition at the dout
     . 2 sclk are required after the ack
     . the sclk is held low by the uc until a high to
     low transition on DOUT indicates an end of conversion
     . the the conversion result can be read by giving
     17 clocks on the sclk pin
     . note it is recommended to reset the module
     before each conversion to avoid losing sync
   */

  int error = 0;

  setup_pinouts();
  reset_module();

  send_bits(0x2f, 8);
  send_bits(0x00, 2);

  if (get_dout() == 0)
    error = 1;

  send_bits(0x00, 2);

  if (error == 0)
  {
    wait_dout_down();
    *w = read_word();
  }

  /* datasheet compliance */
  send_bits(0x00, 1);

  return error;
}


int ms5534_read_d2(ms5534_word_t* w)
{
  int error = 0;

  setup_pinouts();
  reset_module();

  send_bits(0x4f, 8);
  send_bits(0x00, 3);

  if (get_dout() == 0)
    error = 1;

  send_bits(0x00, 1);

  if (error == 0)
  {
    wait_dout_down();
    *w = read_word();
  }

  return error;
}


void ms5534_read_coeffs(ms5534_word_t* c)
{
  /* read the already converted coeffs */

  unsigned int i;

  for (i = 0; i < MS5534_COEFF_COUNT; ++i)
    c[i] = coeffs[i];
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
