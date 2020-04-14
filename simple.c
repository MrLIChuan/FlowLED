/* This program demonstrates use of parallel ports in the DE10_Basic_Nios2_SoC
 *
 * It performs the following: 
 * 	1. displays the SW switch values on the red LEDs
 * 	2. displays a rotating pattern on the HEX displays
 * 	3. if KEY[1] is pressed, uses the SW switches as the pattern
 */
#include <stdio.h>
#include <altera_avalon_pio_regs.h>
#include <system.h>

int main(void) {
	/* Declare volatile pointers to I/O registers (volatile means that IO load
	 * and store instructions will be used to access these pointer locations, 
	 * instead of regular memory loads and stores)
	 */
	//volatile int * red_LED_ptr	= (int *) 0x10000000;	// green LED address
	//volatile int * HEX3_HEX0_ptr	= (int *) 0x10000010;	// HEX3_HEX0 address
	//volatile int * SW_switch_ptr	= (int *) 0x10000020;	// SW slider switch address
	//volatile int * KEY_ptr		= (int *) 0x10000030;	// pushbutton KEY address
	// The seven segment display are set with logic low
	int HEX_bits = 0xFFFFFFF0;  // pattern for HEX displays
	int SW_value, KEY_value;
	volatile int delay_count; // volatile so the C compiler doesn't remove the loop

	printf("Hello World !");

	while (1) {
		SW_value = IORD_ALTERA_AVALON_PIO_DATA(SLIDER_SWITCHES_BASE);// read the SW slider switch values
		IOWR_ALTERA_AVALON_PIO_DATA(RED_LEDS_BASE, SW_value);// light up the green LEDs

		KEY_value = IORD_ALTERA_AVALON_PIO_DATA(PUSHBUTTONS_BASE); // read the pushbutton KEY values
		if (KEY_value != 0x3)			// check if any KEY was pressed
				{
			HEX_bits = ~SW_value;		// set pattern using SW values

			while (KEY_value != 0x3)
				;	// wait for pushbutton KEY release
		}
		//*(HEX3_HEX0_ptr) = HEX_bits;		// display pattern on HEX3 ... HEX0
		IOWR_ALTERA_AVALON_PIO_DATA(HEX3_HEX0_BASE, HEX_bits);
		IOWR_ALTERA_AVALON_PIO_DATA(RED_LEDS_BASE, HEX_bits);
		/* rotate the pattern shown on the HEX displays */
		if (HEX_bits & 0x80000000)
			HEX_bits = (HEX_bits << 1) | 1;
		else
			HEX_bits = HEX_bits << 1;
		printf("SW:%x\n", SW_value);
		for (delay_count = 300000; delay_count != 0; --delay_count)
			; // delay loop
	}
}
