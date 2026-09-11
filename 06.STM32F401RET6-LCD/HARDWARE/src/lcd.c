#include "lcd.h"
#include "image.h"

/*****************************
 * 函数名:lcd_send_cmd
 * 函数功能:发送命令lcd命令
 * 函数参数：u8 cmd
 * 函数返回值：void
 * 函数说明:
 *          发送命令lcd命令
 ***************************** */
void lcd_send_cmd(u8 cmd)
{
    //拉低片选
    LCD_CS_L;
    //拉低DC发送命令
    LCD_CMD;
    //发送命令
    lcd_transfer_data(cmd);
    //拉高片选
    LCD_CS_H;
}



/*****************************
 * 函数名:lcd_send_8bit_data
 * 函数功能:发送数据lcd数据
 * 函数参数：u8 data
 * 函数返回值：void
 * 函数说明:
 *          发送8位数据lcd数据
 ***************************** */
void lcd_send_8bit_data(u8 data)
{
    //拉低片选
    LCD_CS_L;
    //拉低DC发送命令数据
    LCD_DATA;
    //发送数据
    lcd_transfer_data(data);
    //拉高片选
    LCD_CS_H;
}


/*****************************
 * 函数名:lcd_send_8bit_data
 * 函数功能:发送数据lcd数据
 * 函数参数：u16 data
 * 函数返回值：void
 * 函数说明:
 *          发送16位数据lcd数据
 ***************************** */
void lcd_send_16bit_data(u16 data)
{
    //拉低片选
    LCD_CS_L;
    //拉低DC发送命令数据
    LCD_DATA;
    //发送高8位数据
    lcd_transfer_data(data>>8);
    //发送低8位数据
    lcd_transfer_data(data);
    //拉高片选
    LCD_CS_H;
}

/*****************************
 * 函数名:lcd_init
 * 函数功能:lcd初始化
 * 函数参数：void
 * 函数返回值：void
 * 函数说明:
 ***************************** */
void lcd_init(void)
{
    //管脚初始化
    lcd_pin_init();
    //复位
    LCD_RESET_L;
    delay_ms(100);
    LCD_RESET_H;
    delay_ms(100);
    //厂家命令
   	lcd_send_cmd(0x36);
	lcd_send_8bit_data(0x00);

	lcd_send_cmd(0x3A); 
	lcd_send_8bit_data(0x05);

	lcd_send_cmd(0xB2);
	lcd_send_8bit_data(0x0C);
	lcd_send_8bit_data(0x0C);
	lcd_send_8bit_data(0x00);
	lcd_send_8bit_data(0x33);
	lcd_send_8bit_data(0x33); 

	lcd_send_cmd(0xB7); 
	lcd_send_8bit_data(0x35);  

	lcd_send_cmd(0xBB);
	lcd_send_8bit_data(0x19);

	lcd_send_cmd(0xC0);
	lcd_send_8bit_data(0x2C);

	lcd_send_cmd(0xC2);
	lcd_send_8bit_data(0x01);

	lcd_send_cmd(0xC3);
	lcd_send_8bit_data(0x12);   

	lcd_send_cmd(0xC4);
	lcd_send_8bit_data(0x20);  

	lcd_send_cmd(0xC6); 
	lcd_send_8bit_data(0x0F);    

	lcd_send_cmd(0xD0); 
	lcd_send_8bit_data(0xA4);
	lcd_send_8bit_data(0xA1);

	lcd_send_cmd(0xE0);
	lcd_send_8bit_data(0xD0);
	lcd_send_8bit_data(0x04);
	lcd_send_8bit_data(0x0D);
	lcd_send_8bit_data(0x11);
	lcd_send_8bit_data(0x13);
	lcd_send_8bit_data(0x2B);
	lcd_send_8bit_data(0x3F);
	lcd_send_8bit_data(0x54);
	lcd_send_8bit_data(0x4C);
	lcd_send_8bit_data(0x18);
	lcd_send_8bit_data(0x0D);
	lcd_send_8bit_data(0x0B);
	lcd_send_8bit_data(0x1F);
	lcd_send_8bit_data(0x23);

	lcd_send_cmd(0xE1);
	lcd_send_8bit_data(0xD0);
	lcd_send_8bit_data(0x04);
	lcd_send_8bit_data(0x0C);
	lcd_send_8bit_data(0x11);
	lcd_send_8bit_data(0x13);
	lcd_send_8bit_data(0x2C);
	lcd_send_8bit_data(0x3F);
	lcd_send_8bit_data(0x44);
	lcd_send_8bit_data(0x51);
	lcd_send_8bit_data(0x2F);
	lcd_send_8bit_data(0x1F);
	lcd_send_8bit_data(0x1F);
	lcd_send_8bit_data(0x20);
	lcd_send_8bit_data(0x23);

	lcd_send_cmd(0x21); 
	lcd_send_cmd(0x11); 
	lcd_send_cmd(0x29);  

    //打开背光
    LCD_LEDK_ON;

    //清屏
    lcd_clear(0,0,240,240,0xffff);		
}


