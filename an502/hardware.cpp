//---------------------------------------------------------------------------

#include <windows.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif __BORLANDC__

#include "ntport.h"
#include "hardware.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif __BORLANDC__


#define LPT1_ADDRESS 0x378

unsigned char reg0;
unsigned char reg2;

/* ================================================================== */
/* ======================    XXXXXXXXXXXXX        =================== */
/* ================================================================== */
 void SetIOPort   (unsigned char port, unsigned char v)
{
    Outport((WORD) LPT1_ADDRESS+port, (WORD) v);
}
/* ================================================================== */
/* ======================    XXXXXXXXXXXXX        =================== */
/* ================================================================== */
 unsigned char GetIOPort   (unsigned char port)
{
    return((unsigned char) Inport((WORD) LPT1_ADDRESS+port));
}

/* ================================================================== */
/* ======================    XXXXXXXXXXXXX        =================== */
/* ================================================================== */
long  sensor_controlInit    ()  // 0 = no error
{
    long error;
    long version;

    reg0=4;
    reg2=0;

    error = 0;

    version = GetNTPortVersion();
    if (version==ERROR_NONE)
        error = 1;

    SetIOPort(0, reg0);
    SetIOPort(2, reg2);

    return(error);
}
/* ================================================================== */
/* ======================    XXXXXXXXXXXXX        =================== */
/* ================================================================== */
void sensor_controlExit    (void)
{
}
/* ================================================================== */
/* ======================    XXXXXXXXXXXXX        =================== */
/* ================================================================== */
 void setSCLK  (bool state)
{
   if (state)
       reg0 = reg0 | 1;
   else
       reg0 = reg0 & 0xFE;
   SetIOPort(0, reg0);
}
// ---------------------------------------------
 bool getSCLK  (void)
{
   if (reg0 & 1)
       return(true);
   else
       return(false);
}
/* ================================================================== */
/* ======================    XXXXXXXXXXXXX        =================== */
/* ================================================================== */
 void setDIN   (bool state)
{
   if (state)
       reg0 = reg0 | 2;
   else
       reg0 = reg0 & 0xFD;
   SetIOPort(0, reg0);
}
// ---------------------------------------------
 bool getDIN(void)
{
   if (reg0 & 2)
       return(true);
   else
       return(false);
}

/* ================================================================== */
/* ======================    XXXXXXXXXXXXX        =================== */
/* ================================================================== */
 bool getDOUT  (void)
{
   unsigned char data;

   data = GetIOPort(1);
   data = data>>5;
   data = data & 1;
   if (data!=0)
       return(true);
   else
       return(false);
}




/* ================================================================== */
/* ======================    XXXXXXXXXXXXX        =================== */
/* ================================================================== */
void   WaitOnePulse(void)
{
    LARGE_INTEGER t0;
    LARGE_INTEGER t1;
    LARGE_INTEGER freq;

    double t0_l_f;
    double t1_l_f;
    double delta_f;
    double freq_f;
    double n_f;

    long  success;
    long  n;

    n = 20;        // Wait for 20 us

    success = QueryPerformanceFrequency(&freq);
    if (success)
        success = QueryPerformanceCounter(&t0);
    t0_l_f  = (double) t0.LowPart;
    freq_f  = (double) freq.LowPart;// [Hz]
    freq_f  = freq_f / 1.0e+6;      // [MHz]
    n_f     = (double) n;

    while (success)
    {
        success = QueryPerformanceCounter(&t1);
        t1_l_f  = (double) t1.LowPart;
        delta_f = (t1_l_f - t0_l_f) / freq_f;
        if (delta_f>=n_f)
            success = 0;
    }
}


