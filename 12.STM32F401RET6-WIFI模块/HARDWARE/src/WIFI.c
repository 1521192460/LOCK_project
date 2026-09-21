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
void WIFI_init(void)
{
    u8 ret = 0;
    //串口2初始化
    usart2_init(115200);
    //发送AT测试指令
    usart2_send_str("AT\r\n");
    //关闭回显
    usart2_send_str("ATE0\r\n");
    //恢复出厂设置
    usart2_send_str("AT+RST\r\n");
    delay_ms(2000);
    //设置WIFI模式
    ret = WIFI_send_data("AT+WMODE=1,1\r\n",5000);
    if(ret != 0)
    {
        printf("设置WIFI模式失败\r\n");
        return;
    }
    printf("设置WIFI模式成功\r\n");
    //连接WIFI
    ret = WIFI_send_data("AT+WJAP=Class8,12345678\r\n",20000);
    if(ret != 0)
    {
        printf("连接WIFI失败\r\n");
        return;
    }
    printf("连接WIFI成功\r\n");
    //设置sntp时区和服务器
    ret = WIFI_send_data("AT+SNTPTIMECFG=1,8,ntp1.aliyun.com,ntp2.aliyun.com,ntp3.aliyun.com\r\n",5000);
    if(ret != 0)
    {
        printf("设置sntp时区和服务器区失败\r\n");
        return;
    }
    printf("设置sntp时区和服务器区成功\r\n");
    //设置服务器域名
    ret = WIFI_send_data("AT+MQTT=1,gz-3-mqtt.iot-api.com\r\n",5000);
    if(ret != 0)
    {
        printf("设置服务器域名失败\r\n");
        return;
    }
    printf("设置服务器域名成功\r\n");
    //设置端口
    ret = WIFI_send_data("AT+MQTT=2,1883\r\n",5000);
    if(ret != 0)
    {
        printf("设置端口失败\r\n");
        return;
    }
    printf("设置端口成功\r\n");
    //设置连接方式
    ret = WIFI_send_data("AT+MQTT=3,1\r\n",5000);
    if(ret != 0)
    {
        printf("设置连接方式失败\r\n");
        return;
    }
    printf("设置连接方式成功\r\n");
    //设置用户client id
    ret = WIFI_send_data("AT+MQTT=4,2233344\r\n",5000);
    if(ret != 0)
    {
        printf("设置client id失败\r\n");
        return;
    }
    printf("设置client id成功\r\n");
    //设置MQTT 用户名
    ret = WIFI_send_data("AT+MQTT=5,n8kv9ig5vyh3xb08\r\n",5000);
    if(ret != 0)
    {
        printf("设置 MQTT 用户名失败\r\n");
        return;
    }
    printf("设置 MQTT 用户名成功\r\n");
    //设置设置 MQTT 密码
    ret = WIFI_send_data("AT+MQTT=6,Suf1dPEgoc\r\n",5000);
    if(ret != 0)
    {
        printf("设置 MQTT 密码失败\r\n");
        return;
    }
    printf("设置 MQTT 密码成功\r\n");
    //查询 MQTT 连接状态
    ret = WIFI_send_data("AT+MQTT?\r\n",5000);
    if(ret != 0)
    {
        printf("查询 MQTT 连接状态失败\r\n");
        return;
    }
    //连接 MQTT 服务器
    ret = WIFI_send_data("AT+MQTT\r\n",5000);
    if(ret != 0)
    {
        printf("连接 MQTT 服务器失败\r\n");
        return;
    }
    printf("连接 MQTT 服务器成功\r\n");
    delay_ms(1500);
    //订阅主题
    WIFI_send_data("AT+MQTTSUB=attributes/push,0\r\n",3000);
    //打开回显
    usart2_send_str("ATE1\r\n");
    //查询sntp时间
    WIFI_send_data("AT+SNTPTIME?\r\n",5000);

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
  if(u2.flag == 1)
  {
    //显示云端下发数据
    printf("%s\r\n",u2.buff);
    //清除接收数据标志位
    u2.flag = 0;
    if(strstr((const char*)u2.buff,"{\"dooropen\":1}") != NULL)
    {
        NV400F_send_data(0x12);//欢迎回家
        LOCK_ON;
        delay_ms(2000);
        LOCK_OFF;
        delay_ms(2000); 
        WIFI_send_data("AT+MQTTPUB=attributes,0,0,{\"dooropen\":0}\r\n",3000);
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
    u8 report_password[80] = {0};
	at24c02_read_byte(22,&open_password[19]);//读取开门密码长度
	at24c02_sequential_read(1,open_password[19],open_password);//读取开门密码
	at24c02_read_byte(34,&admin_password[9]);//读取管理员密码长度
	at24c02_sequential_read(23,admin_password[9],admin_password);//读取管理员密码
    //上报开门密码和管理员密码
    sprintf((char*)report_password,"AT+MQTTPUB=attributes,0,0,{\"doorpwd\":\"%s\"\\,\"adminpwd\":\"%s\"}\r\n",open_password,admin_password);
    WIFI_send_data(report_password,3000);
}
