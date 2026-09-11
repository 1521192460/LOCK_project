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
	CY8CMBR3116_init();




	u8 err_cnt = 0;
	u8 password_buff[6];
	u8 password[7];
	u8 password_confirm[7];
	u8 open_flag;//开机标志位：0xff表示第一次开机
	//1.读出开机标志位
	at24c02_read_byte(0,&open_flag);
	if(open_flag == 0xff)//如果不是第一次开机
	{
		at24c02_write_byte(0,0x33);
start1:
		//1.输入密码
		lcd_show_zk_str("请输入密码(6位)         ",0,0,32,0x0000,0xffff);
		for(int i = 0;i < 6;i++)
		{
			while((password[i] = CY8CMBR3116_key_scan()) == 0xff);  // 死等，直到有按键按下
		}
		sprintf((char *)password_buff,"%s",(const char *)password);	//将密码转换为字符串
		lcd_show_zk_str("按‘#’键确认           ",0,1,32,0x0000,0xffff);
		while(CY8CMBR3116_key_scan() != '#');
		//2.比对密码
		//读出密码
		at24c02_sequential_read(1,sizeof password,password);
		sprintf((char *)password_confirm,"%s",(const char *)password);
		if(strcmp((char *)password_buff,(char *)password_confirm) == 0)
		{
			lcd_show_zk_str("密码正确                ",0,3,32,0x0000,0xffff);
		}
		else
		{
			lcd_show_zk_str("密码错误，请重新输入    ",0,3,32,0x0000,0xffff);
			err_cnt++;
			if(err_cnt >= 3)
			{
				lcd_show_zk_str("3次密码错误，系统锁定30s",0,3,32,0x0000,0xffff);
				delay_ms(30000);
				err_cnt = 0;
				goto start1;
			}
			goto start1;
		}
	}
	else//这是第一次开机
	{
		//1.1将开机标志位写入0号地址
		at24c02_write_byte(0,0xff);
		//1.2设置初始密码
start2:
		lcd_show_zk_str("请设置初始密码(6位)     ",0,0,32,0x0000,0xffff);
		for(int i = 0;i < 6;i++)
		{
			while((password[i] = CY8CMBR3116_key_scan()) == 0xff);  // 死等，直到有按键按下
		}
		lcd_show_zk_str("按‘#’键确认           ",0,1,32,0x0000,0xffff);
		while(CY8CMBR3116_key_scan() != '#');
		sprintf((char *)password_buff,"%s",(const char *)password);	//将密码转换为字符串
		lcd_show_zk_str("再次输入密码确认        ",0,2,32,0x0000,0xffff);
		for(int i = 0;i < 6;i++)
		{
			while((password[i] = CY8CMBR3116_key_scan()) == 0xff);  // 死等，直到有按键按下
		}
		lcd_show_zk_str("按‘#’键确认           ",0,3,32,0x0000,0xffff);
		while(CY8CMBR3116_key_scan() != '#');
		sprintf((char *)password_confirm,"%s",(const char *)password);
		if(strcmp((char *)password_buff,(char *)password_confirm) == 0)
		{
			lcd_show_zk_str("密码确认成功            ",0,3,32,0x0000,0xffff);
			delay_ms(500);
			at24c02_write_cross_page(1,sizeof password_confirm,(u8 *)password_confirm);
			goto start1;
		}
		else
		{
			lcd_show_zk_str("密码确认失败，请重新输入",0,3,32,0x0000,0xffff);
			delay_ms(500);
			goto start2;
		}
	}

	while(1)
	{	

	}
}


