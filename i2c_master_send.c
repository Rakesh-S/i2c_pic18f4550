#include<p18f4550.h>
#define _XTAL_FREQ 8000000
// CONFIG
#pragma config FOSC = HS   // Oscillator Selection bits (XT oscillator)
#pragma config WDT = OFF  // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRT = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF   // Low-Voltage In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF   // Data EEPROM Memory Code Protection bit

void delay(int a)
{
    int i,j;
    for(i=0;i<a;i++)
        for(j=0;j<a;j++);
}
void i2c_master_init(unsigned long c)
{
    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
    SSPSTAT = 0X80; // slew rate control disabled
    SSPCON1 = 0X28; // Master mode selected and SSPEN enabled
    SSPCON2 = 0X00;
    SSPADD = (_XTAL_FREQ/(4*c))-1;
}

void i2c_master_wait()
{
    while((SSPSTAT & 0X04)||(SSPCON2 & 0X1F));
}

void i2c_master_start()
{
    i2c_master_wait();
    SSPCON2bits.SEN = 1; //start enable set
    //while(SSPCON2bits.SEN);
}

void i2c_master_repeated_start()
{
    i2c_master_wait();
    SSPCON2bits.RSEN = 1;
}

void i2c_master_sent(unsigned b)
{
    i2c_master_wait();
    SSPBUF = b; //data to be sent
}

void i2c_master_stop()
{
    i2c_master_wait();
    SSPCON2bits.PEN = 1;
}

unsigned short master_read(unsigned short a)
{
    unsigned short temp;
    i2c_master_wait();
    SSPCON2bits.RCEN = 1;
    i2c_master_wait();
    SSPCON2bits.ACKDT = (a)?0:1;
    SSPCON2bits.ACKEN = 1;
    return temp;
}

void main(void)
{
    i2c_master_init(100000);
    while(1)
    {
        i2c_master_start();
        i2c_master_sent(0X30); //7 bit address with write option
        i2c_master_sent(0X26); //send data 0X26
        i2c_master_stop();
    }
}

