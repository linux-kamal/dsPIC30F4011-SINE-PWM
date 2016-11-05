#include "header.h"
/*--------------------------- Configuration words ----------------------------*/
//#pragma config FPR=HS
#pragma config FPR=XT_PLL16
#pragma config FOS=PRI
#pragma config WDT = WDT_OFF
#pragma config FPWRT = PWRT_OFF
/*----------------------------------------------------------------------------*/
//change maximum value of [PDC] register value according to [PTPER] since its value depends on PWM time period.
// PDC max value would be [2XPTPER]
// lets say PDC max value is 250 and swithcing frequency[of carrier signal] = 12.5 to 12.8 khz [time period would be 0.08 ms]
// in 10ms[half sine period] 10/.08 = 125 [number of samples]
// in total half sine 180/125= 1.44 degree has to be generated
// here should be 125 or 126
// Is it ture for cross condution or dead time max duty cycle should be 95%

// http://www.ocfreaks.com/sine-wave-generator-using-pwm-lpc2148-microcontroller-tutorial/
//angle = ( ((float)i) * ANGLE_STEP_RATE );
//sineLookupTable[i] = rint(100 + 99* sin( angle * (PI/180) )); //converting angle to radians
unsigned int Sine_Look_up_table_one_[125] = {0, 7, 15, 22, 30, 37, 44, 51, 59, 66, 73, 80, 87, 94, 101, 108, 115, 122, 129, 135, 142, 148, 154, 161, 167, 173, 179, 185, 190, 196, 201, 207, 212, 217, 222, 227, 231, 236, 240, 244, 248, 252, 256, 259, 263, 266, 269, 272, 275, 277, 280, 282, 284, 286, 287, 289, 290, 291, 292, 293, 293, 294, 294, 294, 294, 293, 293, 292, 291, 290, 289, 287, 286, 284, 282, 280, 277, 275, 272, 269, 266, 263, 259, 256, 252, 248, 244, 240, 236, 231, 227, 222, 217, 212, 207, 201, 196, 190, 185, 179, 173, 167, 161, 154, 148, 142, 135, 129, 122, 115, 108, 101, 94, 87, 80, 73, 66, 59, 51, 44, 37, 30, 22, 15, 7};
// consider 2-D array for VARYING DUTY VALUES
int main(void)
{
    TRISFbits.TRISF5 = 0;
    InitPWM();


    while(1);
    return 0;
}




void InitPWM(void)
{
    TRISE = 0X00;
    PWMCON1 = 0x00FF;
    PTCON = 0;
    _PTCKPS = 2;        //(0=1:1, 1=1:4, 2=1:16, 3=1:64)
    PTPER = 147;       //[20 ms period] PTPER = PWM PERIOD/(TCY*PRESCALER) [15-bit register max value 32768 or [0-32767]]
    PTMR = 0;
    PTCONbits.PTEN = 1;
    /* Interrup Enable */
    IEC2bits.PWMIE = 1;
}
void duty(unsigned int value)
{
    PDC1 = value;  //Duty Cycle has a max value of [2xPeriod] OR PDC = PTPER means 50% duty
}

void __attribute__((__interrupt__, __auto_psv__)) _PWMInterrupt(void)
{
    // consider 2-D array for VARYING DUTY VALUES
    // On every pwm period match[when PTMR = PTPER] this interrupt occurs
    // update PDC REGISTER value from sine table array
    // keep counting if First half pass on then turn that PWM and keep updating another PWM
    static int i = 0; // max value of can go 125 from 0
    static char flag = 0;
    IFS2bits.PWMIF = 0;
    if(i>125)
    {   
        i = 0;
        if(!flag)
            flag = 1;
        else
            flag = 0;
    }
    if(!flag)
    {
        PDC2 = 0;
        PDC3 = Sine_Look_up_table_one_[i];
    }
    else
    {
        PDC3 = 0;
        PDC2 = Sine_Look_up_table_one_[i];
    }
    i++;
    
}
