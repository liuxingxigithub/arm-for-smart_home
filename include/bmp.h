#ifndef __BMP_H__
#define __BMP_H__


int bmp_open_file(char* path);      // 读取bmp文件，成功返回文件描述符，失败返回-1
int bmp_get_bytes(int bmpfd, int offset, int byteNum, unsigned char** pp);   // 查询文件几位的内容
int bmp_count_value(char* p, int num);      // 将一组byte数据按逆序计算值
int bmp_get_magic_number(int bmpfd, unsigned char** pp);    // 获取文件魔数
int bmp_get_num_model(int bmpfd, int offset, int byteNum);  // 获取数值型的模板
int bmp_get_size(int bmpfd);        // 获取文件大小
int bmp_get_width(int bmpfd);       // 获取文件宽度
int bmp_get_height(int bmpfd);      // 获取图片高度
int bmp_get_pix_bit(int bmpfd);     // 获取像素所占位数
int bmp_get_pix_offset(int bmpfd);  // 获取像素启示地址
int bmp_get_pic_message(int bmpfd, unsigned char** pp, int* width, int* height, int* pix_bit);    // 获取图片基本信息（像素r,g,b颜色数据及宽和高等）
int bmp_colse_file(int bmpfd);      // 关闭文件读取

#endif
