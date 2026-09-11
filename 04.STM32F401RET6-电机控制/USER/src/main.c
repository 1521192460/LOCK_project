#include "main.h"




int main()
{
	u8 key;
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	led_init();
	key_init();
	motor_init();
	usart1_init(115200);


	while(1)
	{	
		key = key_scan();
		if(key == 1)
		{
			LOCK_ON;
		}
		else if(key == 2)
		{
			LOCK_OFF;
		}
	}

}


