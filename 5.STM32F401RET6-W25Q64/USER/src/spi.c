#include "spi.h"



/******************************
 * 函数名:w25q64_pin_init
 * 函数功能:w25q64引脚初始化---spi1
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：MODE0模式
 *          pa4---FLASH_CS-----通用推挽输出
 *          PA5---FLASH_SCLK---复用推挽输出
 *          PA6---FLASH_MISO---复用输入
 *          PA7---FLASH_MOSI---复用推挽输出
 ******************************/
void w25q64_pin_init(void)
{
    //时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStruct = {0}; 
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
    //SPI1初始化
    SPI_InitTypeDef SPI_InitStruct = {0};
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPI1,&SPI_InitStruct);

    SPI_Cmd(SPI1,ENABLE);
}



/******************************
 * 函数名:w25q64_transfer_data
 * 函数功能:w25q64交换数据
 * 函数参数：u8 data
 * 函数返回值：u8
 * 函数说明：
 ******************************/
u8 w25q64_transfer_data(u8 data)
{
    //等待上一个字节发送完成
    while(!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)));
    //发送字节
    SPI_I2S_SendData(SPI1,data);
    //等待接收完成
    while(!(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)));
    //接收接收缓冲区数据
    u8 ret = SPI_I2S_ReceiveData(SPI1);
    return ret;
}



/******************************
 * 函数名:lcd_pin_init
 * 函数功能:lcd引脚初始化---spi
 * 函数参数：void
 * 函数返回值：void
 * 函数说明：
 *          
 ******************************/
void lcd_pin_init(void)
{


}