/*******************************
 * 函数名:lcd_set_position
 * 函数功能:lcd显示位置函数
 * 函数参数：u16 xs          起始x坐标
 *          u16 ys          起始y坐标
 *          u16 xe          结束x坐标
 *          u16 ye          结束y坐标
 * 函数返回值：void
 * 函数说明:
 *******************************/
void lcd_set_position(u16 xs,u16 ys,u16 xe,u16 ye)
{
    //设置x坐标
    lcd_send_cmd(0x2A);
    lcd_send_16bit_data(xs);
    lcd_send_16bit_data(xe-1);
    //设置y坐标
    lcd_send_cmd(0x2B);
    lcd_send_16bit_data(ys);
    lcd_send_16bit_data(ye-1);
    //写入数据
    lcd_send_cmd(0x2C);
}   


/*******************************
 * 函数名:lcd_clear
 * 函数功能:lcd清屏函数（清空指定区域）
 * 函数参数：u16 xs          起始x坐标
 *          u16 ys          起始y坐标
 *          u16 xe          结束x坐标
 *          u16 ye          结束y坐标
 *          u16 color          清屏颜色
 * 函数返回值：void
 * 函数说明:
 *******************************/
void lcd_clear(u16 xs,u16 ys,u16 xe,u16 ye,u16 color)
{
    //设置位置
    lcd_set_position(xs,ys,xe,ye);
    //发送颜色
    for(u16 i=0;i<(xe-xs)*(ye-ys);i++)
    {
        lcd_send_16bit_data(color);
    }
}


/*******************************
 * 函数名:lcd_show_image
 * 函数功能:lcd显示图片函数（显示指定图片）
 * 函数参数：u8 *image          图片指针，指向图片数据的起始地址
 *          u8 x          图片起始x坐标
 *          u8 y          图片起始y坐标
 * 函数返回值：void
 * 函数说明:图片数据内每个数据颜色都是8位，两个字节表示一个像素的颜色
 *******************************/
void lcd_show_image(u8 *image,u8 x,u8 y)
{
	//提取图片宽度和高度
	u16 width = image[2] << 8 | image[3];
	u16 height = image[4] << 8 | image[5];
	//设置位置
	lcd_set_position(x,y,width+x,y+height);
	//发送颜色
	for(u16 i=0;i<width*height;i++)
	{
		//发送颜色：高8位在前，低8位在后
		lcd_send_16bit_data(image[8+2*i]<<8|(image[8+2*i+1]));
	}
}



/*************************************************取模方式显示***********************************************************************/

/*******************************
 * 函数名:lcd_draw_point
 * 函数功能:lcd画点函数
 * 函数参数：
 *          u8 x          字体起始x坐标
 *          u8 y          字体起始y坐标
 *          u16 color          字体颜色
 * 函数返回值：void
 * 函数说明:
 *******************************/
void lcd_draw_point(u8 x,u8 y,u16 color)
{
	//设置一个像素点坐标
	lcd_set_position(x,y,x+1,y+1);
	//发送颜色
	lcd_send_16bit_data(color);
}


/*******************************
 * 函数名:lcd_show_eng
 * 函数功能:lcd显示英文函数
 * 函数参数：u8 *font        字体指针，指向字体数据的起始地址
 *          u8 x          	字体起始x坐标
 *          u8 y          	字体起始y坐标
 * 			u16 font_color	字体颜色
 * 			u16 font_bg		字体背景颜色
 * 			u8 size			根据行数选择字体大小：16:8x16	24:12x24	32:16x32 
 * 函数返回值：void
 * 函数说明:
 *******************************/
