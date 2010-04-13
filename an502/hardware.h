//---------------------------------------------------------------------------

#ifndef hardwareH
#define hardwareH


long  sensor_controlInit   ();  // 0 = no error
void  sensor_controlExit    (void);


void setSCLK  (bool state);    // set SCLK to the specified state (true/false)
bool getSCLK  (void);          // returns the current state of SCLK

void setDIN   (bool state);    // set DIN  to the specified state (true/false)
bool getDIN   (void);          // returns the current state of DIN

bool getDOUT  (void);          // returns the current state of DOUT

void WaitOnePulse (void);      // Wait for a "pulse" duration



//---------------------------------------------------------------------------
#endif
