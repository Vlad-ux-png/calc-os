#ifndef STDIO_H
#define STDIO_H

/* --- Константи (Macros) --- */
#define VIDEO_MEMORY ((unsigned char*)0xA0000)
#define MAX_FILES 64
#define MAX_SECTORS 10

/* --- Глобальні змінні (Extern) --- */
/* Ці змінні визначені в stdio.c, тут ми даємо доступ до них іншим файлам */
extern int x;
extern int y;
extern int ncount;
extern int current_mode;
extern int show_crt_window;
extern int is_window_crt;
extern int mouse_x;
extern int mouse_y;
extern int mouse_left_button;

/* --- Прототипи функцій (Функції за замовчуванням є extern) --- */

// Керування екраном
void screen_clear();
void put_char(char s, unsigned char color);
void print(char *msg, unsigned char color);
void draw_rect(int x, int y, int width, int height, unsigned char color);

// Введення та обробка системи
void update_system();
void input_wait_string(char *buffer);
int check_keyboard();
void handle_hotkeys(int code);

// Робота з рядками та конвертація
int compare_strings(char *str1, char *str2);
void copy_string(char *dest, char *src);
int atoi(char *str);

// Асемберні функції
extern int get_scancode();
extern void sys_halt();
extern void outb(unsigned short port, unsigned char val);
extern unsigned char inb(unsigned short port);
extern void outw(unsigned short port, unsigned short val);
extern unsigned short inw(unsigned short port);
extern void mouse_wait();
extern void mouse_wait_input();
extern void init_mouse();

#endif