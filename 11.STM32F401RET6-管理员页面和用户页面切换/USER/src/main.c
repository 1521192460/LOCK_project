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
	CY8CMBR3116_init();		//CY8CMBR3116初始化
	if(KEY1)
	{
		//zk_update();		//字库烧录
		printf("初始化设置\r\n");
		at24c02_write_byte(0,0);//将初始密码标志位设置为0
		MG200_erase_all();
		printf("所有用户指纹已擦除\r\n");
	}
	//检查初始密码
	check_init_password();
	//设置管理员密码
	set_admin_password();
	
    u8 current_page;
    u8 key_value;
    u8 auth_result;

    current_page = USER_PAGE;
	while(1)
	{
		// if(current_page == USER_PAGE)
        // {
        //     auth_result = user_page();

        //     if(auth_result == AUTH_ADMIN_OK)
        //     {
        //         current_page = ADMIN_PAGE;
        //     }
        // }
        // else if(current_page == ADMIN_PAGE)
        // {
        //     if(manage_page() == USER_PAGE)
        //     {
        //         current_page = USER_PAGE;
        //     }
        // }
		switch(current_page)
		{
			case USER_PAGE:auth_result = user_page();if(auth_result == AUTH_ADMIN_OK){current_page = ADMIN_PAGE;}break;
			case ADMIN_PAGE:auth_result = manage_page();if(auth_result == USER_PAGE){current_page = USER_PAGE;}break;
		}
	}
}



