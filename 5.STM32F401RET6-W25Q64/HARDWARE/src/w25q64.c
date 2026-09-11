#include "w25q64.h"


/************************************
 * 函数名：w25q64_init
 * 函数功能：w25q64初始化
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 * *********************************/
void w25q64_init(void)
{
    w25q64_pin_init();
    //FLASH_CS高电平
    W25Q64_CS_H;
}


/************************************
 * 函数名：w25q64_read_id
 * 函数功能：w25q64读取ID
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 * *********************************/
void w25q64_read_id(void)
{
    u16 id = 0; 
    //拉低片选
    W25Q64_CS_L;
    //发送指令0x90
    w25q64_transfer_data(0x90);
    //发送0x000000
    w25q64_transfer_data(0x00);
    w25q64_transfer_data(0x00);
    w25q64_transfer_data(0x00);
    //接收8位产商ID
    id = w25q64_transfer_data(0xFF);
    //接收8位设备ID
    id = id << 8 | w25q64_transfer_data(0xFF);
    //拉高片选结束通信
    W25Q64_CS_H;
    printf("id = 0x%X\r\n",id);
}

/************************************
 * 函数名：w25q64_wirte_enable
 * 函数功能：w25q64写使能
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：发送指令0x06，写使能
 * *********************************/
void w25q64_wirte_enable(void)
{
    //拉低片选建立通信
    W25Q64_CS_L;
    //发送指令0x06
    w25q64_transfer_data(0x06);
    //拉高片选结束通信
    W25Q64_CS_H;
}

/************************************
 * 函数名：w25q64_read_busy
 * 函数功能：w25q64读状态寄存器，提取busy位
 * 函数参数：无
 * 函数返回值：u8---busy位的值  
 * 函数说明：发送指令0x05，读状态寄存器，提取busy位，返回busy位的值
 *          busy位：0-空闲，1-忙
 * *********************************/
u8 w25q64_read_busy(void)
{
    u8 status = 0;
    //拉低片选建立通信
    W25Q64_CS_L;
    //发送指令0x05
    w25q64_transfer_data(0x05);
    //读取状态寄存器
    status = w25q64_transfer_data(0xFF);
    //拉高片选结束通信
    W25Q64_CS_H;
    //提取状态寄存器的第0位,1-忙,0-空闲
    if(status & 0x01)
    {
        return 1;
    }
    return 0;
}


/************************************
 * 函数名：w25q64_sector_erase
 * 函数功能：w25q64扇区擦除
 * 函数参数：u32 addr
 * 函数返回值：u8
 * 函数说明：发送指令0x20，读状态寄存器，提取busy位，返回busy位的值
 * *********************************/
void w25q64_sector_erase(u32 addr)
{
    //写使能
    w25q64_wirte_enable();
    //等待忙位为0
    while(w25q64_read_busy());
    //拉低片选建立通信
    W25Q64_CS_L;
    //发送指令0x20
    w25q64_transfer_data(0x20);
    //发送24位地址，高位先出
    w25q64_transfer_data(addr >> 16);
    w25q64_transfer_data(addr >> 8);
    w25q64_transfer_data(addr);
    //拉高片选结束通信
    W25Q64_CS_H;
    //等待不忙
    while(w25q64_read_busy());
}


/************************************
 * 函数名：w25q64_write_page
 * 函数功能：w25q64页写
 * 函数参数：u32 addr、u8 data_size、u8 *data
 * 函数返回值：无
 * 函数说明：发送指令0x02，页写
 * *********************************/
void w25q64_write_page(u32 addr,u8 data_size,u8 *data)
{
    //如果跨页，结束函数：判断写之前的页号和写之后的页号是否相同
    if(addr / 256 != (addr + data_size - 1) / 256)
    {
        return;
    }
    //写使能
    w25q64_wirte_enable();
    //等待忙位为0
    while(w25q64_read_busy());
    //拉低片选建立通信
    W25Q64_CS_L;
    //发送指令0x02
    w25q64_transfer_data(0x02);
    //发送24位地址，高位先出
    w25q64_transfer_data(addr >> 16);
    w25q64_transfer_data(addr >> 8);
    w25q64_transfer_data(addr);
    //发送数据
    while(data_size--)
    {
        w25q64_transfer_data(*data++);
    }
    //拉高片选结束通信
    W25Q64_CS_H;
    //等待不忙
    while(w25q64_read_busy());
}

