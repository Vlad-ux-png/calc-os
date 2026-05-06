#include <disk.h>
#include <stdio.h>
#include <mouse.h>
#include <utils.h>
#include <keyboard.h>
#include <font.h>

int x = 0;
int y = 0;
int ncount = 0;
int current_mode = 0;
int show_crt_window = 0;
int is_window_crt = 0;
int is_button_apps = 0;
int is_button_files = 1;

void set_palette_color(unsigned char index, unsigned char r, unsigned char g, unsigned char b) {
    outb(0x03C8, index);
    outb(0x03C9, r);
    outb(0x03C9, g);
    outb(0x03C9, b);
}

void init_palette() {
    set_palette_color(0, 8, 10, 12);
    set_palette_color(1, 45, 50, 58);
}

void screen_clear() {
    unsigned int *fb = VIDEO_MEMORY_PTR;
    int total_pixels = 1024 * 768;

    for (int i = 0; i < total_pixels; i++) {
        fb[i] = 0x00000000; 
    }

    x = 0;
    y = 0;
}

void put_char(char s, unsigned int color) {
    if (s == '\n') {
        x = 0;
        y += 8; 
        return;
    }

    if (x >= 1024) {
        x = 0;
        y += 8;
    }

    if (y >= 768) {
        screen_clear();
    }

    unsigned short pitch = *(unsigned short*)0x8010; 
    unsigned int *lfb_ptr = (unsigned int*)0x8028;  
    unsigned char *fb = (unsigned char *)(*lfb_ptr);
    
    int bytes_per_pixel = 3; 

    for (int i = 0; i < 8; i++) {
        unsigned char bits = font[(int)s][i];
        
        for (int j = 0; j < 8; j++) {
            if (bits > 127) {
                int offset = ((y + i) * pitch) + ((x + j) * bytes_per_pixel);
                
                fb[offset]     = (color & 0xFF);     
                fb[offset + 1] = (color >> 8) & 0xFF; 
                fb[offset + 2] = (color >> 16) & 0xFF;   
            }
            bits = bits << 1;
        }
    }

    x += 8;
}

void print(char *msg, unsigned int color) {
	for (int i = 0; msg[i] != 0; i++) {
		put_char(msg[i], color);
	}
}

void update_system() {
	restore_background();
	mouse_handler();
	save_background();
	draw_cursor(mouse_x, mouse_y);
	old_mouse_x = mouse_x;
	old_mouse_y = mouse_y;
	check_ui_clicks();
}