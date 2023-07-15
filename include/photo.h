#ifndef __PHOTO_H__
#define __PHOTO_H__

void showPic_append(char* data, int height,int width, int pix_bit);     // 以不覆盖的方式绘制图片
void showPic_write(char* data, int height,int width, int pix_bit);       // 以覆盖的方式绘制图片
int open_album(int num, char* names[num]);
void photo_start();      // 启动电子相册

#endif