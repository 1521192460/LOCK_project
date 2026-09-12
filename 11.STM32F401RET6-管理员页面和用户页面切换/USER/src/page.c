#include "page.h"

u8 user_page(void)
{
    static u8 page_flag = 0;
    u8 auth_result;

    //用户页面,只显示一次
    if(page_flag == 0)
    {
        lcd_clear(0, 0, 240, 240, 0xffff);
        lcd_show_zk_str("请输入密码：                ",0, 0, 32, 0x0000, 0xffff);
        page_flag = 1;
    }

    //一直循环部分,等待按键选择
    auth_result = password_open_door();

    if(auth_result == AUTH_ADMIN_OK)
    {
        page_flag = 0;
    }

    return auth_result;
}

u8 manage_page(void)
{
    static u8 page_flag = 0;
    u8 key_value;

    //管理员页面,只显示一次
    if(page_flag == 0)
    {
        lcd_clear(0, 0, 240, 240, 0xffff);

        lcd_show_zk_str("管理员页面                  ",32, 0, 32, 0x0000, 0xffff);
        lcd_show_zk_str("【1】密码管理                ",0, 32, 32, 0x0000, 0xffff);
        lcd_show_zk_str("【2】指纹管理                ",0, 64, 32, 0x0000, 0xffff);
        lcd_show_zk_str("【3】射频卡管理              ",0, 96, 32, 0x0000, 0xffff);
        lcd_show_zk_str("【*】返回                    ",0, 128, 32, 0x0000, 0xffff);
        page_flag = 1;
    }


    //一直循环部分,等待按键选择
    key_value = CY8CMBR3116_key_scan();

    switch(key_value)
    {
        case '1':printf("密码管理\r\n");break;
        case '2':printf("指纹管理\r\n");break;
        case '3':printf("射频卡管理\r\n");break;
        case '*':page_flag = 0;return USER_PAGE;
    }
    return ADMIN_PAGE;
}