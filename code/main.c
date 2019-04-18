#include "include.h"


sbit FMQ=P3^4;

sbit LED1=P1^0;
sbit LED2=P1^1;
sbit LED3=P1^2;

sbit STAT=P1^3;
sbit EN=P1^4;
void delay_ms( INT16U tms )
{
    INT16U i;

    while( tms-- )
    {
    	for( i = 0; i < 300; i ++ )
    	{
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
    	}
    }
}

void init_port( void )
{
//    P0M1 = 0x00;
//    P0M0 = 0xff;

    P3M1 &= ~0x10;
    P3M0 |= 0x10;
    //
    //	P1M1=0x00;
    //	P1M0=0x00;
    //
    //	P2M1=0x00;
    //	P2M0=0xff;
    //
    //	P30=1;
    //	P3M1=0x41;
    //	P3M0=0xa2;
    //
    //	P4M1=0x02;
    //	P4M0=0x00;
    BEEP_OFF;
    LED_OFF;
}

void init_par( void )
{
    BEEP_OFF;
    LED_OFF;
}

void init_wdt( void )	 //2.7S
{
    //	WDT_CONTR = 0xC1;
    //	WDT_CONTR = 0x3E;
}

void feed_wdt( void )
{
    //	WDT_CONTR = 0x3E;
}

void init_all(void)
{
    EA = 0;

    init_timer();
    init_uart();
    init_port();

    //init_rc522();
    init_par();
    init_wdt();

    EA = 1;
}
/*
void delay(int ms)// 延时子程序
{
    char i;
    while(ms--)
    {
        for(i=0;i<120;i++);
    }
}*/
/*
void ledc(int i)
{
    P1 |= i;
    delay(2000);
    P1 &= ~i;
    delay(2000);
    P1 |= i;
    delay(2000);
    P1 &= ~i;
}
*/
void main(void)
{

    if((PCON&0x10)==0) //如果POF位=0
    {
       PCON=PCON|0x10;  //将POF位置1
       IAP_CONTR=0x60;  //软复位,从ISP监控区启动
    }
    else
    {
       PCON=PCON&0xef;  //将POF位清零
    }

    lcdInitinal();
    lcdMsg("创思通信",2,2);
    lcdMsg("www.csgsm.com",4,1);
    delay_ms(1000);
    lcdInitinal();
    delay_ms(2);
    lcdMsg("蓝牙模块",1,2);
    lcdMsg("www.csgsm.com",2,1);

    init_all();
    clear_buf_uart();
#if	0
    {
        init_all();

        feed_wdt();
        ctrl_process();
        
    }
#endif
	STAT=1;
	EN = 1;
    while(1)
    {
        delay_ms(500);
        if(strlen(UartBuf)> 0)
        {
            if(strstr(UartBuf,"f1")!=NULL)
            {
                pass();
//                FMQ=1;
                delay_ms(1000);
            }
            if(strstr(UartBuf,"f0")!=NULL)
            {
                warn();
//                FMQ=0;
                delay_ms(1000);
            }
            if(strstr(UartBuf,"k11")!=NULL)
            {

                LED1=0;
            }
            if(strstr(UartBuf,"k10")!=NULL)
            {
                LED1=1;
            }
            if(strstr(UartBuf,"k21")!=NULL)
            {
                LED2=0;
            }
            if(strstr(UartBuf,"k20")!=NULL)
            {
                LED2=1;
            }
            if(strstr(UartBuf,"k31")!=NULL)
            {
                LED3=0;
            }
            if(strstr(UartBuf,"k30")!=NULL)
            {
                LED3=1;
            }

            if(strstr(UartBuf,"+")==NULL&&strstr(UartBuf,"ER")==NULL&&strstr(UartBuf,":")==NULL&&strstr(UartBuf,"NEC")==NULL)
            {
                lcdMsg("----------------",3,0);
                lcdMsg("----------------",4,0);
                lcdMsg(UartBuf,3,0);
                send_bytes(UartBuf);
                send_bytes("\r\nSEND OK\r\n");
    			lcdMsg("  蓝牙模块  ",1,0);
    			lcdMsg(" www.csgsm.com  ",2,0);
            }

            clear_buf_uart();
        }
    }

}
