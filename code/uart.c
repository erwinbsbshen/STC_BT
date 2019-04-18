#include "include.h"

INT8U  UartBuf[UART_MAX_LEN] = { 0 };
INT8U UartCount = 0, UartLen = 0, UartStart = FALSE, UartComp = FALSE;

/**********************************************************************
functionName:putChar(BYTE sentData)
description:通过串口发送数据sentData
**********************************************************************/
void send_byte( INT8U SendData )
{
    ES = 0;
    TI = 0;
    SBUF = SendData;
    while( TI == 0 );
    TI = 0;
    ES = 1;
}


void send_bytes( INT8U *Buf )
{
    while( *Buf )
    {
        send_byte( *Buf++ );
    }
}

/**********************************************************************
串口发送格式：fe  cmdlen  datalen cxx cxx dxx dxx dxx ... check
**********************************************************************/
void uart_interrupt_receive(void) interrupt 4
{
    INT8U ReByte;
    if( RI == 1 )
    {
        RI = 0;
        ReByte = SBUF;

        if(UartCount >= UART_MAX_LEN)
        {
            UartCount = 0 ;
        }
        else
        {
            UartBuf[ UartCount++ ] = ReByte;
        }
    }
}

//UART0 initialize
// desired baud rate: 9600
// actual: baud rate:9600 (0.0%)
// char size: 8 bit
// parity: Disabled
void init_uart(void)
{
    SCON = 0x50;
    ES=1;
    AUXR1|= 0X80;
}


void clear_buf_uart(void)
{
    unsigned int i = 0 ,c ;
    c = UART_MAX_LEN +1 ;
    ES = 0 ;

    for( i = 0 ; i < c ; i ++)
    {
       UartBuf[i] = 0x0 ;
    }

    UartCount = 0 ;

    ES = 1 ;
}
