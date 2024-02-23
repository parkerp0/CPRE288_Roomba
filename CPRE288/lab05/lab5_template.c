/**
 * lab5_template.c
 *
 * Template file for CprE 288 Lab 5
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 * @author Diane Rover, updated 2/25/2021, 2/17/2022
 */

#include "button.h"
#include "timer.h"
#include "lcd.h"
#include "uart.h"

#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART1)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

#include "cyBot_Scan.h"  // Scan using CyBot servo and sensors


#define REPLACEME 0



int main(void) {
    int num = 0;
    char message[90];
    char count[5] = "";
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();

  // initialize the cyBot UART1 before trying to use it

  // (Uncomment ME for UART init part of lab)
	 cyBot_uart_init_clean();  // Clean UART1 initialization, before running your UART1 GPIO init code


	 //Complete this code for configuring the GPIO PORTB part of UART1 initialization (your UART1 GPIO init code)
//      SYSCTL_RCGCGPIO_R |= 0x2;
//	    while ((SYSCTL_PRGPIO_R & 0x2) == 0) {};
//		  GPIO_PORTB_DEN_R |= 0x3;
//		  GPIO_PORTB_AFSEL_R |= 0x3;
//      GPIO_PORTB_PCTL_R &= 0x11;     // Force 0's in the desired locations
//      GPIO_PORTB_PCTL_R |= 0x11;     // Force 1's in the desired locations
		 // Or see the notes for a coding alternative to assign a value to the PCTL field

    // (Uncomment ME for UART init part of lab)
		//cyBot_uart_init_last_half();  // Complete the UART device configuration

		// Initialize the scan
	  // cyBOT_init_Scan();
		// Remember servo calibration function and variables from Lab 3

	uart_init();

	while(1)
	{

//	    num = button_getButton();
//	          lcd_printf("%d",num);
//	          switch(num)
//	          {
//	          case 1:
//	              sprintf(message, "Button 1 has been pressed woo!    \r");
//	          break;
//	          case 2:
//	              sprintf(message, "Button 2 has been pressed Hi!     \r");
//	          break;
//	          case 3:
//	              sprintf(message, "Button 3 has been pressed AY YOO! \r");
//	          break;
//	          case 4:
//	              sprintf(message, "Button 4 has been pressed Wee!   \r");
//	          break;
//	          case 0:
//	              sprintf(message, "No button is pressed :(           \r");
//	          break;
//	          }
//
//	          uart_sendStr(message);

	   message[num] = uart_receive();

	    timer_waitMillis(1);
	   //lcd_putc(message[num]);
	   if(message[num])
	   {
	       uart_sendChar(message[num]);

	       if(num>=19 || message[num] == '\r')
	           {

	           uart_sendChar('\n');
	           if(message[num] == '\r')
	               num--;
	           else
	               uart_sendChar('\r');


	           message[num+1]='\0';
	           lcd_clear();
	           lcd_puts(message);
	           lcd_setCursorPos(0,1);
	           sprintf(count,"%d",num+1);
	           lcd_puts(count);
	           num=-1;
	           }
	       num++;
	   }

	}

}
