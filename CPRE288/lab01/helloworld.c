/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */

#include "Timer.h"
#include "lcd.h"
#include <stdlib.h>


int lcd_rotatingBanner(char *data);


int main (void) {

    char *data = "288 is fun";
    lcd_rotatingBanner(data);

	// lcd_puts("Hello, world"); // Replace lcd_printf with lcd_puts
        // step through in debug mode and explain to TA how it works
    
	// NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
       // NOTE: For time functions, see Timer.h

	return 0;
}

int lcd_rotatingBanner(char *data)
{
    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                      // and enables time functions (e.g. timer_waitMillis)

        lcd_init();   // Initialize the LCD screen.  This also clears the screen.

        // Print "Hello, world" on the LCD
        int messageLength=sizeof(data)/sizeof(char);
        char *subset = malloc(sizeof(char)*messageLength);

        int startPos = 20-1;
        int length = 1;
        int dataStart = 0;

        while(data!="\0")
        {
            memcpy(subset, &data[dataStart], length);
            subset[length]= '\0';
            lcd_setCursorPos(startPos, 0);
            lcd_puts(subset);
            timer_waitMillis(300);
    //      data++;
            if (length < 20)
            {
                length++;
                startPos--;
            }else
            {
                dataStart++;
                if (data[dataStart] == '\0'){
                    startPos = 20-1;
                    length = 1;
                    dataStart = 0;
                }
            }
            lcd_clear();
        }
    free(subset);
    return 0;
}
