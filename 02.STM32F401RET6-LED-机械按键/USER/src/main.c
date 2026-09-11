#include "main.h"




int main()
{
	u8 key;
	led_init();
	key_init();



	while(1)
	{
		key = key_scan();
		if(key == 1)
		{
			LEDR_ON;
			delay_ms(100);
			LEDR_OFF;
			delay_ms(100);
		}
		else if(key == 2)
		{
			LEDG_ON;
			delay_ms(100);
			LEDG_OFF;
			delay_ms(100);
		}
		
		// LEDR_ON;
		// delay_ms(500);
		// LEDR_OFF;
		// delay_ms(500);
		// LEDG_ON;
		// delay_ms(500);
		// LEDG_OFF;
		// delay_ms(500);
	}

}


