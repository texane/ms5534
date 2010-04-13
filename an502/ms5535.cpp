//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma hdrstop
#endif __BORLANDC__

#include "ms5535.h"
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif __BORLANDC__


/* ------------------------------------------------------------------------ */
/* --------------------------- ConvertWtoC5535 ---------------------------- */
/* ------------------------------------------------------------------------ */
long ConvertWtoC5535 (int ix, long W1, long W2, long W3, long W4)
{
    long c;
    long x, y;

    c = 0;
    switch(ix)
    {
        case 1:
                    c =  (W1 >> 3) & 0x1FFF;
                    break;
        case 2:
                    x = (W1 <<10) & 0x1C00;
                    y = (W2 >> 6) & 0x03FF;
                    c =  x | y;
                    break;
        case 3:
                    c = (W3 >> 6) & 0x03FF;
                    break;
        case 4:
                    c = (W4 >> 7) & 0x01FF;
                    break;
        case 5:
                    x = (W2 << 6 )& 0x0FC0;
                    y =  W3       & 0x003F;
                    c =  x | y;
                    break;
        case 6:
                    c = W4 & 0x007F;
                    break;
    }
    return(c);
}

/* ------------------------------------------------------------------------ */
/* --------------------------- ConvertCtoW5535 ---------------------------- */
/* ------------------------------------------------------------------------ */
long ConvertCtoW5535 (int ix, long C1, long C2, long C3, long C4, long C5, long C6)
{
    long w;

    C1 = C1 & 0x1FFF;
    C2 = C2 & 0x1FFF;
    C3 = C3 & 0x03FF;
    C4 = C4 & 0x01FF;
    C5 = C5 & 0x0FFF;
    C6 = C6 & 0x007F;

    w = 0;
    switch(ix)
    {
        case 1 :
                    w =  ( C1<<3) | (C2 >> 10);
                    break;
        case 2 :
                    w =  ((C2 & 0x3FF) << 6) | ((C5>>6) & 0x3F);
                    break;
        case 3 :
                    w =  ( C3 << 6) | (C5 & 0x3F);
                    break;
        case 4 :
                    w =  ( C4 << 7) | C6;
                    break;
    }

    w = w & 0xFFFF;
    return(w);
}

/* ------------------------------------------------------------------------ */
/* --------------------------- calcPT5535 --------------------------------- */
/* ------------------------------------------------------------------------ */
void calcPT5535    (double *pressure, double *temperature, long d1_arg, long d2_arg)
{
    double dt, off, sens;
    double fd1, fd2;

    d1_arg = d1_arg & 0xFFFF;
    d2_arg = d2_arg & 0xFFFF;

    fd1 = (double) d1_arg;
    fd2 = (double) d2_arg;

    dt          = -10000.0 +  fd2 - (8.0 * fc[5]);
    off         =  10000.0 +  fc[2]        + (  (   ( fc[4]-250.0) *  dt ) / 4096.0);
    sens        =   3000.0 + (fc[1] / 2.0) + (  (   ( fc[3]+200.0) *  dt ) / 8192.0);
    if (pressure!=0)
        *pressure    =   1000.0 +    (( sens * (fd1- off)) / 4096.0);
    if (temperature!=0)
        *temperature =  ( 200   +    ((  dt * ( fc[6]+100.0) ) / 2048.0)) / 10.0;
}
