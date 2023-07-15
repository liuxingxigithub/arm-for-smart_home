#ifndef __MINE_H__
#define __MINE_H__

#define mine_width 8
#define mine_height 8
#define per_len 50


extern char* path_arrow;
extern int mine_num;
extern int clicked_num;
extern int margin_UD;
extern int margin_LR;
extern int m[mine_height + 2][mine_width + 2];

extern int c_bg;
extern int c_number[8];
extern int c_unopen;
extern int c_open;
extern int c_win_bg;
extern int c_win_font;
extern int c_failed_bg;
extern int c_failed_font;

extern int num_width;
extern int num_height;
extern int padding_UD;
extern int padding_LR;
extern unsigned char mine_number[8][16*21/8];
extern unsigned char mine_win_fonts[4][40*35/8];
extern unsigned char mine_failed_fonts[4][40*35/8];


void mine_draw_gezi(int mx, int my, int color);
void mine_draw_font(int mx, int my, int number);
void mine_init();
int mine_count_around(int mx, int my);
void mine_click(int mx, int my);
void player_click(int x, int y);
void mine_win();
void mine_failed();
void mine_start();

#endif