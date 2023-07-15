#include"bmp.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdlib.h>
#include <stdio.h>

int bmp_open_file(char* path)      // 读取bmp文件，成功返回文件描述符，失败返回-1
{
	int bmpfd = open(path ,O_RDONLY);
	if (-1 == bmpfd)
	{
		perror("open bmp error");
		return -1;
	}
	return bmpfd;
}

int bmp_get_bytes(int bmpfd, int offset, int byteNum, unsigned char** pp)   // 查询文件几位的内容
{
	lseek(bmpfd, offset, SEEK_SET);
	ssize_t rsize = read(bmpfd, *pp, byteNum);
	if (-1 == rsize)
	{
		perror("read bmp error");
		close(bmpfd);
		return -1;
	}
	return 0;
}

int bmp_count_value(char* p, int num)      // 将一组byte数据按逆序计算值
{
	int file_size = -1;
	if (num == 4)
		file_size = p[0]&0xff |(p[1]&0xff)<<8 |(p[2]&0xff)<<16 |(p[3]&0xff)<<24;
	else if (num == 2)
		file_size = p[0]&0xff |(p[1]&0xff)<<8;
	return file_size;
}

int bmp_get_magic_number(int bmpfd, unsigned char** pp)    // 获取文件魔数
{	
	*pp = (char*)malloc(2); 
	if ((*pp) == NULL)
	{
		printf("bmp_get_magic_number函数中内存分配失败\n");
		return -1;
	}
	
	if(bmp_get_bytes(bmpfd, 0X00, 2, pp) == -1)
	{
		free(*pp);
		printf("bmp_get_magic_number函数中bmp_get_bytes获取失败\n");
		return -1;
	}

	return 0;	
}

int bmp_get_num_model(int bmpfd, int offset, int byteNum)	// 获取数值型的模板
{		
	unsigned char* p = (char*)malloc(byteNum); 
	if (p == NULL)
	{
		printf("bmp_get_num_model函数中内存分配失败\n");
		return -1;
	}
	
	if(bmp_get_bytes(bmpfd, offset, byteNum, &p) == -1)
	{
		free(p);
		printf("bmp_get_num_model函数中bmp_get_bytes获取失败\n");
		return -1;
	}
		
	int result = bmp_count_value(p, byteNum);
	if (result == -1)
	{
		free(p);
		printf("bmp_get_num_model函数中字节数非2或4, 不符合bmp_count_value函数\n");
		return -1;
	}

	free(p);
	return result;
}

int bmp_get_size(int bmpfd)        // 获取文件大小
{	
	return bmp_get_num_model(bmpfd, 0X02, 4);
}

int bmp_get_width(int bmpfd)       // 获取文件宽度
{
	return bmp_get_num_model(bmpfd, 0x12, 4);
}

int bmp_get_height(int bmpfd)      // 获取图片高度
{
	return bmp_get_num_model(bmpfd, 0x16, 4);
}

int bmp_get_pix_bit(int bmpfd)     // 获取像素所占位数
{
	return bmp_get_num_model(bmpfd, 0x1C, 2);
}

int bmp_get_pix_offset(int bmpfd)  // 获取像素启示地址
{
	return bmp_get_num_model(bmpfd, 0x0A, 4);
}

int bmp_get_pic_message(int bmpfd, unsigned char** pp, int* width, int* height, int* pix_bit)    // 获取图片像素颜色数据及宽和高
{
    *height = bmp_get_height(bmpfd);
    *width = bmp_get_width(bmpfd);  
    *pix_bit = bmp_get_pix_bit(bmpfd);
    int pix_offset = bmp_get_pix_offset(bmpfd);

	int byteNum = (*width)*(*height)*(*pix_bit)/8;
	*pp = (char*)malloc(byteNum); 
	if ((*pp) == NULL)
	{
		printf("bmp_get_pic_message函数中内存分配失败\n");
		return -1;
	}
	return bmp_get_bytes(bmpfd, pix_offset, byteNum, pp);
}

int bmp_colse_file(int bmpfd)      // 关闭文件读取
{
	return close(bmpfd);
}
