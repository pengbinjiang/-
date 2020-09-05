#include <msp430f169.h>
#include "DS18B20.h"
#include "UART.h"
#include "miaobiao.h"
#include "keypd.h"
#include "lcd12864.h"
#include "electrical.h"
#include "clock.h"

//Һ����ʾ���ֱ���
uchar shuzi[] = {"0123456789"};
//˳���룬�֣�ʱ���գ��£����ڣ��ꣻ��ʽ��BCD
uchar times[7];

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
    init_clk();
	LCD_Port_init();
	delay_ms(100);                      //��ʱ100ms
	LCD_init();                         //Һ��������ʼ������
	LCD_clear();                        //����
	LCD_Desk();
	DS18B20_Reset();                    //��λD18B20
	xianshi();
	timer_A_init();
	ele_init();
	timer_B_init();
	init_port1();
	UART_Init();
	speed = 0;
	j=0;
	_EINT();
	keyvalue=0;
	while(1){
	    stopwatch(keyvalue);                //��ͣ��������ʱ

	    ds1820_start();                     //����һ��ת��
	    ds1820_read_temp();                 //��ȡ�¶���ֵ
	    data_do(temp_value);                //�������ݣ��õ�Ҫ��ʾ��ֵ

	    lcd_pos(3,3);
	    DisplayCgrom(Temp_Value);           //��ʾ�¶ȣ��¶�ֵ��Temp_Value������
	    DisplayCgrom("���϶�");

	    if(keyvalue == 4) j=0;
	    if(keyvalue == 5) j=1;

	    if(keyvalue == 0) speed = 0 ,j = 0;
	    if((keyvalue == 1)&(j == 0)) speed = 3;
	    if((keyvalue == 2)&(j == 0)) speed = 4;
	    if((keyvalue == 3)&(j == 0)) speed = 5;

	    if(j == 1){
	        if(temp_value>=160 & temp_value<170)
	        {
	            if(month>4 & month<10) speed = 1;
	            else speed = 0;
	        }

	        if(temp_value>=170 & temp_value<180)
	        {
	            if(month>4 & month<10) speed = 2;
	            else speed = 1;
	        }

	        if(temp_value>=180 & temp_value<190)
	        {
	            if(month>4 & month<10) speed = 3;
	            else speed = 2;
	        }
	        if(temp_value>=190 & temp_value<200)
            {
                if(month>4 & month<10) speed = 4;
                else speed = 3;
            }

	        if(temp_value>=200 & temp_value<210)
            {
                if(month>4 & month<10) speed = 5;
                else speed = 4;
            }

	        if(temp_value>=220)
            {
                if(month>4 & month<10) speed = 6;
                else speed = 5;
            }

	    }
	    choosespeed();

	}
	
}



/*------------------------------------------
P1���жϺ���
------------------------------------------*/
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    if(P1IFG&0x0f)
     {
       Key_Event();             //��ⰴ��
       if(key_Flag == 1)
       {
        key_Flag = 0;
        switch(key_val)
        {
            case 0x01:
            switch(PP++)        //ȷ���α��ַ
            {
            case 0: lcd_pos(0,1);break;
            case 1: lcd_pos(0,3);break;
            case 2: lcd_pos(0,5);break;
            case 3: lcd_pos(1,0);break;
            case 4: lcd_pos(1,2);break;
            case 5: lcd_pos(1,4);break;
            case 6: lcd_pos(2,0);break;
            case 7: lcd_pos(2,2);break;
            default:break;
            }
            LCD_write_com(0x0f);   //���α�
            if(PP == 9) PP = 0;
            cflag = 1;          //��־��λ
            break;
       case 0x02:
           if(cflag)
           {
               switch(PP)
               {
               case 1:         //��
                   year++;
                   LCD_write_data(year / 10 + 0x30);
                   LCD_write_data(year % 10 + 0x30);
                   lcd_pos(0,1);
                   break;
               case 2:         //��
                   month++;
                   LCD_write_data(month / 10 + 0x30);
                   LCD_write_data(month % 10 + 0x30);
                   lcd_pos(0,3);
                   break;
               case 3:         //��
                   day++;
                   LCD_write_data(day / 10 + 0x30);
                   LCD_write_data(day % 10 + 0x30);
                   lcd_pos(0,5);
                   break;
               case 4:         //ʱ
                   hour++;
                   LCD_write_data(hour / 10 + 0x30);
                   LCD_write_data(hour % 10 + 0x30);
                   lcd_pos(1,0);
                   break;
               case 5:         //��
                   min++;
                   LCD_write_data(min / 10 + 0x30);
                   LCD_write_data(min % 10 + 0x30);
                   lcd_pos(1,2);
                   break;
               case 6:         //��
                   sec++;
                   LCD_write_data(sec / 10 + 0x30);
                   LCD_write_data(sec % 10 + 0x30);
                   lcd_pos(1,4);
                   break;
               case 7:         //ʱ
                   hour1++;
                   LCD_write_data(hour1 / 10 + 0x30);
                   LCD_write_data(hour1 % 10 + 0x30);
                   lcd_pos(2,0);
                   break;
               case 8:         //��
                   min1++;
                   LCD_write_data(min1 / 10 + 0x30);
                   LCD_write_data(min1 % 10 + 0x30);
                   lcd_pos(2,2);
                   break;
               default:
                   break;

               }
           }break;
      case 0x03:
               if(cflag)
               {
                   switch(PP-1)
                   {
                       case 0:         //��
                                year--;
                                LCD_write_data(year / 10 + 0x30);
                                LCD_write_data(year % 10 + 0x30);
                                lcd_pos(0,1);
                                break;
                        case 1:         //��
                                month--;
                                LCD_write_data(month / 10 + 0x30);
                                LCD_write_data(month % 10 + 0x30);
                                lcd_pos(0,3);
                                break;
                        case 2:         //��
                                day--;
                                LCD_write_data(day / 10 + 0x30);
                                LCD_write_data(day % 10 + 0x30);
                                lcd_pos(0,5);
                                break;
                        case 3:         //ʱ
                                hour--;
                                LCD_write_data(hour / 10 + 0x30);
                                LCD_write_data(hour % 10 + 0x30);
                                lcd_pos(1,0);
                                break;
                         case 4:         //��
                                min--;
                                LCD_write_data(min / 10 + 0x30);
                                LCD_write_data(min % 10 + 0x30);
                                lcd_pos(1,2);
                                break;
                         case 5:         //��
                                sec--;
                                LCD_write_data(sec / 10 + 0x30);
                                LCD_write_data(sec % 10 + 0x30);
                                lcd_pos(1,3);
                                break;
                         case 6:         //ʱ
                                hour1--;
                                LCD_write_data(hour1 / 10 + 0x30);
                                LCD_write_data(hour1 % 10 + 0x30);
                                lcd_pos(2,0);
                                break;
                         case 7:         //��
                                min1--;
                                LCD_write_data(min1 / 10 + 0x30);
                                LCD_write_data(min1 % 10 + 0x30);
                                lcd_pos(2,2);
                                break;
                        default:
                                break;
                        }
                    }break;
          case 0x04:
                    if(cflag)
                    {
                        cflag = 0;
                        PP = 0;
                        LCD_write_com(0x0c);   //�ر��α�
                        P1IFG=0x00;
                    }
                    break;


        }
       }
 }
}

