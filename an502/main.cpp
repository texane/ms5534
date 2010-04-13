//---------------------------------------------------------------------------


#include <stdio.h>
#ifdef __BORLANDC__
#pragma hdrstop
#endif __BORLANDC__

#include "ms5534.h"
#include "ms5535.h"

//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma argsused
#endif __BORLANDC__

int main(int argc, char* argv[])
{
    long error;
    long w[5];
    int  i;
    long d1, d2;
    double pressure, temperature;

    error = sensor_controlInit   ();  // 0 = no error
    if (error)
    {
        printf("ERROR : unable to enable hardware.\n");
        printf("Don't forget to install NTPORT\n");
    }
    else
    {
        printf("Sensors data:\n");
        printf("******************\n");
        reset();
        for (i=1; i<=4; i++)
        {
            w[i] = getW(i);
            printf("    W%d = x%04x\n",i,w[i]);
        }

        d1 = getD1(0);
        d2 = getD2(0);

        printf("    D1 = %d\n",d1);
        printf("    D2 = %d\n",d2);

        // -------------------------------------
        printf("\n");
        printf("FOR MS5534 sensors:\n");
        printf("******************\n");
        for (i=1; i<=6; i++)
        {
            fc[i] = (double) ConvertWtoC5534(i, w[1], w[2], w[3], w[4]);
            printf("    fc[%d] = %.2f\n",i,fc[i]);
        }
        calcPT5534(&pressure, &temperature, d1, d2);
        printf("    pressure = %.2f mbar, temperature = %.2f degC\n",pressure, temperature);

        // -------------------------------------
        printf("\n");
        printf("FOR MS5535 sensors:\n");
        printf("******************\n");
        for (i=1; i<=6; i++)
        {
            fc[i] = (double) ConvertWtoC5535(i, w[1], w[2], w[3], w[4]);
            printf("    fc[%d] = %.2f\n",i, fc[i]);
        }
        calcPT5535(&pressure, &temperature, d1, d2);
        printf("    pressure = %.2f mbar, temperature = %.2f degC\n",pressure, temperature);

    }
    sensor_controlExit();

    return 0;
}
//---------------------------------------------------------------------------