/************************************
 * 函数名：w25q64_read_data
 * 函数功能：w25q64页读
 * 函数参数：u32 addr、u8 data_size、u8 *data
 * 函数返回值：无
 * 函数说明：发送指令0x03，页读
 * *********************************/
void w25q64_read_data(u32 addr,u8 data_size,u8 *data)
{
    //拉低片选
    W25Q64_CS_L;
    //发送0x03
    w25q64_transfer_data(0x03);
    //发送24位地址，高位先出
    w25q64_transfer_data(addr >> 16);
    w25q64_transfer_data(addr >> 8);
    w25q64_transfer_data(addr);
    //接收数据
    while(data_size--)
    {
        *data++ = w25q64_transfer_data(0xFF);
    }
    //拉高片选
    W25Q64_CS_H;

}


/************************************
 * 函数名：w25q64_cross_page_write
 * 函数功能：w25q64跨页写
 * 函数参数：u32 addr、u8 data_size、u8 *data
 * 函数返回值：u8
 * 函数说明：数据大于当前页字节剩余空间跨页写
************************************/
void w25q64_cross_page_write(u32 addr,u16 data_size,u8 *data)
{
    //循环写入数据
    while(1)
    {
        //计算写入剩余空间：当前页字节剩余空间
        u16 less_size = 256 - (addr % 256);
        //判断如果是否跨页,如果跨页就跨页写，不跨页就页写
        if(data_size > less_size)
        {
            //先写入当前页字节剩余空间的数据
            w25q64_write_page(addr,less_size,data);
            //地址跨页
            addr += less_size;
            //数据长度跨页
            data_size -= less_size;
            //数据指针跨页
            data += less_size;
        }
        else
        {
            //页写
            w25q64_write_page(addr,data_size,data);
            return;
        }
    }
}


/************************************
 * 函数名：w25q64_half_block_erase
 * 函数功能：w25q64半块擦除
 * 函数参数：u32 addr
 * 函数返回值：u8
 * 函数说明：发送指令0x52，半块擦除
 * *********************************/
void w25q64_half_block_erase(u32 addr)
{
    //写使能
    w25q64_wirte_enable();
    //等待忙位为0
    while(w25q64_read_busy());
    //拉低片选
    W25Q64_CS_L;
    //发送指令0x52
    w25q64_transfer_data(0x52); 
    //发送24位地址，高位先出
    w25q64_transfer_data(addr >> 16);
    w25q64_transfer_data(addr >> 8);
    w25q64_transfer_data(addr);
    //拉高片选结束通信
    W25Q64_CS_H;
    //等待不忙
    while(w25q64_read_busy());
}


/************************************
 * 函数名：w25q64_block_erase
 * 函数功能：w25q64全块擦除
 * 函数参数：u32 addr
 * 函数返回值：u8
 * 函数说明：发送指令0xd8，全块擦除
 * *********************************/
void w25q64_block_erase(u32 addr)
{
    //写使能
    w25q64_wirte_enable();
    //等待忙位为0
    while(w25q64_read_busy());
    //拉低片选
    W25Q64_CS_L;
    //发送指令0xd8
    w25q64_transfer_data(0xd8); 
    //发送24位地址，高位先出
    w25q64_transfer_data(addr >> 16);
    w25q64_transfer_data(addr >> 8);
    w25q64_transfer_data(addr);
    //拉高片选结束通信
    W25Q64_CS_H;
    //等待不忙
    while(w25q64_read_busy());
}


/************************************
 * 函数名：w25q64_chip_erase
 * 函数功能：w25q64全芯片擦除
 * 函数参数：void
 * 函数返回值：void
 * 函数说明：发送指令0xc7或者0x60，全芯片擦除
 * *********************************/
void w25q64_chip_erase(void)
{
    //写使能
    w25q64_wirte_enable();
    //等待忙位为0
    while(w25q64_read_busy());
    //拉低片选
    W25Q64_CS_L;
    //发送指令0xc7或者0x60
    w25q64_transfer_data(0xc7);
    //拉高片选结束通信
    W25Q64_CS_H;
    //等待不忙
    while(w25q64_read_busy());
}