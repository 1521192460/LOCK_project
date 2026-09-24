#include "app.h"

TaskHandle_t start_task_handle;
TaskHandle_t open_page_task_handle;
TaskHandle_t admin_check_task_handle;
TaskHandle_t admin_page_task_handle;
/*************************
 * 函数名：open_page_task
 * 函数功能：开门页面任务
 * 参数：void *ptr
 * 返回值：无
 * *************************/
void open_page_task(void *ptr)
{
    u8 key;
    u8 time_str[10];
    while(1)
    {   
        key = key_scan();
        if(status.time_updata == 1)
        {
            status.time_updata = 0;
            RTC_t time = get_time_date();
            sprintf((char*)time_str, "%02u:%02u", time.hour, time.min);   
            lcd_show_zk_str((u8*)time_str, 40, 0, 64, 0x0000, 0xffff); 
        }
        //只执行一次----ui界面
        if(ui_flag == 0)
        {
            ui_flag = 1;
            lcd_show_zk_str("欢迎",88,100,32,0x0000,0xffff);
            lcd_show_zk_str("*清空     #开门",0,208,32,0x0000,0xffff);
        }
        
        //一直循环的部分
        //开门功能:密码开门、指纹开门、射频卡开门
        password_open_door();
        MG200_open_door();
        RFID_open_door();
        WIFI_ctrl();
        if(key == 1)    //按键一进入管理员验证页面
        {
            //挂起开门页面任务
            ui_flag = 0;//重置ui_flag
            reset_password_input();
            lcd_clear(0,0,240,240,0xffff);
            vTaskSuspend(open_page_task_handle);
        }
    }
}


/*************************
 * 函数名：admin_confirm_task
 * 函数功能：管理员验证任务
 * 参数：void *ptr
 * 返回值：无
 * *************************/
void admin_check_task(void *ptr)
{
    while(1)
    {   
        //只执行一次的功能----ui界面
        if(ui_flag == 0)
        {
            ui_flag = 1;
            NV400F_send_data(0x0b);//请输入请输入管理员密码
            lcd_show_zk_str("管理员验证",32,0,32,0x0000,0xffff);
            lcd_show_zk_str("输入管理员密码",8,100,32,0x0000,0xffff);
        }
        //执行管理员验证功能
        admin_password_check();
    }
}


/*************************
 * 函数名：admin_page_task
 * 函数功能：管理员页面任务
 * 参数：void *ptr
 * 返回值：无
 * *************************/
void admin_page_task(void *ptr)
{
    while(1)
    {
        u8 key_value  = 0xff;
		key_value = CY8CMBR3116_key_scan();
        //只执行一次的功能----ui界面
		if(ui_flag == 0)
		{
			ui_flag = 1;
			lcd_show_zk_str("1.修改开门密码",0,0,24,0x0000,0xffff);
			lcd_show_zk_str("2.修改管理员密码",0,24,24,0x0000,0xffff);
			lcd_show_zk_str("3.注册指纹",0,48,24,0x0000,0xffff);
			lcd_show_zk_str("4.删除指定指纹",0,72,24,0x0000,0xffff);
			lcd_show_zk_str("5.删除所有指纹",0,96,24,0x0000,0xffff);
			lcd_show_zk_str("6.录入新卡片",0,120,24,0x0000,0xffff);
			lcd_show_zk_str("7.删除指定卡片",0,144,24,0x0000,0xffff);
			lcd_show_zk_str("8.删除所有卡片",0,168,24,0x0000,0xffff);
            lcd_show_zk_str("9.配置wifi",0,192,24,0x0000,0xffff);
			lcd_show_zk_str("*.返回",0,216,24,0x0000,0xffff);
		}
        if(key_value != 0xff)
		{
			switch(key_value)
			{
				case '1':change_password_door();break;
				case '2':change_password_admin();break;
				case '3':MG200_register();break;
				case '4':MG200_delete_id();break;
				case '5':MG200_delete_all();break;
				case '6':RFID_add_card();break;
				case '7':RFID_delete_card();break;
				case '8':RFID_delete_all_card();break;
                case '9':WIFI_config();break;
				case '*':ui_flag = 0;key_value = 0xff;reset_password_input();
                        lcd_clear(0,0,240,240,0xffff);
                        vTaskResume(open_page_task_handle);
                        vTaskResume(admin_check_task_handle);break;
			}
		}
    }
}


/*************************
 * 函数名：start_task
 * 函数功能：起始任务
 * 参数：void *ptr
 * 返回值：无
 * *************************/
void start_task(void *ptr)
{
    //优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	led_init();				//LED初始化
	key_init();				//按键初始化
	motor_init();			//电机初始化
	w25q64_init();			//W25Q64初始化
	at24c02_init();			//AT24C02初始化
    dma1_init();			//DMA初始化(SPI2发送)
	lcd_init();				//LCD屏幕初始化
	usart1_init(115200);	//USART1初始化	
    clock_init();			//RTC初始化
	NV400F_init();			//NV400F语音初始化
	MG200_init();			//MG200指纹初始化
	CY8CMBR3116_init();		//CY8CMBR3116电容按键初始化
	RC522_Init();			//RC522初始化
    usart2_init(115200);    //USART2初始化
    timer4_interrupt_ms(1);
	lcd_show_zk_str("正在启动中",40,104,32,0x0000,0xffff);

    //复位后按按键1初始化设置
    if(KEY1)
	{
		//zk_update();		//字库烧录
		printf("初始化设置\r\n");
		at24c02_write_byte(0,0);//将初始密码标志位设置为0
		at24c02_write_cross_page(23,10,"123456");//默认管理员密码
		at24c02_write_byte(34,6);//写入管理员密码长度
		MG200_erase_all();
		for(u8 i=35;i<=55;i++)
		{
			at24c02_write_byte(i,0xff);//将射频卡id设置为0xff
		}
		printf("初始化已完成\r\n");
	}
    /****************初始密码和初始网络连接（蓝牙连接）*******************************************/
    check_init_password();
    /*****************************************************************************************/


    /************************不使用蓝牙连接的方法**********************************************
	* if(WIFI_init() == 0)//如果连接上服务器
	* {
    *     //上报开门密码和管理员密码
    *     WIFI_report_password();
	* }
    ***************************************************************************************/
    //获取NTP服务器时间
    WIFI_get_time();
    //上报开门密码和管理员密码
    WIFI_report_password();



  
    
    //创建其他任务
    //TASK1：优先级为3
    xTaskCreate(open_page_task,			    //任务函数
                "open_page",			    //任务名字
                256,					    //任务栈深度（4倍大小）=1024
                NULL,					    //传递任务参数
                3,						    //任务优先级3
                &open_page_task_handle);	//任务句柄指针
    //TASK2：优先级为2
    xTaskCreate(admin_check_task,			//任务函数
                "admin_check",			    //任务名字
                256,					    //任务栈深度（4倍大小）=1024
                NULL,					    //传递任务参数
                2,						    //任务优先级2
                &admin_check_task_handle);//任务句柄指针
    //TASK3：优先级为1
    xTaskCreate(admin_page_task,			//任务函数
                "admin_page",			    //任务名字
                256,					    //任务栈深度（4倍大小）=1024
                NULL,					    //传递任务参数
                1,						    //任务优先级1
                &admin_page_task_handle);   //任务句柄指针
    //创建任务或其他功能完成就删除起始任务
    vTaskDelete(start_task_handle);
}