#ifndef STDIO_H
#define STDIO_H

#define VIDEO_MEMORY ((unsigned char*)0xA0000)
#define MAX_FILES 64
#define MAX_SECTORS 10

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


void screen_clear();
void put_char(char s, unsigned char color);
void print(char *msg, unsigned char color);

void update_system();
void handle_hotkeys(int code);

void set_palette_color(unsigned char index, unsigned char r, unsigned char g, unsigned char b);
void init_palette();

extern int get_scancode();
extern void outb(unsigned short port, unsigned char val);
extern unsigned char inb(unsigned short port);
extern void outw(unsigned short port, unsigned short val);
extern unsigned short inw(unsigned short port);

#endif