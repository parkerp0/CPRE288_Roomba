/**
 * lab4_template.c
 *
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "puTTy.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

#warning "Possible unimplemented functions"
#define REPLACEME 0



int main(void) {
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	cyBot_uart_init();            // Don't forget to initialize the cyBot UART before trying to use it
	int num;
	char message[90];

	// YOUR CODE HERE


	
	while(1)
	{
	  num = button_getButton();
	  lcd_printf("%d",num);
	  switch(num)
	  {
	  case 1:
	      sprintf(message, "Button 1 has been pressed woo!    \r");
	  break;
	  case 2:
	      sprintf(message, "Button 2 has been pressed Hi!     \r");
	  break;
	  case 3:
	      sprintf(message, "Button 3 has been pressed AY YOO! \r");
	  break;
	  case 4:
	      sprintf(message, "Button 4 has been pressed Wee!   \r");
	  break;
	  case 0:
	      sprintf(message, "No button is pressed :(           \r");
	  break;
	  }

	  sendString(message);
      // YOUR CODE HERE




	}

}
