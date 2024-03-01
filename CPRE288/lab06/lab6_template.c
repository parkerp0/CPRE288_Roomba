/**
 * lab6_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 */

#include "Timer.h"
#include "lcd.h"
#include "cyBot_Scan.h"  // For scan sensors
#include "uart-interrupt.h"

// Uncomment or add any include directives that are needed
 #include "open_interface.h"
// #include "movement.h"
// #include "button.h"


#define REPLACEME 0


int main(void) {
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_interrupt_init();
    cyBOT_init_Scan(0b0111);

    cyBOT_Scan_t* scan = malloc(sizeof(cyBOT_Scan_t));


    int i;

	// YOUR CODE HERE

	while(1)
	{
	  if(command_byte == 'g')
	  {
	      command_byte = -1;
          for(i=45; i<130; i+=2)
          {
              cyBOT_Scan(i,scan);
              if(command_byte == 's')
              {
                  command_byte = -1;
                  break;
              }
          }
	  }


	}

}
