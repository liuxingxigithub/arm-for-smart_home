/*************************************************
*头文件
*************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/input.h>
#include"bmp.h"
#include"lcd.h"

int open_album(int num, char* names[num])
{
    int lable = 0;  // 当前图片标识数

    /*图片的信息*/
    unsigned char* datas[num];
    int widths[num];
    int heights[num];
    int pix_bits[num];

    /*获取图片的信息*/
    for (int i = 0; i < num; i++)
    {
        int bmpfd = bmp_open_file(names[i]);
        bmp_get_pic_message(bmpfd, &datas[i], &widths[i], &heights[i], &pix_bits[i]);
        bmp_colse_file(bmpfd);
    }
    
    /*绘制第一张*/
    showPic_write(datas[lable], heights[lable], widths[lable], pix_bits[lable]);
    // 绘制箭头
    showPic_append(datas[num - 1], heights[num - 1], widths[num - 1], pix_bits[num - 1]);
	
    int event_fd = 0;
	ssize_t rsize = 0;
	struct input_event ev;//用来保存从输入设备中获取到的的输入事件
	event_fd =open("/dev/input/event0",O_RDWR);
	if (-1 == event_fd)
	{
		perror("open event0 error");
	}
	
    int xi = 0, x_down = 0;
    int yi = 0, y_down = 0;
	while(1)
	{
		rsize = read(event_fd, &ev, sizeof(ev));
		if (rsize == sizeof(ev))    //读到了输入事件
		{
            if (ev.type == 0X03)
            {
                // 1022*600
                if (ev.code == 0X00)
                {
                    //printf("x: %d   %d\n", ev.value, ev.value*8/12);
                    xi = ev.value;              
                }
                else if (ev.code == 0X01)
                {   
                    //printf("y: %d   %d\n", ev.value, ev.value*8/12);
                    yi = ev.value;
                }
            }
            if (ev.code == 0x14a)
            {
                if (ev.value == 1)
                {
                    x_down = xi;
                    y_down = yi;
                }
                else if (ev.value == 0)
                {
                    if (xi < 80 && yi < 60)
                    {
                        break;
                    }
                    if ((xi < (x_down - 100))||(xi > (x_down + 100)))
                    {
                        if (xi < (x_down - 100))
                        {
                            lable--;
                        }
                        else
                        {
                            lable ++;
                        }
                    
                        if (lable < 0)
                            lable = num - 2;
                        if (lable == num - 1)
                            lable = 0;

                        showPic_write(datas[lable], heights[lable], widths[lable], pix_bits[lable]);
                        // 绘制箭头
                        showPic_append(datas[num - 1], heights[num - 1], widths[num - 1], pix_bits[num - 1]);
                    }
                }
                
            }
        }
	}

    /*释放datas空间*/
    for (int i = 0; i < num; i++)
    {
        free(datas[i]);
    }
    return 0;
}

void photo_start()      // 启动电子相册
{
    char* names[] = {"../res/2.bmp", "../res/luoli.bmp", "../res/xiaogong.bmp", "../res/return.bmp"};
	open_album(4, names);
}