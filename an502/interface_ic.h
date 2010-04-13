//---------------------------------------------------------------------------

#ifndef interface_icH
#define interface_icH

#include "hardware.h"

extern double fc[7];


void reset (void);       // send a reset sequence to the IC
long getW  (long index); // Read the corresponding calibration word of the IC (index [1:4])
long getD1 (long *error_pt); // Start a D1 acquisition, wait for end of conversion and
                             // return the value
long getD2 (long *error_pt); // Start a D2 acquisition, wait for end of conversion and 
                             // return the value



//---------------------------------------------------------------------------
#endif