void lcd_show_eng(u8 *font,u8 x,u8 y,u8 size,u16 font_color,u16 font_bg)
{
	u8 i,j,k;
	u8 font_size;//计算一行有多少字节
	font_size = size / 2 / 8;
	//对12x24字体进行处理
	//字模会补0凑成完整的字节
	if(size /2 % 8 != 0)
	{
		font_size+=1;
	}
	//遍历行数
	for(i=0;i<size;i++)
	{
		//遍历字节数
		for(j=0;j<font_size;j++)
		{
			//遍历像素点
			for(k=0;k<8;k++)
			{
				//判断如果当前像素点为1
				//font[i*font_size+j]:先根据第几个字节，再根据第几个像素点
				if(font[i*font_size+j] & (0x80 >> k))
				{
					//发送字体颜色
					//x+j*8+k:根据字节数然后偏移k个像素点
					lcd_draw_point(x+j*8+k,y+i,font_color);
				}
				else
				{
					lcd_draw_point(x+j*8+k,y+i,font_bg);
				}
			}
		}
	}
}


/*******************************
 * 函数名:lcd_show_chinese
 * 函数功能:lcd显示中文函数
 * 函数参数：u8 *font        字体指针，指向字体数据的起始地址
 *          u8 x          	字体起始x坐标
 *          u8 y          	字体起始y坐标
 * 			u16 font_color	字体颜色
 * 			u16 font_bg		字体背景颜色
 * 			u8 size			根据行数选择字体大小：8:8x8 16:16*16	32:16x32 
 * 函数返回值：void
 * 函数说明:
 *******************************/
void lcd_show_chinese(u8 *font,u8 x,u8 y,u8 size,u16 font_color,u16 font_bg)
{
	u8 i,j,k;
	u8 font_size;//计算一行有多少字节
	font_size = size / 8;//计算字符的字节数
	if(size % 8 != 0)
	{
		font_size+=1;//字模数据自动补0凑成完整的字节
	}
	//遍历行数
	for(i=0;i<size;i++)
	{
		//根据字节数
		for(j=0;j<font_size;j++)
		{
			//遍历每一个像素点
			for(k=0;k<8;k++)
			{
				//判断如果当前像素点为1
				//font[i*font_size+j]:先根据第几个字节，再根据第几个像素点
				if(font[i*font_size+j] & (0x80 >> k))
				{
					//发送字体颜色
					//x+j*8+k:根据字节数然后偏移k个像素点
					lcd_draw_point(x+j*8+k,y+i,font_color);
				}
				else
				{
					lcd_draw_point(x+j*8+k,y+i,font_bg);
				}
			}
		}
	}
}






/*************************************************字库显示**************************************************************************/



/*******************************
 * 函数名:lcd_show_zk_font
 * 函数功能:从字库中显示文字
 * 函数参数：u8 *font        字体指针，指向字体数据的起始地址
 *          u8 x          	字体起始x坐标
 *          u8 y          	字体起始y坐标
 * 			u16 font_color	字体颜色
 * 			u16 font_bg		字体背景颜色
 * 			u8 size			根据行数选择字体大小：8:8x8 16:16*16	32:16x32 
 * 函数返回值：void
 * 函数说明:
 *                                           大小              偏移量：对0x00000000进行偏移           
 *	C:\Users\安圣SAMA\Desktop\ZK\HZK16.bin   0x0003FE46        0x00000000             
 *	C:\Users\安圣SAMA\Desktop\ZK\HZK24.bin   0x0008FC16        0x0003FE46             
 *	C:\Users\安圣SAMA\Desktop\ZK\HZK32.bin   0x000FF906        0x000CFA5C             
 *	C:\Users\安圣SAMA\Desktop\ZK\ASC16.bin   0x00000806        0x001CF362             
 *	C:\Users\安圣SAMA\Desktop\ZK\ASC24.bin   0x00001806        0x001CFB68             
 *	C:\Users\安圣SAMA\Desktop\ZK\ASC32.bin   0x00002006        0x001D136E             
 *
********************************/
void lcd_show_zk_font(u8 *font,u8 x,u8 y,u8 size,u16 font_color,u16 font_bg)
{
	//存放字体数据的缓冲区
	u8 font_buff[128];

	/*计算字体数据的偏移量*/
	u32 addr = 0;
	//计算这个字的字模字节大小
	u8 font_size = size / 8;
	//计算一个字的字模数据大小
	u8 font_data = size * font_size;
	//根据区码和位码求字体数据的偏移量
	addr = ((font[0] - 0xA1) * 94 + (font[1] - 0xA1)) * font_data;
	u32 font_addr = 0;
	//从字库中提取字体数据：根据字体大小选择不同的字库
	switch(size)
	{
		case 16:font_addr = 0x00000000;break;
		case 24:font_addr = 0x0003FE46;break;
		case 32:font_addr = 0x000CFA5C;break;
	}
	//读取字体数据
	w25q64_read_data(font_addr + addr,font_data,font_buff);
	//显示字体
	lcd_show_chinese(font_buff,x,y,size,font_color,font_bg);
}


