#include "lcd.h"
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int lcdfd;
int *plcd;


// int lcd_getlength(int x0, int y0, int x1, int y1)  // 求屏幕两点间距离
// {
//     return (int)sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
// }

int lcd_init()  // 初始化，打开LCD屏驱动设备文件
{
	/*打开LCD屏驱动设备文件*/
	lcdfd = open("/dev/fb0",O_RDWR);//打开lcd屏幕
	if (lcdfd == -1)
	{
		perror("open lcd error");
		return -1;
	}

	/*da将LCD屏映射到内存空间 =》Frame Buffer 帧缓冲设备*/
	plcd = (int*)mmap(NULL,//系统自动选择映射地址
						800*480*32/8,//映射空间大小（单位 字节），屏幕800*480个像素点，每个像素点32位（32/8）字节
						PROT_READ|PROT_WRITE, //具备读写权限
						MAP_SHARED,//共享映射，对映射内存空间的操作会被同步到文件
                  		lcdfd,//指定映射的文件
                  		0);//从文件头开始映射
	if (MAP_FAILED == plcd)
	{
		perror("mmap lcd error");
		close(lcdfd);
		return -1;
	}
}

int lcd_uninit()   // 关闭LCD屏驱动设备文件
{
    /*解除映射，关闭屏幕文件*/
	munmap(plcd,800*480*32/6);
	close(lcdfd);
}

int lcd_draw_point(int x, int y, int color)    //画点函数
{
    if (x<0||x>=800||y<0||y>=480)
    {
        printf("画点超出范围\n");
        return -1;
    }
    *(plcd+y*800+x) = color;
}

int lcd_draw_clear(int x0, int y0, int x1, int y1, int color)  //矩形填色函数
{
    int xmax, xmin;
    int ymax, ymin;
    if (x0 > x1)
    {
        xmax = x0;
        xmin = x1;
    }
    else
    {
        xmax = x1;
        xmin = x0;
    }
    
    if (y0 > y1)
    {
        ymax = y0;
        ymin = y1;
    }
    else
    {
        ymax = y1;
        ymin = y0;
    }
    
    /*通过操作映射内存地址，来实现对于文件内容的访问：plcd+y*800+x就是（x,y)像素点的坐标*/
	for (int y = ymin; y < ymax; y++)
	{
		for (int x = xmin; x < xmax; x++)
		{
			lcd_draw_point(x, y, color);
		}
	}
}

// int lcd_draw_rainrow()     // 画彩虹函数
// {
//     int sky_blue = 0X2CA9E1;
//     int purple = 0X800080;
//     int blue = 0X0000FF;
//     int cyan = 0X00FFFF;
//     int green = 0X008000;
//     int yellow = 0XFFFF00;
//     int orange = 0XFFA500;
//     int red = 0XFF0000;

//     /*通过操作映射内存地址，来实现对于文件内容的访问：plcd+y*800+x就是（x,y)像素点的坐标*/
// 	for (int y = 0; y < 480; y++)
// 	{
// 		for (int x = 0; x < 800; x++)
// 		{
//             int length = lcd_getlength(x, y, 400, 480);
// 			if (length < 100)
//                 lcd_draw_point(x, y, sky_blue);
// 			else if (length < 130)
//                 lcd_draw_point(x, y, purple);
//             else if (length < 160)
//                 lcd_draw_point(x, y, blue);
//             else if (length < 190)
//                 lcd_draw_point(x, y, cyan);
//             else if (length < 220)
//                 lcd_draw_point(x, y, green);
//             else if (length < 250)
//                 lcd_draw_point(x, y, yellow);
//             else if (length < 280)
//                 lcd_draw_point(x, y, orange);
//             else if (length < 310)
//                 lcd_draw_point(x, y, red);
//             else
//                 lcd_draw_point(x, y, sky_blue);
// 		}
// 	}

// }

// int lcd_draw_fourleaves()  // 画四叶草函数
// {
//     int x = 400;    // 中心点坐标
//     int y = 240;
//     int r = 150;    // 叶子长度

//     /*四个旋转点的坐标*/
//     int x0 = x, y0 = y - r;
//     int x1 = x + r, y1 = y;
//     int x2 = x, y2 = y + r;
//     int x3 = x - r, y3 = y;

