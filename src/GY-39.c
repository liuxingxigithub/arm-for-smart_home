#include "GY-39.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

static int fd; // 串口句柄

// 初始化串口并打开设备
int gy39_init(char* portname, int baudrate)
{
    fd = open(portname, O_RDWR);
	if (fd == -1)
	{
		perror("open device error:");
		return -1;
	}

	struct termios myserial;
	//清空结构体
	memset(&myserial, 0, sizeof (myserial));
	//O_RDWR               
	myserial.c_cflag |= (CLOCAL | CREAD);
	//设置控制模式状态，本地连接，接受使能
	//设置 数据位
	myserial.c_cflag &= ~CSIZE;   //清空数据位
	myserial.c_cflag &= ~CRTSCTS; //无硬件流控制
	myserial.c_cflag |= CS8;      //数据位:8

	myserial.c_cflag &= ~CSTOPB;//   //1位停止位
	myserial.c_cflag &= ~PARENB;  //不要校验
	//myserial.c_iflag |= IGNPAR;   //不要校验
	//myserial.c_oflag = 0;  //输入模式
	//myserial.c_lflag = 0;  //不激活终端模式

	switch (baudrate)
	{
		case 9600:
			cfsetospeed(&myserial, B9600);  //设置波特率
			cfsetispeed(&myserial, B9600);
			break;
		case 115200:
			cfsetospeed(&myserial, B115200);  //设置波特率
			cfsetispeed(&myserial, B115200);
			break;
		case 19200:
			cfsetospeed(&myserial, B19200);  //设置波特率
			cfsetispeed(&myserial, B19200);
			break;
	}
	/* 刷新输出队列,清楚正接受的数据 */
	tcflush(fd, TCIFLUSH);

	/* 改变配置 */
	tcsetattr(fd, TCSANOW, &myserial);
    return 0;
}

// 关闭串口
void gy39_close()
{
    close(fd);
}

// 获取气象数据
int gy39_get_weather(float* temperature, float* humidity, float* pressure, float* altitude)
{
    unsigned char cmd[] = {0xA5, 0x82, 0x27};
    unsigned char buf[15];

    // 发送命令
    write(fd, cmd, sizeof(cmd));

    ssize_t rsize;

    // 读取数据
    while(1)
    {
        rsize = read(fd, buf, 15);

        if (rsize == 15)
        {   
            // 解析数据
            *temperature = ((buf[4] << 8) | buf[5]) / 100.0;
            *pressure = ((buf[6] << 24) | (buf[7] << 16) | (buf[8] << 8) | buf[9]) / 100.0;
            *humidity = ((buf[10] << 8) | buf[11]) / 100.0;
            *altitude = (buf[10] << 8) | buf[11];
            return 0;
        }
    }
}

// 获取光照强度数据
int gy39_get_light(float* illuminance)
{
    unsigned char cmd[] = {0xA5, 0x81, 0x26};
    unsigned char buf[25]={0};

    // 发送命令
    ssize_t rsize = 0;
    write(fd, cmd, 3);

    // 读取数据
    while(1)
    {
        rsize = read(fd, buf, 9);

        if (rsize == 9)
        {   
            // 解析数据
            *illuminance =((buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7]);
            break;
        }
    }
    return 0;
}