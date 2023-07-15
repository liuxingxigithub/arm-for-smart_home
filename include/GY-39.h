#ifndef _GY_39_H_
#define _GY_39_H_

int gy39_init(char* portname, int baudrate); // 初始化串口并打开设备
void gy39_close(); // 关闭串口
int gy39_get_weather(float* temperature, float* humidity, float* pressure, float* altitude); // 获取气象数据
int gy39_get_light(float* illuminance); // 获取光照强度数据

#endif