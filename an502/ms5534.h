//---------------------------------------------------------------------------

#ifndef ms5534H
#define ms5534H

#include "interface_ic.h"

long ConvertWtoC5534 (int ix, long W1, long W2, long W3, long W4);
long ConvertCtoW5534 (int ix, long C1, long C2, long C3, long C4, long C5, long C6);
void calcPT5534      (double *pressure, double *temperature, long d1_arg, long d2_arg);


//---------------------------------------------------------------------------
#endif
