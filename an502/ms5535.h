//---------------------------------------------------------------------------

#ifndef ms5535H
#define ms5535H

#include "interface_ic.h"

long ConvertWtoC5535 (int ix, long W1, long W2, long W3, long W4);
long ConvertCtoW5535 (int ix, long C1, long C2, long C3, long C4, long C5, long C6);
void calcPT5535      (double *pressure, double *temperature, long d1_arg, long d2_arg);


//---------------------------------------------------------------------------
#endif
