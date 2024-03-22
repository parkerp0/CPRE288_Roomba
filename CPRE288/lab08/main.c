#include "adc.h"
#include "Timer.h"
#include "lcd.h"

#include <math.h>

int main()
{
    timer_init();
    lcd_init();
    adc_init();

    uint16_t temp;
    double dist = 0;

    char message[90];

    while(1)
    {//y = 8057.6x^-0.937

        temp = adc_read();
        dist = 8057.6 * pow(temp,-0.937);

        sprintf(message,"IR: %d, Dist: %lf",temp,dist);
        lcd_printf(message);
        timer_waitMillis(50);
    }
}
