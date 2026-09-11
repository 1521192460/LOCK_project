#include "main.h"


int main()
{
	u8 key;
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	led_init();
	key_init();
	motor_init();
	w25q64_init();
	lcd_init();
	usart1_init(115200);

	//zk_update();


	 lcd_show_zk_str("AAAAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDD",0,0,32,0x0000,0xFFFF);
	while(1)
	{	

	}
}


