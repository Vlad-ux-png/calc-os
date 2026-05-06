#ifndef STDIO_H
#define STDIO_H

#define VIDEO_MEMORY_PTR (*(unsigned int**)0x8028)

#define VBE_INFO_PTR ((unsigned char*)0x8000)

#define VESA_PITCH   (*(unsigned short*)(VBE_INFO_PTR + 0x10)) 
#define VESA_WIDTH   (*(unsigned short*)(VBE_INFO_PTR + 0x12)) 
#define VESA_BPP     (*(unsigned char*)(VBE_INFO_PTR + 0x19))  

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
void put_char(char s, unsigned int color);
void print(char *msg, unsigned int color);

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