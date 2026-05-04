#include <disk.h>
#include <stdio.h>
#include <mouse.h>
#include <utils.h>
#include <keyboard.h>
#include <font.h>

#define VIDEO_MEMORY ((unsigned char*)0xA0000)
#define MAX_FILES 64
#define MAX_SECTORS 10

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
	for (int i = 0; i < 64000; i = i + 1) {
		VIDEO_MEMORY[i] = 0;
	}

	x = 0;
	y = 0;
	ncount = 1;
}

void put_char(char s, unsigned char color) {
    if (s == '\n') {
        x = 0;
        y = y + 8;
        return;
    }

    if (x >= 640) {
        x = 0;
        y = y + 8;
    }

    if (y >= 480) {
        screen_clear();
        y = 0;
    }

    for (int i = 0; i < 8; i = i + 1) {
        unsigned char bits = font[(int)s][i];
        unsigned char *row = &VIDEO_MEMORY[(y + i) * 80];

        for (int j = 0; j < 8; j = j + 1) {
            int pos = x + j;
            unsigned char mask = 128 >> (pos % 8);

            if (bits > 127) {
                if (color > 0) {
                    row[pos / 8] = row[pos / 8] | mask;
                } else {
                    row[pos / 8] = row[pos / 8] & ~mask;
                }
            }
            bits = bits << 1;
        }
    }
    x = x + 8;
}

void print(char *msg, unsigned char color) {
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