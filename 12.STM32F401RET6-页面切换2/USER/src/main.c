#include "main.h"


//1.开锁页面	2.管理员验证	3.管理员页面
u8 page_flag = 1;
//0.表示当前页面不可刷新	1.表示当前页面可刷新
u8 ui_flag = 0;

/****************************
 * 函数名：main
 * 函数功能：主函数
 * 		AT24C02空间分布：
 * 						addr 0空间 用于存储当前页面标志位
 * 						addr 1-21空间 用于存储初始密码
 *          			addr 22空间 用于存储密码长度标志位
 *          			addr 23-33空间 用于存储管理员密码
 *          			addr 34空间 用于存储管理员密码长度
 ****************************/




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
	CY8CMBR3116_init();		//CY8CMBR3116电容按键初始化
	if(KEY1)
	{
		//zk_update();		//字库烧录
		printf("初始化设置\r\n");
		at24c02_write_byte(0,0);//将初始密码标志位设置为0
		at24c02_write_cross_page(23,10,"123456");//默认管理员密码
		at24c02_write_byte(34,6);//默认管理员密码长度
		MG200_erase_all();
		printf("初始化已完成\r\n");
	}
	
	//检查初始密码
	check_init_password();
	// NV400F_send_data(0x2f);
	// lcd_show_zk_str("请输入密码：                ",0,0,32,0x0000,0xffff);

	
	
	while(1)
	{
		//开门页面
		if(page_flag == 1)
		{
			//只执行一次的功能----ui界面
			if(ui_flag == 0)
			{
				ui_flag = 1;
				printf("开门页面\r\n");
				printf("请输入密码\r\n");
			}

			//执行开门功能
			password_open_door();

			if(KEY1)
			{
				page_flag = 2;//切换到管理员验证页面
				ui_flag = 0;//重置ui_flag
				lcd_clear(0,0,240,240,0xffff);
			}
		}
		//管理员验证页面
		else if(page_flag == 2)
		{
			//只执行一次的功能----ui界面
			if(ui_flag == 0)
			{
				ui_flag = 1;
				printf("管理员验证页面\r\n");
				printf("请输入管理员密码\r\n");
			}
			//执行管理员验证功能
			admin_password_check();
		}
		//管理员页面
		else if(page_flag == 3)
		{
			//只执行一次的功能----ui界面
			if(ui_flag == 0)
			{
				ui_flag = 1;
				printf("*******************************************************\r\n");
				printf("管理员页面\r\n");
				printf("1.修改开门密码\r\n");
				printf("2.修改管理员密码\r\n");
				printf("3.注册指纹\r\n");
				printf("4.删除指定指纹\r\n");
				printf("5.删除所有指纹\r\n");
				printf("6.录入新卡片\r\n");
				printf("7.删除指定卡片\r\n");
				printf("8.删除所有卡片\r\n");
				printf("*.返回\r\n");
				printf("*******************************************************\r\n");
			}

			u8 key_value  = 0xff;
			key_value = CY8CMBR3116_get_key();
			if(key_value != 0xff)
			{
				printf("按键值：%d\r\n",key_value);
				if(key_value == '*')
				{
					page_flag = 1;//切换到开门页面
					ui_flag = 0;//重置ui_flag
					key_value = 0xff;
				}
			}
		}
	}
}


