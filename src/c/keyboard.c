#include <keyboard.h>
#include <disk.h>
#include <stdio.h>
#include <mouse.h>
#include <utils.h>

int shift_pressed = 0;

void handle_hotkeys(int code) {
	if (code == 0x4B) {
		current_mode = 0;
		is_window_crt = 0;
		show_crt_window = 0;
		ncount = 1;
	}
	if (code == 0x4D) {
		current_mode = 1;
		ncount = 1;
	}

	if (code == 0x3C && current_mode == 1) {
		show_crt_window = 1;
		is_window_crt = 0;
		ncount = 1;
	}

	if (code == 0x1C && current_mode == 1 && is_button_files == 1) {
		current_mode = 2;
		ncount = 1;
	}
	if (code == 0x50 && current_mode == 1 && is_button_files == 1) {
		is_button_apps = 1;
		is_button_files = 0;
		ncount = 1;
	}
	if (code == 0x1C && current_mode == 1 && is_button_apps == 1) {
		current_mode = 3;
		ncount = 1;
	}
	if (code == 0x48 && current_mode == 1 && is_button_apps == 1) {
		is_button_apps = 0;
		is_button_files = 1;
		current_mode = 1;

		ncount = 1;
	}
}

void input_wait_string(char *buffer) {
	int i = 0;
	while (1) {
		update_system();

		int code = get_scancode();

		if (ncount == 1) {
			buffer[i] = '\0';
			return;
		}

		if (code == 0) continue;

		handle_hotkeys(code);

		if (code == 0x1C) {
			buffer[i] = '\0';
			return;
		}

		char letter = 0;

		switch (code) {
		case 0x18: letter = 'o'; break;
		case 0x23: letter = 'h'; break;
		case 0x2E: letter = 'c'; break;
		case 0x1E: letter = 'a'; break;
		case 0x26: letter = 'l'; break;
		case 0x12: letter = 'e'; break;
		case 0x13: letter = 'r'; break;
		case 0x2D: letter = 'x'; break;
		case 0x17: letter = 'i'; break;
		case 0x14: letter = 't'; break;
		case 0x19: letter = 'p'; break;
		case 0x10: letter = 'q'; break;
		case 0x11: letter = 'w'; break;
		case 0x15: letter = 'y'; break;
		case 0x16: letter = 'u'; break;
		case 0x1F: letter = 's'; break;
		case 0x20: letter = 'd'; break;
		case 0x21: letter = 'f'; break;
		case 0x22: letter = 'g'; break;
		case 0x24: letter = 'j'; break;
		case 0x25: letter = 'k'; break;
		case 0x2C: letter = 'z'; break;
		case 0x2F: letter = 'v'; break;
		case 0x30: letter = 'b'; break;
		case 0x31: letter = 'n'; break;
		case 0x32: letter = 'm'; break;
		case 0x39: letter = ' '; break;
		case 0x02: letter = '1'; break;
		case 0x03: letter = '2'; break;
		case 0x04: letter = '3'; break;
		case 0x05: letter = '4'; break;
		case 0x06: letter = '5'; break;
		case 0x07: letter = '6'; break;
		case 0x08: letter = '7'; break;
		case 0x09: letter = '8'; break;
		case 0x0A: letter = '9'; break;
		case 0x0B: letter = '0'; break;
		case 0x0C: letter = '-'; break;
		case 0x0D: letter = '='; break;
		case 0x34: letter = '.'; break;
		case 0x35: letter = '/'; break;
		case 0x0E:
			if (is_window_crt == 0) {
				if (i > 0) {
					i = i - 1;
					if (x > 16) {
						x = x - 8;
						draw_rect(x, y, 8, 8, 0);
					}
				}
			}
			else if (is_window_crt == 1) {
				if (i > 0) {
					i = i - 1;
					if (x > 48) {
						x = x - 8;
						draw_rect(x, y, 8, 8, 0);
					}
				}
			}
			break;

		default:
			letter = 0;
			break;
		}

		if (letter != 0 && i < 255) {
			if (is_window_crt == 0) {
				put_char(letter, 1);
				buffer[i] = letter;
				i++;
			}
			else if (is_window_crt == 1) {
				put_char(letter, 1);
				buffer[i] = letter;
				i++;
			}
		}
	}
}