#include "dma.h"



/*******************************
 * 函数名：dma_init
 * 函数功能：初始化DMA通道0通道4
 * 函数参数：void
 * 函数返回值：void
 * 函数说明：    初始化DMA1通道0数据流4，用于SPI2数据传输
 ******************************/
void dma1_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStruct={0};
	DMA_InitStruct.DMA_Channel = DMA_Channel_0;              // 通道0
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(SPI2->DR); // SPI 数据寄存器地址
	DMA_InitStruct.DMA_Memory0BaseAddr = NULL; // 内存地址
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;     // 存储器到外设
	DMA_InitStruct.DMA_BufferSize = 0;                       // 传输数量动态设置
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址不递增
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;    // 内存地址递增
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 8bit
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // 8bit
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;               // 单次传输模式
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;     //优先级
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;      // 禁用 FIFO
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;//FIFO状态
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;      //突发配置
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//突发配置
	DMA_Init(DMA1_Stream4, &DMA_InitStruct);
}


/*******************************
 * 函数名：dma1_spi2_tx
 * 函数功能：使用DMA发送数据到SPI2
 * 函数参数：
 *           u8 *buf：要发送的数据指针
 *           u16 len：数据长度
 * 函数返回值：void
 * 函数说明：    使用DMA发送数据到SPI2，数据发送完成后，CS高电平，设置DC=data
 *******************************/
void dma1_spi2_tx(u8 *buf, u16 len)
{
    DMA_Cmd(DMA1_Stream4, DISABLE);              // 禁止DMA传输
    DMA1_Stream4->M0AR = (uint32_t)buf;          // 设置内存地址
    DMA_SetCurrDataCounter(DMA1_Stream4, len);   // 设置传输数量
    DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4); // 清除传输完成标志位
    DMA_Cmd(DMA1_Stream4, ENABLE);               // 启动DMA传输

    while(DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4) == RESET); //等待传输完成
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY));            //等待SPI2空闲
}





