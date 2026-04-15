#include <stdio.h>
#include <disk.h>
#include <mouse.h>
#include <utils.h>
#include <keyboard.h>

unsigned char mouse_cursor[8][8] = {
	{2, 0, 0, 0, 0, 0, 0, 0},
	{2, 2, 0, 0, 0, 0, 0, 0},
	{2, 1, 2, 0, 0, 0, 0, 0},
	{2, 1, 1, 2, 0, 0, 0, 0},
	{2, 1, 1, 1, 2, 0, 0, 0},
	{2, 1, 2, 2, 2, 2, 0, 0},
	{2, 2, 0, 0, 0, 0, 0, 0},
	{2, 0, 0, 0, 0, 0, 0, 0}
};

void draw_cursor(int mx, int my) {
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			unsigned char color = mouse_cursor[row][col];
			if (color == 1) {
				VIDEO_MEMORY[(my + row) * 320 + (mx + col)] = 0x0F;
			}
			else if (color == 2) {
				VIDEO_MEMORY[(my + row) * 320 + (mx + col)] = 0x00;
			}
		}
	}
}

int mouse_x = 160;
int mouse_y = 100;

int old_mouse_x = 160;
int old_mouse_y = 100;
int mouse_left_button = 0;
unsigned char cursor_back_buffer[64];

void mouse_handler() {
	if ((inb(0x64) & 1) == 0) {
		return;
	}

	unsigned char mouse_packet[3];
	unsigned char status = inb(0x64);

	if (!(status & 0x01) || !(status & 0x20)) {
		return;
	}

	unsigned char first_byte = inb(0x60);

	if (!(first_byte & 0x08)) {
		return;
	}

	mouse_packet[0] = first_byte;

	mouse_wait_input();
	mouse_packet[1] = inb(0x60);

	mouse_wait_input();
	mouse_packet[2] = inb(0x60);

	int x_offset = (int)((signed char)mouse_packet[1]);
	int y_offset = (int)((signed char)mouse_packet[2]);

	mouse_x += x_offset;
	mouse_y -= y_offset;

	if (mouse_x < 0) mouse_x = 0;
	if (mouse_y < 0) mouse_y = 0;
	if (mouse_x > 312) mouse_x = 312;
	if (mouse_y > 192) mouse_y = 192;

	mouse_left_button = first_byte & 0x01;
}

void save_background() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cursor_back_buffer[i * 8 + j] = VIDEO_MEMORY[(mouse_y + i) * 320 + (mouse_x + j)];
		}
	}
}

void restore_background() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			VIDEO_MEMORY[(old_mouse_y + i) * 320 + (old_mouse_x + j)] = cursor_back_buffer[i * 8 + j];
		}
	}
}

int check_keyboard() {
	int code = get_scancode();
	if (code == 0) return 0;
	return code;
}


void check_ui_clicks() {
	if (mouse_left_button == 1) {
		if (mouse_x >= 95 && mouse_x <= 163 && mouse_y >= 24 && mouse_y <= 37) {
			if (current_mode != 1) {
				current_mode = 1;
				ncount = 1;
			}
		}
		if (mouse_x >= 10 && mouse_x <= 78 && mouse_y >= 24 && mouse_y <= 37) {
			if (current_mode != 0) {
				current_mode = 0;
				ncount = 1;
			}
		}
		if (current_mode == 1 && mouse_x >= 78 && mouse_x <= 238 && mouse_y >= 50 && mouse_y <= 68) {
            current_mode = 2;
			ncount = 1;
        }
		if (current_mode == 1 && mouse_y >= 180 && mouse_y <= 200) {
			show_crt_window = 1;
			ncount = 1;
		}
	}
}