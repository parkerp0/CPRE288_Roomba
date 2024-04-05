#include "servo.h"
#include "Timer.h"
#include "lcd.h"
/**
 * main.c
 */
int main(void)
{
    timer_init();
    lcd_init();
    servo_init();

    timer_waitMillis(1000);
    servo_move(45);

    timer_waitMillis(1000);
    servo_move(135);

	while(1);
}