/*******************************
 * 函数名:lcd_show_zk_char
 * 函数功能:从字库中显示字符
 * 函数参数：u8 font        字体指针，指向字体数据的起始地址
 *          u8 x          	字体起始x坐标
 *          u8 y          	字体起始y坐标
 * 			u16 font_color	字体颜色
 * 			u16 font_bg		字体背景颜色
 * 			u8 size			根据行数选择字体大小：8:8x8 16:16*16	32:16x32 
 * 函数返回值：void
 * 函数说明:
 *                                           大小              偏移量：对0x00000000进行偏移           
 *	C:\Users\安圣SAMA\Desktop\ZK\HZK16.bin   0x0003FE46        0x00000000             
 *	C:\Users\安圣SAMA\Desktop\ZK\HZK24.bin   0x0008FC16        0x0003FE46             
 *	C:\Users\安圣SAMA\Desktop\ZK\HZK32.bin   0x000FF906        0x000CFA5C             
 *	C:\Users\安圣SAMA\Desktop\ZK\ASC16.bin   0x00000806        0x001CF362             
 *	C:\Users\安圣SAMA\Desktop\ZK\ASC24.bin   0x00001806        0x001CFB68             
 *	C:\Users\安圣SAMA\Desktop\ZK\ASC32.bin   0x00002006        0x001D136E             
 *
********************************/
void lcd_show_zk_char(u8 font,u8 x,u8 y,u8 size,u16 font_color,u16 font_bg)
{
	//存放字体数据的缓冲区
	u8 char_buff[64];

	/*计算字体数据的偏移量*/
	u32 addr = 0;
	//计算这个字的字模字节大小
	u8 char_size = size / 2 / 8 ;
	if(size / 2 % 8 != 0)
	{
		char_size++;
	}
	//计算一个字的字模数据大小
	u8 char_data = size * char_size;
	//根据区码和位码求字体数据的偏移量
	addr =  font * char_data;
	//根据字体大小选择不同的字库
	u32 font_addr = 0;
	switch(size)
	{
		case 16:font_addr = 0x001CF362;break;
		case 24:font_addr = 0x001CFB68;break;
		case 32:font_addr = 0x001D136E;break;
	}
	//读取数据
	w25q64_read_data(font_addr+addr,char_data,char_buff);
	//显示字体
	lcd_show_eng(char_buff,x,y,size,font_color,font_bg);
}





/*******************************
 * 函数名:lcd_show_zk_str
 * 函数功能:从字库中显示字符串
 * 函数参数：u8 *str         字符串指针，指向字符串的起始地址
 *          u8 x          	字体起始x坐标
 *          u8 y          	字体起始y坐标
 * 			u16 font_color	字体颜色
 * 			u16 font_bg		字体背景颜色
 * 			u8 size			根据行数选择字体大小：8:8x8 16:16*16	32:16x32 
 * 函数返回值：void
 * 函数说明:      
********************************/
void lcd_show_zk_str(u8 *str,u8 x,u8 y,u8 size,u16 font_color,u16 font_bg)
{
	u16 i = 0;

	while(str[i] != '\0')
	{
		//如果是英文/字符字符
		if(str[i] < 0xA0)
		{
			lcd_show_zk_char(str[i],x,y,size,font_color,font_bg);
			i++;
			x += size / 2;
			if(x > 240 - size / 2)
			{
				//切换到下一行
				x = 0;
				y += size;
			}
		}
		//如果是中文字符
		else
		{

			lcd_show_zk_font(&str[i],x,y,size,font_color,font_bg);
			i += 2;
			x += size;
			if(x > 240 - size)
			{
				//切换到下一行
				x = 0;
				y += size;
			}
		}
	}
}
