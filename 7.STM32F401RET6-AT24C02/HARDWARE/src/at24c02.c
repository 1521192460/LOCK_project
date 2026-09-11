#include "at24c02.h"



/***********************************
 * 函数名:at24c02_init
 * 函数功能:初始化at24c02
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 ***********************************/
void at24c02_init(void)
{
    //at24c02引脚初始化
    iic3_pin_init();
}



/***********************************
 * 函数名:at24c02_write_byte
 * 函数功能:at24c02字节写
 * 函数参数：u8 addr--写入地址
 *          u8 data--写入的数据
 * 函数返回值：u8---接收应答信号
 * 函数说明：
 *          每次发送数据后，都需要接收应答信号，判断是否成功响应
 *          非0为非应答信号，0为应答信号
************************************/
u8 at24c02_write_byte(u8 addr,u8 data)
{
    //起始信号
    iic3_start();
    //发送器件写地址
    iic3_send_data(AT24C02_ADDR_WRITE);
    //判断是否应答
    if(iic3_rec_ack()!=0)
    {
        iic3_stop();
        return 1;
    }
    //发送写入地址
    iic3_send_data(addr);
    if(iic3_rec_ack()!=0)
    {
        iic3_stop();
        return 2;
    }
    //发送数据函数
    iic3_send_data(data);
    if(iic3_rec_ack()!=0)
    {
        iic3_stop();
        return 3;
    }
    //发送结束信号
    iic3_stop();
    //跳过写周期，等待at24c02写入完成
    delay_ms(5);  
    return 0;
}


/***********************************
 * 函数名:at24c02_read_byte
 * 函数功能:at24c02字节读取
 * 函数参数：u8 addr--读取地址  
 *          u8 *data--要读取的数据
 * 函数返回值：u8---接收应答信号
 * 函数说明：
 *          每次发送数据后，都需要接收应答信号，判断是否成功响应
 *          非0为非应答信号，0为应答信号
************************************/
u8 at24c02_read_byte(u8 addr,u8 *data)
{
    iic3_start();//起始信号
    iic3_send_data(AT24C02_ADDR_WRITE);//发送写方向地址
    if(iic3_rec_ack()!=0)//接收应答
    {
        iic3_stop();
        return 1;
    }
    iic3_send_data(addr);//发送读取地址
    if(iic3_rec_ack()!=0)//接收应答
    {
        iic3_stop();
        return 2;
    }
    iic3_start();//起始信号
    iic3_send_data(AT24C02_ADDR_READ);//发送读方向地址
    if(iic3_rec_ack()!=0)//接收应答
    {
        iic3_stop();
        return 3;
    }
    *data = iic3_rec_data();//接收数据
    iic3_send_ack(AT24C02_NACK);//发送非应答信号
    iic3_stop();//发送结束信号
    return 0;
}


/***********************************
 * 函数名:at24c02_write_page
 * 函数功能:at24c02页写
 * 函数参数：u8 addr--写入地址
 *          u8 size--数据大小
 *          u8 *data--要写入的数据
 * 函数返回值：u8---ack--接收应答信号
 * 函数说明：
 *          每次发送数据后，都需要接收应答信号，判断是否成功响应
 *          1为非应答信号，0为应答信号
************************************/
void at24c02_write_page(u8 addr,u8 size,u8 *data)
{
    iic3_start();//起始信号
    iic3_send_data(AT24C02_ADDR_WRITE);//发送写方向地址
    iic3_rec_ack();
    iic3_send_data(addr);//发送写入地址
    iic3_rec_ack();
    while(size--)
    {
        iic3_send_data(*data++);//发送数据
        iic3_rec_ack();
    }
    iic3_stop();//发送结束信号
    delay_ms(5);//跳过写周期，等待at24c02写入完成
}


/***********************************
 * 函数名:at24c02_sequential_read
 * 函数功能:at24c02顺序读
 * 函数参数：u8 addr--读取地址  
 *          u8 size--数据大小
 *          u8 *data--要读取的数据
 * 函数返回值：u8---ack--接收应答信号
 * 函数说明：
 *          每次发送数据后，都需要接收应答信号，判断是否成功响应
 *          非0为非应答信号，0为应答信号
************************************/
void at24c02_sequential_read(u8 addr,u8 size,u8 *data)
{

    iic3_start();//起始信号
    iic3_send_data(AT24C02_ADDR_WRITE);//发送写方向地址
    iic3_rec_ack();
    iic3_send_data(addr);//发送读取地址
    iic3_rec_ack();
    iic3_start();//起始信号
    iic3_send_data(AT24C02_ADDR_READ);//发送读方向地址
    iic3_rec_ack();
    while(size--)
    {
        *data++ = iic3_rec_data();//接收数据
        if(size == 0)
        {
            iic3_send_ack(AT24C02_NACK);//发送不应答信号
        }
        else
        {
            iic3_send_ack(AT24C02_ACK);//发送应答信号
        }
    }
    iic3_stop();//发送结束信号
}


/***********************************
 * 函数名:at24c02_write_cross_page
 * 函数功能:at24c02跨页写
 * 函数参数：u8 addr--写入地址
 *          u8 size--数据大小
 *          u8 *data--要写入的数据
 * 函数返回值：u8---ack--接收应答信号
 * 函数说明：
 *          每次发送数据后，都需要接收应答信号，判断是否成功响应
 *          1为非应答信号，0为应答信号
************************************/
void at24c02_write_cross_page(u8 addr,u16 size,u8 *data)
{
    //循环写入数据
    while(1)
    {
        //计算写入剩余空间：当前页字节剩余空间
        u8 less_size = 8 - (addr % 8);
        //判断如果是否跨页,如果跨页就跨页写，不跨页就页写
        if(size > less_size)
        {
            //先写入当前页字节剩余空间的数据
            at24c02_write_page(addr,less_size,data);
            //地址跨页
            addr += less_size;
            //数据长度跨页
            size -= less_size;
            //数据指针跨页
            data += less_size;
        }
        else
        {
            //页写
            at24c02_write_page(addr,size,data);
            return;
        }
    }
}