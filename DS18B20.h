#include <msp430f169.h>
#include "type.h"
#include "delay.h"

#ifndef DS18B20_H_
#define DS18B20_H_

uchar   A1,A2,A3;                     //����ı���,��ʾ���ݴ���
uchar   Temp_Value[6];                     //��ʾ�õ��¶����ݣ������÷�������׼������
uint    temp_value;

//DS18B20���ƽţ����ſ���
#define DQ_IN           P1DIR &= ~BIT7        //�������룬DS18B20�ӵ�Ƭ��P53��
#define DQ_OUT          P1DIR |= BIT7         //�������
#define DQ_CLR          P1OUT &= ~BIT7            //�õ͵�ƽ
#define DQ_SET          P1OUT |= BIT7             //�øߵ�ƽ
#define DQ_R            P1IN & BIT7       //����ƽ

unsigned char DS18B20_Reset(void);
unsigned char ds1820_read_byte(void);
void ds1820_write_byte(unsigned char);
void ds1820_start(void);
unsigned int ds1820_read_temp(void);
void data_do(uint);

#endif /* DS18B20_H_ */
