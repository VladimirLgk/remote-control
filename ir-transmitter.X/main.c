/* 
 * File:   main.c
 * Author: Vladimir Legky
 *
 * Created on 08 January 2014, 01:20
 */
#include <htc.h>
#include <stdio.h>

#include "system.h"
#include "uart.h"

__CONFIG(FOSC_INTRCIO & PWRTE_OFF & WDTE_OFF & CPD_OFF & CP_OFF & MCLRE_OFF);

//store calibrated value for osc as retlw instruction in given address
const unsigned char osccallibrate @ 0x3FF = 0x34;

void configure(void)
{
    //configure GPIO as Led driver, Lcd Driver,
    //Uart output driver, Ir Sensor
    GPIO = 0;
    CMCON = 0x07;
    TRISIO = 0b11001000;

    //setup initial value
    STB = 1;
    DAT = 0;
    CLK = 1;
    SERIAL = 1;
}

unsigned char toHex(unsigned char v)
{
    if(v >= 10)
       v += 'A'-'0'-10;
    v += '0';
    return v;
}

#define LCD_DEBUG
#define UART_DEBUG
void uartErrorMessage();
void uartOkMessage(unsigned char data);
void lcdErrorMessage(unsigned char * msg);
void lcdOkMessage(unsigned char * msg, unsigned char data);


void main(void)
{
    configure();
#ifdef LCD_DEBUG
    lcdInit();
    unsigned char lcdMessage[8];
#endif

    while(1)
    {
       __delay_ms(500);
       error = 0;
#ifdef LCD_DEBUG
      lcdMessage[0] = 0b10000000;
      lcdPuts(DIGIT0 , lcdMessage, 1);
#endif
#ifdef UART_DEBUG
       putch('\r');
       putch('\n');
#endif
       unsigned char data = getIrData();
       if(error == 0)
       {
        #ifdef LCD_DEBUG
                lcdOkMessage(lcdMessage,data);
        #endif
        #ifdef UART_DEBUG
                uartOkMessage(data);
        #endif
                //TBD: perform operations
       }
       else
       {
        #ifdef LCD_DEBUG
                lcdErrorMessage(lcdMessage);
        #endif
        #ifdef UART_DEBUG
                uartErrorMessage();
        #endif
       }
    }
}
