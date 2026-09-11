#include "main.h"


int main()
{
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	led_init();				//LED初始化
	key_init();				//按键初始化
	motor_init();			//电机初始化
	w25q64_init();			//W25Q64初始化
	at24c02_init();			//AT24C02初始化
	lcd_init();				//LCD屏幕初始化
	usart1_init(115200);	//USART1初始化
	if(KEY2)
	{
		zk_update();		//字库烧录
	}

	u8 write_data[] = "我是一个学生";
	at24c02_write_cross_page(0,sizeof(write_data),write_data);
	u8 data[sizeof(write_data)] = {0};
	at24c02_sequential_read(0,sizeof(data),data);

	printf("%s\r\n",data);

	while(1)
	{	

	}
}


