#ifndef __WARNER_H__
#define __WARNER_H__

extern char* bg_path;
extern int data_x1, data_x2;
extern int data_y1, data_y2, data_y3;
extern int number_width, number_height;
extern int is_return;

extern unsigned char number[10][16*21/8];
extern unsigned char warnModel[5][40*35/8];

void showPic_append(char* data, int height,int width, int pix_bit);     // 以不覆盖的方式绘制图片
void showPic_write(char* data, int height,int width, int pix_bit);
void getWordIndex(int num, int* len, int** words_p);        // 获得数字的字模数组
void showData(float temperature, float humidity, float pressure, float altitude, float illuminance);    // 绘制数据
void showWarn();
void* check_return(void *args);
int warner_start();


#endif