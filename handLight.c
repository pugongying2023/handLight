#include <tiny13a.h>
#include <delay.h>

#define uchar unsigned char
#define uint unsigned int

#define key PINB.1

//#define LED PORTB.0

uchar key_code = 0;
uchar time_counter = 0;

bit change_flag = 0;

void key_scan(void)
{
    if(key)
    {                                                
        if((time_counter > 3) && (time_counter < 100))//短按
        {
            if(change_flag)
            {
                key_code++;
                if(key_code == 5) key_code = 1;
            }
        }
        time_counter = 0;
    }   
    else   
    {  
        time_counter++;
        delay_ms(10);  
        if(time_counter == 100)//长按
        {
            if (key_code != 0)        //如果状态不处于关机状态，则执行关机
            {
                key_code = 0;
                change_flag = 0;
            }
            else
            {
                key_code = 1;         //如果已经在关机状态则执行开机
                change_flag = 1;
            }
        }
        if(time_counter > 100)
            time_counter = 101;   
    }
}

void main(void)
{
// Input/Output Ports initialization
// Port B initialization
// Function: Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=Out
DDRB=(0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (1<<DDB0);
// State: Bit5=T Bit4=T Bit3=T Bit2=T Bit1=P Bit0=0
PORTB=(0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (1<<PORTB1) | (1<<PORTB0);

TCCR0A=0x03;
TCCR0B=0x01;

    while (1)
    {
        key_scan();

        switch (key_code)
        {
            case 0x00:              //初始关机状态
                TCCR0A=0x03;
                PORTB.0 = 1;
                break;
            case 0x01:
                TCCR0A=0x83;
                OCR0A = 0x19;       //10%亮度
                break;
            case 0x02:
                OCR0A = 0x40;       //25%亮度
                break;
            case 0x03:
                OCR0A = 0x80;       //50%亮度
                break;
            case 0x04:
                OCR0A = 0xc0;       //75%亮度
                break;                  
            default:
                break;
        }   
    }
}