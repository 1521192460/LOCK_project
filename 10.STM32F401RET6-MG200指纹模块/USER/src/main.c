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
	NV400F_init();			//NV400F语音初始化
	MG200_init();			//MG200指纹初始化

	if(KEY1)
	{
		//zk_update();		//字库烧录
		printf("初始化设置\r\n");
		at24c02_write_byte(0,0);//将初始密码标志位设置为0
	}
	// CY8CMBR3116_init();
	// //检查初始密码
	// check_init_password();
	// NV400F_send_data(0x2f);
	// lcd_show_zk_str("请输入密码：                ",0,0,32,0x0000,0xffff);





	MG200_enroll(0x00);

	
	
	while(1)
	{
		u8 id = MG200_match();
		if(id != 0)
		{
			printf("匹配成功，用户ID：%d\r\n",id);
			delay_ms(3000);
			printf("擦除用户指纹...\r\n");
			MG200_erase(id);
			printf("擦除完成\r\n");
		}
		else
		{
			printf("匹配失败\r\n");
		}
		
	}
}


