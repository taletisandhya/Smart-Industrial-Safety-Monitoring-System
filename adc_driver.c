#include"header.h"
void adc_init()
{
	PINSEL1|=0X15400000;
	ADCR=(1<<21)|(4<<8);
}
#define DONE ((ADDR>>31)&1)
unsigned int adc_read(unsigned int ch_num)
{
	unsigned int result=0;
	ADCR &=~0XFF;
	ADCR|=(1<<ch_num);
	ADCR|=1<<24;
	while(DONE==0);
	ADCR^=1<<24;
	ADCR^=(1<<ch_num);
	result=(ADDR>>6)&0X3FF;
	return result;
}
