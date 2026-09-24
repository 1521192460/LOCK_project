#include "WIFI.h"

/********************************
 * 函数名：WIFI_send_data
 * 函数功能：发送数据并且响应
 * 函数参数：
 *           u8 *data：要发送的数据指针
 *           u8 timeout：响应时长
 * 函数返回值：
 *              0为响应成功
 *              1为响应失败
 * 函数说明：发送数据并且等待响应，超时时间为timeout毫秒
 *******************************/
u8 WIFI_send_data(u8 *cmd,u16 timeout)
{
  u16 cnt = 0;
  usart2_send_str(cmd);
  delay_ms(200);
  while(1)
  {
    //等待U2接收到wifi模块数据
    while(!u2.flag)
    {
      //每ms自增
      cnt++;
      delay_ms(1);
      if(cnt >= timeout)
      {
        //通信失败
        return 1;
      }
    }
    u2.flag = 0;
    //判断WIFI模块回复的数据是否存在"OK"
    if(strstr((const char*)u2.buff,"OK") != NULL)
    {
      return 0;
    }
  }

}







/********************************
 * 函数名：WIFI_init
 * 函数功能：初始化WIFI模块
 * 函数参数：无
 * 函数返回值：无
 * 函数功能：初始化WIFI模块，包括串口2初始化
 *******************************/
u8 WIFI_init(void)
{
    u8 ret = 0;
    //发送AT测试指令
    usart2_send_str("AT\r\n");
    delay_ms(200);
    usart2_send_str("ATE0\r\n");
    //恢复出厂设置
    ret = WIFI_send_data("AT+RESTORE\r\n",5000);
    if(ret != 0)
    {
        printf("恢复出厂设置失败\r\n");
        return 1;
    }
    printf("恢复出厂设置成功\r\n");
    delay_ms(5000);
    //设置WIFI模式
    ret = WIFI_send_data("AT+WMODE=1,0\r\n",5000);
    if(ret != 0)
    {
        printf("设置WIFI模式失败\r\n");
        return 1;
    }
    printf("设置WIFI模式成功\r\n");
    //设置自动连接
    ret = WIFI_send_data("AT+WAUTOCONN=1\r\n",5000);
    if(ret != 0)
    {
        printf("设置自动连接失败\r\n");
        return 1;
    }
    printf("设置自动连接成功\r\n");
    // 设置手机配网
    ret = WIFI_send_data("AT+WCONFIG=2,Roy\r\n",5000);
    if(ret != 0)
    {
        printf("设置手机配网失败\r\n");
        return 2;
    }
    printf("设置手机配网成功\r\n");
    lcd_clear(0,0,240,240,0xffff);
    lcd_show_zk_str("等待蓝牙配网",24,104,32,0x0000,0xffff);
    //等待WIFI连接成功
    while(strstr((const char*)u2.buff,"+EVENT:WIFI_GOT_IP") == NULL);
    while(strstr((const char*)u2.buff,"BLUFI_BLE_DISCONNECT") == NULL);
    //连接WIFI
    // ret = WIFI_send_data("AT+WJAP=Class8,12345678\r\n",10000);
    // if(ret != 0)
    // {
    //     printf("连接WIFI失败\r\n");
    //     return 2;
    // }
    // printf("连接WIFI成功\r\n");
    //设置sntp时区和服务器
    ret = WIFI_send_data("AT+SNTPTIMECFG=1,8,ntp1.aliyun.com,ntp2.aliyun.com,ntp3.aliyun.com\r\n",5000);
    if(ret != 0)
    {
        printf("设置sntp时区和服务器区失败\r\n");
        return 3;
    }
    printf("设置sntp时区和服务器区成功\r\n");
    // 设置服务器域名
    ret = WIFI_send_data("AT+MQTT=1,gz-3-mqtt.iot-api.com\r\n",5000);
    if(ret != 0)
    {
        printf("设置服务器域名失败\r\n");
        return 4;
    }
    printf("设置服务器域名成功\r\n");
    //设置端口
    ret = WIFI_send_data("AT+MQTT=2,1883\r\n",5000);
    if(ret != 0)
    { 
        printf("设置端口失败\r\n");
        return 5;
    }
    printf("设置端口成功\r\n");
    //设置连接方式
    ret = WIFI_send_data("AT+MQTT=3,1\r\n",5000);
    if(ret != 0)
    {
        printf("设置连接方式失败\r\n");
        return 6;
    }
    printf("设置连接方式成功\r\n");
    //设置用户client id
    ret = WIFI_send_data("AT+MQTT=4,bbccdd\r\n",5000);
    if(ret != 0)
    {
        printf("设置client id失败\r\n");
        return 7;
    }
    printf("设置client id成功\r\n");
    //设置MQTT 用户名
    ret = WIFI_send_data("AT+MQTT=5,n8kv9ig5vyh3xb08\r\n",5000);
    if(ret != 0)
    {
        printf("设置 MQTT 用户名失败\r\n");
        return 8;
    }
    printf("设置 MQTT 用户名成功\r\n");
    //设置设置 MQTT 密码
    ret = WIFI_send_data("AT+MQTT=6,Suf1dPEgoc\r\n",5000);
    if(ret != 0)
    {
        printf("设置 MQTT 密码失败\r\n");
        return 9;
    }
    printf("设置 MQTT 密码成功\r\n");
    //连接 MQTT 服务器
    ret = WIFI_send_data("AT+MQTT\r\n",5000);
    if(ret != 0)
    {
        printf("连接 MQTT 服务器失败\r\n");
        return 11;
    }
    printf("连接 MQTT 服务器成功\r\n");
    delay_ms(5000);
    ret = WIFI_send_data("AT+MQTTSUB=attributes/push,0\r\n",5000);
    if(ret != 0)
    {
        printf("订阅属性Topic失败\r\n");
        return 12;
    }   
    printf("订阅属性Topic成功\r\n");
    return 0;
}


