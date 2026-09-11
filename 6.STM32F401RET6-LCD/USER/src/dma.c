#include "dma.h"

void dma_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStruct = {0};
    DMA_InitStruct.DMA_Channel = DMA_Channel_0;                             //使用DMA通道0
    DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;                    //内存到外设（ram->外设）
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;            //外设地址
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_MemoryDataSize_HalfWord;    //外设数据大小16位
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;           //外设地址不自增
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;        //外设不支持突发模式
    DMA_InitStruct.DMA_Memory0BaseAddr = NULL;                              //内存地址为NULL
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;        //内存数据大小16位
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                    //内存地址自增
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;                //内存不支持突发模式
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;                              //单次传输模式
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;                        //高优先级
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;                    //FIFO模式禁用   
    DMA_Init(DMA1_Stream4, &DMA_InitStruct);

    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
}
