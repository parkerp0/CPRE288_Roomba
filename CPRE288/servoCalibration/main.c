#include "cyBot_scan.h"
#include "cyBot_uart.h"
#include "Timer.h"
#include "lcd.h"

/**
 * main.c
 */
int main()
{
    timer_init();
    lcd_init();
    cyBOT_init_Scan(0b0111);
    cyBOT_SERVO_cal();

	return 0;
}
