#include <lpc21xx.h>
#include "header.h"

#define TEMP_LIMIT 40

unsigned int adc_value;
unsigned int temp;
unsigned int pir;
unsigned int flame;
unsigned int status;

void gpio_init()
{
    // PIR P0.30
    PINSEL1 &= ~(3 << 28);
    IODIR0 &= ~(1 << 30);

    // Flame sensor P0.5
    PINSEL0 &= ~(3 << 10);
    IODIR0 &= ~(1 << 5);

    // LEDs P0.17, P0.18, P0.19
    IODIR0 |= (7 << 17);
}

int main()
{
    gpio_init();
    adc_init();
    lcd_init();
    uart0_init(9600);

    while(1)
    {
        adc_value = adc_read(1);

        // LM35 temperature conversion
        temp = (adc_value * 330) / 1023;

        pir = (IOPIN0 >> 30) & 1;
        flame = (IOPIN0 >> 5) & 1;

        // Status priority
        if(flame == 0)
            status = 3;
        else if(temp > TEMP_LIMIT)
            status = 2;
        else if(pir == 1)
            status = 1;
        else
            status = 0;

        // All LEDs OFF (active LOW)
        IOSET0 = (7 << 17);

        if(status == 0)
            IOCLR0 = (1 << 17); // Green
        else if(status == 1 || status == 2)
            IOCLR0 = (1 << 18); // Yellow
        else
            IOCLR0 = (1 << 19); // Red

        // LCD
        lcd_cmd(0x01);

        lcd_cmd(0x80);
        lcd_string("TEMP:");
        lcd_integer(temp);
        lcd_string(" C");

        lcd_cmd(0xC0);

        if(status == 0)
            lcd_string("STATUS: SAFE");
        else if(status == 1)
            lcd_string("WORKER DETECTED");
        else if(status == 2)
            lcd_string("STATUS: HOT");
        else
            lcd_string("FIRE DETECTED");

        // UART
        uart0_tx_string("Temperature : ");
        uart0_integer(temp);
        uart0_tx_string(" C\r\n");

        uart0_tx_string(pir ? "Worker : YES\r\n" : "Worker : NO\r\n");

        uart0_tx_string(flame == 0 ?
                        "Fire : YES\r\n" : "Fire : NO\r\n");

        if(status == 0)
            uart0_tx_string("Status : SAFE\r\n");
        else if(status == 1)
            uart0_tx_string("Status : WORKER DETECTED\r\n");
        else if(status == 2)
            uart0_tx_string("Status : HOT\r\n");
        else
            uart0_tx_string("Status : FIRE ALERT\r\n");

        uart0_tx_string("--------------------\r\n");

        delay_ms(500);
    }
}