/*************************
函数名:WIFI_ctrl
函数功能：wifi控制函数
函数参数：无
函数返回值：无
函数说明：
*************************/  
void WIFI_ctrl(void)
{
    u8 open_password[20] = {0};
    u8 admin_password[10] = {0};
    u8 len = 0;
    if(u2.flag == 1)
    {
        //显示云端下发数据
        //printf("%s\r\n",u2.buff);
        //清除接收数据标志位
        u2.flag = 0;
        if(strstr((const char*)u2.buff,"{\"dooropen\":1}") != NULL)
        {
            lcd_clear(0,0,240,240,0xffff);
            lcd_show_zk_str("开门成功，欢迎回家",0,0,32,0x0000,0xffff);
            delay_ms(100);
            NV400F_send_data(0x12);//欢迎回家语音
            LOCK_ON;
            delay_ms(2000);
            LOCK_OFF; 
            WIFI_send_data("AT+MQTTPUB=attributes,0,0,{\"dooropen\":0}\r\n",3000);
            ui_flag = 0;
            lcd_clear(0,0,240,240,0xffff);
        }
        if(strstr((const char*)u2.buff,"\"doorpwd\"") != NULL)
        {
            //定位到开门密码值位置并提取
            sscanf(strstr((const char*)u2.buff,"\"doorpwd\""),"\"doorpwd\":\"%[^\"]\"",open_password);
            len = strlen((char*)open_password);
            //写入开门密码到at24c02空间1-20
            at24c02_write_cross_page(1,len,open_password);
            //写入开门密码长度到at24c02空间22
            at24c02_write_byte(22,len);
        }
        if(strstr((const char*)u2.buff,"{\"adminpwd\":}") != NULL)
        {
            //定位到管理员密码值位置并提取
            sscanf(strstr((const char*)u2.buff,"\"adminpwd\""),"\"adminpwd\":\"%[^\"]\"",admin_password);
            len = strlen((char*)admin_password);
            //写入管理员密码到at24c02空间23-33
            at24c02_write_cross_page(23,len,admin_password);
            //写入管理员密码长度到at24c02空间34
            at24c02_write_byte(34,len);
        }
    }
}


