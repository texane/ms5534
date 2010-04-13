//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma hdrstop
#endif __BORLANDC__

#include "ms5534.h"
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif __BORLANDC__




/* ------------------------------------------------------------------------ */
/* --------------------------- ConvertWtoC5534 ---------------------------- */
/* ------------------------------------------------------------------------ */
long ConvertWtoC5534 (int ix, long W1, long W2, long W3, long W4)
{
    long c;
    long x, y;

    c = 0;
    switch(ix)
    {
        case 1:
                    c =  (W1 >> 1) & 0x7FFF;
                    break;
        case 2:
                    x = (W3 << 6) & 0x0FC0;
                    y =  W4       & 0x003F;
                    c = x | y;
                    break;
        case 3:
                    c = (W4 >> 6) & 0x03FF;
                    break;
        case 4:
                    c = (W3 >> 6) & 0x03FF;
                    break;
        case 5:
                    x = (W1 << 10)& 0x0400;
                    y = (W2 >> 6 )& 0x03FF;
                    c = x | y;
                    break;
        case 6:
                    c =  W2       & 0x003F;
                    break;
    }
    return(c);
}


/* ------------------------------------------------------------------------ */
/* --------------------------- ConvertCtoW5534 ---------------------------- */
/* ------------------------------------------------------------------------ */
long ConvertCtoW5534 (int ix, long C1, long C2, long C3, long C4, long C5, long C6)
{
    long w;

    C1 = C1 & 0x7FFF;
    C2 = C2 & 0x0FFF;
    C3 = C3 & 0x03FF;
    C4 = C4 & 0x03FF;
    C5 = C5 & 0x07FF;
    C6 = C6 & 0x003F;

    w = 0;
    switch(ix)
    {
        case 1 :
                    w =  (C1<<1) | (C5 >> 10);
                    break;
        case 2 :
                    w =  ((C5 & 0x3FF) << 6) | C6;
                    break;
        case 3 :
                    w =  (C4 << 6) | (C2 >> 6);
                    break;
        case 4 :
                    w =  (C3 << 6) | (C2 & 0x003F);
                    break;
    }

    w = w & 0xFFFF;
    return(w);
}


/* ------------------------------------------------------------------------ */
/* --------------------------- calcPT5534 --------------------------------- */
/* ------------------------------------------------------------------------ */
void calcPT5534    (double *pressure, double *temperature, long d1_arg, long d2_arg)
{
    double dt, off, sens;
    double fd1, fd2, x;

    d1_arg = d1_arg & 0xFFFF;
    d2_arg = d2_arg & 0xFFFF;

    fd1 = (double) d1_arg;
    fd2 = (double) d2_arg;

    dt          =   fd2 - ((8.0 * fc[5]) + 20224.0);
    off         =   fc[2] * 4.0         + (  (   ( fc[4]-512.0) *  dt ) / 4096.0);
    sens        =   24576.0 +  fc[1]    + (  (     fc[3] *  dt ) / 1024.0);
    x           =   (( sens * (fd1- 7168.0)) / 16384.0) -off;
    if (pressure!=0)
        *pressure    =   250.0 +   x / 32;
    if (temperature!=0)
        *temperature =  20.0 +      ( (  dt * ( fc[6]+50.0) ) / 10240.0);
}
