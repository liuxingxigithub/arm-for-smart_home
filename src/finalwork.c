#include"lcd.h"
#include"bmp.h"
#include"GY-39.h"
#include"pwm.h"

#include"photo.h"
#include"warner.h"
#include"mine.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>

int main()
{
    lcd_init();
    // 初始化串口并打开设备
    gy39_init("/dev/ttySAC1", 9600);
    // 初始化蜂鸣器
    pwm_init();

    // 获取背景数据
    unsigned char* data;
    int width;
    int height;
    int pix_bit;
    int bmpfd = bmp_open_file("../res/back.bmp");
    bmp_get_pic_message(bmpfd, &data, &width, &height, &pix_bit);
    bmp_colse_file(bmpfd);
    while (1)
    {
        showPic_write(data, height, width, pix_bit);
        int event_fd = 0;
        ssize_t rsize = 0;
        struct input_event ev;  //用来保存从输入设备中获取到的的输入事件
        event_fd =open("/dev/input/event0",O_RDWR);
        if (-1 == event_fd)
        {
            perror("open event0 error");
        }
        
        int x = 0;
        int y = 0;
        int choose = 0;
        while(1)
        {
            rsize = read(event_fd, &ev, sizeof(ev));
            if (rsize == sizeof(ev))//读到了输入事件
            {
                if (ev.type == 0X03)
                {
                    if (ev.code == 0X00)
                    {
                        x = ev.value;        
                    }
                    else if (ev.code == 0X01)
                    {   
                        y = ev.value;
                    }
                    
                    //printf("F03 x:%d y:%d\n", x, y);
                }
                if (ev.code == 0x14a)
                {
                    //printf("F14 x:%d y:%d\n", x, y);
                    // x:130-190  y:170-260
                    // x:340-410  y:250-360
                    if (ev.value == 0)
                    {
                        if (x > 160 && x < 210 && y > 200 && y < 300)
                        {
                            choose = 1;
                            break;
                        }
                        if (x > 450 && x < 520 && y > 330 && y < 400)
                        {
                            choose = 2;
                            break;
                        }
                        if (x > 800 && x < 850 && y > 230 && y < 280)
                        {
                            choose = 3;
                            break;
                        }
                    }
                }
            }
        }
        if (choose == 1)
            photo_start();      // 打开电子相册
        else if (choose == 2)
            warner_start();
        else if (choose == 3)
            mine_start();
        
    }
    free(data);

    lcd_uninit();
    // 关闭蜂鸣器
    pwm_uninit();
    // 关闭串口
    gy39_close();
    return 0;
}

