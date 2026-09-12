#include "NV400F.h"


/**************************************************
 * 函数名:NV400F_init
 * 函数功能:密码开门
 * 函数参数：
 * 函数返回值：无
 * 函数说明：
 *          VOICE_BUSY---PC4---输入模式
 *          VOICE_DATA---PC5---通用推挽输出
 ***************************************************/
void NV400F_init(void)
{
    //时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    //初始化VOIC_BUSY为推挽输出
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //初始化VOICE_DATA为输入模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //初始化data为高电平
    VOICE_DATA_H;
    //初始音量
    NV400F_send_data(0xea);
    //延时1ms
    delay_ms(10);
}


/**************************************************
 * 函数名:NV400F_start
 * 函数功能:语音输入起始信号
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 ***************************************************/
void NV400F_start(void)
{
    //拉低数据线
    VOICE_DATA_L;
    //延时5ms
    delay_ms(5);
}

/**************************************************
 * 函数名:NV400F_send_data
 * 函数功能:语音输入数据
 * 函数参数：u8 cmd--要发送的指令
 * 函数返回值：无
 * 函数说明：
 ***************************************************/
void NV400F_send_data(u8 cmd)
{
    //起始信号
    NV400F_start();
    for(int i = 0;i < 8;i++)
    {
        if(cmd & (0x01 << i))
        {
            VOICE_DATA_H;
            delay_us(1200);
            VOICE_DATA_L;
            delay_us(400);
        }
        else
        {
            VOICE_DATA_H;
            delay_us(400);
            VOICE_DATA_L;
            delay_us(1200);
        }
    }
    VOICE_DATA_H;
    delay_ms(7);
}
