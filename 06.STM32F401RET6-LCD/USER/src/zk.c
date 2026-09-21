#include "zk.h"


/**************************************
 * 函数名：zk_init
 * 函数功能：烧录字库
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
***************************************/
u8 file_flag = 0;   //0：未发送文件，1：已发送文件
u8 update_flag = 1; //0:已经烧录完成，1:未烧录完成
void zk_update(void)
{
    //在w25q64中清除1.82M字节（字库文件大小）
    printf("正在清除\r\n");
    for(u8 i=0;i<32;i++)
    {
        w25q64_block_erase(i*65536);
    }
    printf("清除完成\r\n");
    printf("等待上传文件\r\n");
    //等待串口发送字库文件
    while(!file_flag);
    
    printf("正在烧录\r\n");

    //等待字库烧录完成
    while(update_flag);

    printf("烧录完成\r\n");
 
}
