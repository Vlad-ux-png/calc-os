#ifndef VIDEO_H
#define VIDEO_H
#include <stdint.h>

#define fb_address (*(volatile uint32_t*)0x0500)
#define VIDEO_MEMORY ((uint32_t*)(uint32_t)fb_address)

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

#define RGB32(r, g, b) ((uint32_t)((((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF)))

#define COLOR_BLACK          RGB32(0, 0, 0)
#define COLOR_BLUE           RGB32(0, 0, 170)
#define COLOR_GREEN          RGB32(0, 170, 0)
#define COLOR_CYAN           RGB32(0, 170, 170)
#define COLOR_RED            RGB32(170, 0, 0)
#define COLOR_MAGENTA        RGB32(170, 0, 170)
#define COLOR_BROWN          RGB32(170, 85, 0)
#define COLOR_LIGHT_GRAY     RGB32(170, 170, 170)
#define COLOR_DARK_GRAY      RGB32(85, 85, 85)
#define COLOR_LIGHT_BLUE     RGB32(85, 85, 255)
#define COLOR_LIGHT_GREEN    RGB32(85, 255, 85)
#define COLOR_LIGHT_CYAN     RGB32(85, 255, 255)
#define COLOR_LIGHT_RED      RGB32(255, 85, 85)
#define COLOR_LIGHT_MAGENTA  RGB32(255, 85, 255)
#define COLOR_YELLOW         RGB32(255, 255, 85)
#define COLOR_WHITE          RGB32(255, 255, 255)

#define COLOR_SYS_BG         RGB32(212, 208, 200)  
#define COLOR_SYS_TITLE      RGB32(10, 24, 80)  
#define COLOR_SYS_SHADOW     RGB32(128, 128, 128)  
#define COLOR_SYS_LIGHT      RGB32(230, 230, 230)  
#define COLOR_ACCENT_BLUE    RGB32(0, 120, 215)  
#define COLOR_TEXT_DARK      RGB32(26, 26, 26) 
#define COLOR_ORANGE         RGB32(255, 165, 0)  

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
extern int is_button_calc;
extern int draw_0;
extern int draw_1;
extern int is_scaled;
extern char content[512];
extern int current_uid;
extern int win_file_x;
extern int win_file_y;
extern int is_crushed;

extern char name[128];
extern char content[512];
extern const char* root_password;

void draw_desktop();

void screen_clear();
void put_char(char s, uint32_t color);
void printk(const char *msg, uint32_t color);
void print(const char *msg, uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_rounded_rect(int x, int y, int width, int height, int r, uint32_t color);
void draw_crt_window(int wx, int wy);

void update_system();
void handle_hotkeys(int code);
void init_palette();

extern void outb( uint16_t port, uint8_t val);
extern volatile uint8_t inb(uint16_t port);
extern void outw(uint16_t port, uint16_t val);
extern volatile uint16_t inw(uint16_t port);
extern void outl(uint16_t port, uint32_t val);
extern volatile uint32_t inl(uint16_t port);
void graphics();

#endif