/*************************
函数名:WIFI_report_password
函数功能：上报开门密码和管理员密码
函数参数：无
函数返回值：无
函数说明：
*************************/  
void WIFI_report_password(void)
{
    u8 open_password[20] = {0};
	u8 admin_password[10] = {0};
    static u8 open_len,admin_len;
    u8 report_password[100] = {0};
	at24c02_read_byte(22,&open_len);//读取开门密码长度
	at24c02_sequential_read(1,open_len,open_password);//读取开门密码 
	at24c02_read_byte(34,&admin_len);//读取管理员密码长度
	at24c02_sequential_read(23,admin_len,admin_password);//读取管理员密码
    //上报开门密码和管理员密码
    sprintf((char*)report_password,"AT+MQTTPUB=attributes,0,0,{\"doorpwd\":\"%s\"\\,\"adminpwd\":\"%s\"}\r\n",open_password,admin_password);
    WIFI_send_data(report_password,3000);
}


/*************************
* 函数名:WIFI_get_time
* 函数功能：获取时间日期
* 函数参数：无
* 函数返回值：无
* 函数说明：
*************************/  
void WIFI_get_time(void)
{
    const char *p = NULL;
    RTC_t t = {0};
    unsigned int h = 0, m = 0, s = 0;
    if(WIFI_send_data("AT+SNTPTIME?\r\n", 3000) != 0)
    {
        printf("获取时间失败\r\n");
        return;
    }
    printf("获取时间成功\r\n");
    p = strstr((const char*)u2.buff, "+SNTPTIME:");
    if(p == NULL) return;         //未找到时间字符串

    //获取时间字符数据
    sscanf(p, "+SNTPTIME:%*3s %*3s %*u %2u:%2u:%2u", &h, &m, &s);

    //将数据传递给hour,min,sec
    t.hour = (u8)h;
    t.min = (u8)m;
    t.sec = (u8)s;
    //将获取到的时间赋值给RTC_t结构体
    set_time(t);      // 时分秒进 RTC
}



/******************************
* 函数名:WIFI_config
* 函数功能：配置wifi功能
* 函数参数：无
* 函数返回值：无
* 函数说明：
*******************************/ 
void WIFI_config(void)
{
    lcd_clear(0,0,240,240,0xffff);
    lcd_show_zk_str("请连接蓝牙",40,104,32,0x0000,0xffff);
    NV400F_send_data(0x33);//请连接蓝牙语音
    //连接蓝牙
    WIFI_send_data("AT+WCONFIG=2,'Roy'\r\n",3000);
    while(1)
    {
        if(strstr((const char*)u2.buff,"BLUFI_BLE_CONNECT") != NULL)
            break;
        else if(strstr((const char*)u2.buff,"BLUFI_BLE_DISCONNECT") != NULL)
        {
            lcd_show_zk_str("蓝牙连接失败",24,104,32,0x0000,0xffff);
            NV400F_send_data(0x34);//蓝牙连接失败语音
            delay_ms(500);
            lcd_clear(0,0,240,240,0xffff);
            ui_flag = 0;
            return;
        }
    }
    //等待蓝牙配网成功
    lcd_show_zk_str("等待蓝牙配网",24,104,32,0x0000,0xffff);
    while(strstr((const char*)u2.buff,"+EVENT:WIFI_GOT_IP") == NULL);
    
    //重新连接MQTT服务器和订阅主题
    WIFI_send_data("AT+MQTT\r\n",5000);                 //连接MQTT
    while(strstr((char *)u2.buff,"+EVENT:MQTT_CONNECT") == NULL);//等待MQTT连接成功
    printf("MQTT连接成功\r\n");
    delay_ms(3000);
    WIFI_send_data("AT+MQTTSUB=attributes/push,0\r\n",3000);

    printf("订阅成功\r\n");
    lcd_show_zk_str("蓝牙配网成功",24,104,32,0x0000,0xffff);
    NV400F_send_data(0x35);//wifi配置成功语音
    WIFI_get_time();
    lcd_clear(0,0,240,240,0xffff);
    ui_flag = 0;
    return;


    
}
