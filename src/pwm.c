#include"pwm.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int fd;
char beep_ctrl[1];

void pwm_init()     // 初始化蜂鸣器
{
    fd = open("/dev/pwm",O_RDWR);
	if(fd < 0)
	{
		printf("open /dev/pwm failed\n");
	}
}

void pwm_start()    // 开启蜂鸣器
{
    int ret;
    beep_ctrl[0] = 1; 	//响
    ret = write(fd,beep_ctrl,sizeof(beep_ctrl));
    if( ret == -1)
    {
        perror("write");
    } 
}

void pwm_stop()     // 停止蜂鸣器
{
    int ret;
    beep_ctrl[0] = 0;  //不响
    ret = write(fd,beep_ctrl,sizeof(beep_ctrl));
    if( ret == -1)
    {
        perror("write");
    }
}

void pwm_uninit()   // 注销蜂鸣器
{
	close(fd);
}