//     /*通过操作映射内存地址，来实现对于文件内容的访问：plcd+y*800+x就是（x,y)像素点的坐标*/
// 	for (int y = 0; y < 480; y++)
// 	{
// 		for (int x = 0; x < 800; x++)
// 		{
//            if ((lcd_getlength(x, y, x0, y0)<=r)&&(lcd_getlength(x, y, x1, y1)<=r)||
//            (lcd_getlength(x, y, x2, y2)<=r)&&(lcd_getlength(x, y, x1, y1)<=r)||
//            (lcd_getlength(x, y, x2, y2)<=r)&&(lcd_getlength(x, y, x3, y3)<=r)||
//            (lcd_getlength(x, y, x0, y0)<=r)&&(lcd_getlength(x, y, x3, y3)<=r))
//            {
//                 lcd_draw_point(x, y, 0X008000);
//            }
//            else
//                 lcd_draw_point(x, y, 0XFF0000);
           
// 		}
// 	}

// }

int lcd_draw_word(int x, int y, int color, int wordNum, int wordWidth, int wordHeight, const unsigned char fonts[wordNum][wordWidth*wordHeight/8]) // 绘制文字
{
    int numPerRow = wordWidth / 8;
    int dataNum = numPerRow * wordHeight;
    // 按照字节行遍历字模数据，在屏幕上绘制对应位置的像素点
    for (int i = 0; i < wordNum; i++){
        for (int dataNo = 0; dataNo < dataNum; ++dataNo) {
            unsigned char data = fonts[i][dataNo];
            for (int bitnum = 0; bitnum < 8; ++bitnum) {
                if (data & 0x80) {
                    lcd_draw_point(x + wordWidth * i + (dataNo % numPerRow)*8 + bitnum, y + dataNo / numPerRow, color);
                }
                data <<= 1;
            }
        }
    }
}


int lcd_draw_word_reverse(int x, int y, int color, int wordNum, int wordWidth, int wordHeight, const unsigned char fonts[wordNum][wordWidth*wordHeight/8]) // 逆序绘制文字
{
    int numPerRow = wordWidth / 8;
    int dataNum = numPerRow * wordHeight;
    // 按照字节行遍历字模数据，在屏幕上绘制对应位置的像素点
    for (int i = 0; i < wordNum; i++){
        for (int dataNo = 0; dataNo < dataNum; ++dataNo) {
            unsigned char data = fonts[i][dataNo];
            for (int bitnum = 0; bitnum < 8; ++bitnum) {
                if (data & 0x80) {
                    lcd_draw_point(x + wordWidth * (wordNum - 1 - i) + (dataNo % numPerRow)*8 + bitnum, y + dataNo / numPerRow, color);
                }
                data <<= 1;
            }
        }
    }
}


int lcd_draw_one_word(int x, int y, int color, int wordWidth, int wordHeight, unsigned char* fonts) // 绘制一个文字
{
    int numPerRow = wordWidth / 8;
    int dataNum = numPerRow * wordHeight;
    // 按照字节行遍历字模数据，在屏幕上绘制对应位置的像素点
    for (int dataNo = 0; dataNo < dataNum; ++dataNo) {
        unsigned char data = fonts[dataNo];
        for (int bitnum = 0; bitnum < 8; ++bitnum) {
            if (data & 0x80) {
                lcd_draw_point(x + (dataNo % numPerRow)*8 + bitnum, y + dataNo / numPerRow, color);
            }
            data <<= 1;
        }
    }

}

void showPic_append(char* data, int height,int width, int pix_bit)     // 以不覆盖的方式绘制图片
{
    int r,g,b,a,color;
    int i = 0;

    for (int y = height-1; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			b = data[i++];
			g = data[i++];
			r = data[i++];
			a =((pix_bit == 32)?data[i++]:0);
			color = (a&0xff)<<24|(r&0xff)<<16|(g&0xff)<<8|b&0xff;
			lcd_draw_point(x, y, color);
		}
	}
}

void showPic_write(char* data, int height,int width, int pix_bit)       // 以覆盖的方式绘制图片
{
    lcd_draw_clear(0, 0, 800, 480, 0XFFFFFF);  //矩形填色函数
    showPic_append(data, height, width, pix_bit);
}
