#ifndef __LCD_H__
#define __LCD_H__



extern int lcdfd;
extern int *plcd;

// int lcd_getlength(int x0, int y0, int x1, int y1);  // 求屏幕两点间距离
int lcd_init(); // 初始化，打开LCD屏驱动设备文件
int lcd_uninit();   // 关闭LCD屏驱动设备文件
int lcd_draw_point(int x, int y, int color);    //画点函数
int lcd_draw_clear(int x0, int y0, int x1, int y1, int color);  //矩形填色函数
// int lcd_draw_rainrow();     // 画彩虹函数
// int lcd_draw_fourleaves();  // 画四叶草函数
int lcd_draw_word(int x, int y, int color, int wordNum, int wordWidth, int wordHeight, const unsigned char fonts[wordNum][wordWidth*wordHeight/8]); // 绘制文字
int lcd_draw_word_reverse(int x, int y, int color, int wordNum, int wordWidth, int wordHeight, const unsigned char fonts[wordNum][wordWidth*wordHeight/8]); // 逆序绘制文字
int lcd_draw_one_word(int x, int y, int color, int wordWidth, int wordHeight, unsigned char* fonts); // 绘制一个文字
void showPic_append(char* data, int height,int width, int pix_bit);     // 以不覆盖的方式绘制图片
void showPic_write(char* data, int height,int width, int pix_bit);       // 以覆盖的方式绘制图片

#endif
