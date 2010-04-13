/*
** Made by fabien le mentec <texane@gmail.com>
** 
** Started on  Thu Nov 19 20:03:19 2009 texane
** Last update Tue Apr 13 10:51:18 2010 texane
*/



#pragma stack 0x200 255

#include <pic18fregs.h>



static void initialize(void)
{
  ADCON1 = 0x0F;
  CMCON = 0x07;
}


#if 0

static void switch_led(void)
{
  static unsigned int ledv = 0;
  static unsigned int ledc = 0;

  ++ledc;

  if (ledc == 0x1000)
  {
    ledv ^= 1;
    ledc = 0;

    TRISAbits.TRISA0 = 0;
    PORTAbits.RA0 = ledv;
  }
}

#endif


void main(void)
{
  float temp = 0.f;

  initialize();

  while(1)
  {
  }
}
