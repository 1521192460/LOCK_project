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
 * 函数说明:
 *          LCD_LEDK--------PB1------通用推挽输出
 *          LCD_RESET-------PB10-----通用推挽输出
 *          LCD_CS----------PB12-----通用推挽输出
 *          LCD_SPI2_SCLK---PB13-----复用推挽输出
 *          LCD_D/C---------PB14-----通用推挽输出
 *          LCD_SPI2_MOSI---PB15-----复用推挽输出
 ******************************/
void lcd_pin_init(void)
{
    /*时钟使能*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    /*GPIO初始化*/
    //LCD_RESET、LCD_LEDK、LCD_CS初始化
    GPIO_InitTypeDef GPIO_InitStruct = {0}; 
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_14;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

    //LCD_SPI2_SCLK、LCD_D/C、LCD_SPI2_MOSI初始化
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);
    GPIO_Init(GPIOB,&GPIO_InitStruct);

    /*SPI2初始化*/
    SPI_InitTypeDef SPI_InitStruct = {0};
    SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPI2,&SPI_InitStruct);
    //使能SPI2
    SPI_Cmd(SPI2,ENABLE);

    //拉高片选
    LCD_CS_H;
    //RES拉高
    LCD_RESET_H;
}


/******************************
 * 函数名:lcd_transfer_data
 * 函数功能:lcd交换数据
 * 函数参数：u8 data
 * 函数返回值：void
 * 函数说明:
 ******************************/
void lcd_transfer_data(u8 data)
{
    //等待上一个字节发送完成
    while(!(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)));
    //发送字节
    SPI_I2S_SendData(SPI2,data);
    //等待不忙
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY));
}