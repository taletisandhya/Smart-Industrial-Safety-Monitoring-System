#include"header.h"
void lcd_data(unsigned char data)
{
	IOCLR0=0X7FF;
	IOSET0=data;
	IOSET0=1<<8;
	IOCLR0=1<<9;
	IOSET0=1<<10;
	delay_ms(2);
	IOCLR0=1<<10;
}
void lcd_cmd(unsigned char cmd)
{
	IOCLR0=0X7FF;
	IOSET0=cmd;
	IOCLR0=1<<8;
	IOSET0=1<<9;
	IOSET0=1<<10;
	delay_ms(2);
	IOCLR0=1<<10;
}
void lcd_init()
{
	IODIR0=0X7FF;
	lcd_cmd(0x38);
	lcd_cmd(0x0e);
	lcd_cmd(0x01);
}
void lcd_string(char *ptr)
{
	while(*ptr)
	{
		lcd_data(*ptr);
		ptr++;
	}
}
void lcd_integer(int num)
{
	int a[10],i=0;
	if(num==0)
	{
		lcd_data('0');
	}
	else if(num<0)
	{
		num=-num;
		lcd_data('-');
	}
	while(num>0)
	{
		a[i]=num%10+48;
		num=num/10;
		i++;
	}
	for(i=i-1;i>=0;i--)
	{
		lcd_data(a[i]);
	}
}

