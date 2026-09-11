#include "main.h"




int main()
{
	// u8 key;
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	led_init();
	key_init();
	usart1_init(115200);



	while(1)
	{	
		printf("hello world\r\n");
		timer9_delay_ms(1000);
	}

}


