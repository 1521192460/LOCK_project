#include "main.h"




int main()
{
	u8 key;
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	led_init();
	key_init();
	motor_init();
	w25q64_init();
	usart1_init(115200);

	u8 write_size[] = "";
	u8 read_data[sizeof(write_size)] = {0};
	printf("正在擦除...\r\n");
	w25q64_sector_erase(0x000000);
	printf("擦除完成\r\n");
	//往250号地址写入数据
	w25q64_cross_page_write(0,sizeof(write_size),write_size);
	printf("写入完成\r\n");
	//读出数据
	w25q64_read_data(0,sizeof(write_size),read_data);
	printf("读出数据：%s\r\n",read_data);
	while(1)
	{	

	}

}


