#include "main.h"


int main()
{
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	led_init();
	key_init();
	motor_init();
	w25q64_init();
	lcd_init();
	usart1_init(115200);

	//zk_update();

	lcd_show_zk_str("123",0,0,16,0x0000,0xFFFF);
	lcd_show_zk_str("123",0,16,24,0x0000,0xFFFF);
	lcd_show_zk_str("123",0,40,32,0x0000,0xFFFF);
	lcd_show_zk_char('1',0,74,64,0x0000,0xFFFF);

	while(1)
	{	

	}
}


