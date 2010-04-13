//---------------------------------------------------------------------------

#include <time.h>


#ifdef __BORLANDC__
#pragma hdrstop
#endif __BORLANDC__


#include "interface_ic.h"
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif __BORLANDC__


double fc[7];


long waitOnDoutFall     (void);    // wait until a falling edge on DOUT is detected
long SerialGet16        (void);    // shift in (from IC to PC) a 16 bit value
void SerialSendLsbFirst (char pattern, char nbr_clock);  // shift out (from PC to IC) a
                                   // sequence of bits



/* ===================================================================== */
/*                           waitOnDoutFall                              */
/* ===================================================================== */
long waitOnDoutFall(void)
{
    bool     working;
    long     cnt;
    unsigned long t0;
    unsigned long t1;
    long     error;

    working = true;
    error   = 0;

    WaitOnePulse();

    t0  = (unsigned long) time(0);
    cnt = 0;

    while(working)
    {
        working = getDOUT();
        cnt++;
        WaitOnePulse();
        if (cnt>=100)
        {
            t1 = (unsigned long) time(0);
            if ((t1-t0)>1)
            {
                working = false;
                error   = 1;
            }
            cnt = 0;
        }
    };

    return(error);
}


/* ===================================================================== */
/*                           SerialGet16                                 */
/* ===================================================================== */
long SerialGet16(void)
{
    char i;
    long v;

    v = 0;
    setSCLK(false);
    WaitOnePulse();

    for (i=0; i<16; i++)
    {
        setSCLK(true);
        WaitOnePulse();
        setSCLK(false);
        v = v << 1;
        if (getDOUT())
            v = v | 1;
        WaitOnePulse();
    }
    return(v);
}


/* ===================================================================== */
/*                           SerialSendLsbFirst                          */
/* ===================================================================== */
void SerialSendLsbFirst(char pattern, char nbr_clock)
{
    char i;
    char c;

    setSCLK(false);
    WaitOnePulse();
    for (i=0; i<nbr_clock; i++)
    {
        c = (char) (pattern & 1);
        if (c==1)
            setDIN(true);
        else
            setDIN(false);
        WaitOnePulse();
        setSCLK(true);
        WaitOnePulse();
        setSCLK(false);
        pattern = (char) (pattern >> 1);
    }
}











/* ===================================================================== */
/*                           reset                                       */
/* ===================================================================== */
void reset(void)
{
    SerialSendLsbFirst(0x55, 8);
    SerialSendLsbFirst(0x55, 8);
    SerialSendLsbFirst(0x00, 5);
}

/* ===================================================================== */
/*                                getW                                   */
/* ===================================================================== */
long getW            (long index)  // 1 to 4
{
    long data;

    data = 0;
    switch(index)
    {
        case 1:
            SerialSendLsbFirst((char) 0x57, (char) 8);
            SerialSendLsbFirst((char) 0x01, (char) 5);
            data = SerialGet16();
            break;

        case 2:
            SerialSendLsbFirst((char) 0xD7, (char) 8);
            SerialSendLsbFirst((char) 0x00, (char) 5);
            data = SerialGet16();
            break;

        case 3:
            SerialSendLsbFirst((char) 0x37, (char) 8);
            SerialSendLsbFirst((char) 0x01, (char) 5);
            data = SerialGet16();
            break;

        case 4:
            SerialSendLsbFirst((char) 0xB7, (char) 8);
            SerialSendLsbFirst((char) 0x00, (char) 5);
            data = SerialGet16();
            break;
    }
    SerialSendLsbFirst(0x00, 1);  // to be compliant with the data sheet
    return(data);
}


/* ===================================================================== */
/*                               getD1                                   */
/* ===================================================================== */
long getD1            (long *error_pt)
{
    long d1;
    long error;

    SerialSendLsbFirst(0x2F, 8);
    SerialSendLsbFirst(0x00, 2);
    error = 0;
    if (getDOUT()==false)
        error = 1;              // line should be at 1 now
    SerialSendLsbFirst(0x00, 2);

    if (!error)
        error = waitOnDoutFall();

    if (!error)
        d1 = SerialGet16();
    else
        d1 = 0;

    SerialSendLsbFirst(0x00, 1);  // to be compliant with the data sheet
    if (error_pt!=0)
        *error_pt = error;
    return(d1);
}


/* ===================================================================== */
/*                               getD2                                   */
/* ===================================================================== */
long getD2            (long *error_pt)
{
    long d2;
    long error;

    SerialSendLsbFirst(0x4F, 8);
    SerialSendLsbFirst(0x00, 3); // Note the difference with getD1
    error = 0;
    if (getDOUT()==false)
        error = 1;              // line should be at 1 now
    SerialSendLsbFirst(0x00, 1);

    if (!error)
        error = waitOnDoutFall();

    if (!error)
        d2 = SerialGet16();
    else
        d2 = 0;
    if (error_pt!=0)
        *error_pt = error;
    return(d2);
}
