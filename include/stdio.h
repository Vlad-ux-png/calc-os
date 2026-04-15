#ifndef STDIO_H
#define STDIO_H

/* --- ��������� (Macros) --- */
#define VIDEO_MEMORY ((unsigned char*)0xA0000)
#define MAX_FILES 64
#define MAX_SECTORS 10

/* --- ��������� ����� (Extern) --- */
/* ֳ ����� ��������� � stdio.c, ��� �� ���� ������ �� ��� ����� ������ */
extern int x;
extern int y;
extern int ncount;
extern int current_mode;
extern int show_crt_window;
extern int is_window_crt;
extern int mouse_x;
extern int mouse_y;
extern int mouse_left_button;
extern int is_button_apps;
extern int is_button_files;


/* --- ��������� ������� (������� �� ������������� � extern) --- */

// ��������� �������
void screen_clear();
void put_char(char s, unsigned char color);
void print(char *msg, unsigned char color);
void draw_rect(int x, int y, int width, int height, unsigned char color);

// �������� �� ������� �������
void update_system();
void input_wait_string(char *buffer);
int check_keyboard();
void handle_hotkeys(int code);

// ������ � ������� �� �����������
int compare_strings(char *str1, char *str2);
void copy_string(char *dest, char *src);
int atoi(char *str);

// ��������� �������
extern int get_scancode();
extern void outb(unsigned short port, unsigned char val);
extern unsigned char inb(unsigned short port);
extern void outw(unsigned short port, unsigned short val);
extern unsigned short inw(unsigned short port);
extern void mouse_wait();
extern void mouse_wait_input();
extern void init_mouse();

#endif