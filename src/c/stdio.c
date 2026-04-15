#include <disk.h>
#include <stdio.h>
#include <mouse.h>
#include <utils.h>
#include <keyboard.h>

#define VIDEO_MEMORY ((unsigned char*)0xA0000)
#define MAX_FILES 64
#define MAX_SECTORS 10

void draw_rect(int x, int y, int width, int height, unsigned char color);
void prompt();

int x = 0;
int y = 0;
int ncount = 0;
int current_mode = 0;
int show_crt_window = 0;
int is_window_crt = 0;
int is_button_apps = 0;
int is_button_files = 1;

void screen_clear() {
	for (int i = 0; i < 64000; i = i + 1) {
		VIDEO_MEMORY[i] = 0;
	}

	x = 0;
	y = 0;
	ncount = 1;
}

void put_char(char s, unsigned char color) {
	unsigned char *font = (unsigned char*)0xFFA6E;

	if (s == '\n') {
		x = 0;
		y = y + 8;
		return;
	}

	if (x >= 320) {
		x = 0;
		y = y + 8;
	}

	if (y >= 200) {
		screen_clear();
	}

	for (int i = 0; i < 8; i = i + 1) {
		unsigned char bits = font[s * 8 + i];
		for (int j = 0; j < 8; j = j + 1) {
			if (bits > 127) {
				VIDEO_MEMORY[(y + i) * 320 + (x + j)] = color;
			}
			else {
			}
			bits = bits * 2;
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