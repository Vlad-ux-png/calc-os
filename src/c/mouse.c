#include <stdio.h>
#include <disk.h>
#include <mouse.h>
#include <utils.h>
#include <keyboard.h>

unsigned char mouse_cursor[16][16] = {
    {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0},
    {2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0},
    {2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0},
    {2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0},
    {2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


void draw_cursor(int mx, int my) {
    unsigned short pitch = *(unsigned short*)0x8010;
    unsigned int *lfb_ptr = (unsigned int*)0x8028;
    unsigned char *fb = (unsigned char *)(*lfb_ptr);
    
    int bytes_per_pixel = 3; 

    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 16; col++) {
            unsigned char c = mouse_cursor[row][col];
            
            if (c == 1 || c == 2) {
                int curr_x = mx + col;
                int curr_y = my + row;

                if (curr_x >= 0 && curr_x < 1024 && curr_y >= 0 && curr_y < 768) {
                    int offset = (curr_y * pitch) + (curr_x * bytes_per_pixel);

                    if (c == 1) {
                        fb[offset]     = 0xFF; 
                        fb[offset + 1] = 0xFF; 
                        fb[offset + 2] = 0xFF;
                    } else if (c == 2) {
                        fb[offset]     = 0x00; 
                        fb[offset + 1] = 0x00; 
                        fb[offset + 2] = 0x00; 
                    }
                }
            }
        }
    }
}

int mouse_x = 320;
int mouse_y = 200;

int old_mouse_x = 320;
int old_mouse_y = 200;
int mouse_left_button = 0;
unsigned long cursor_back_buffer[16 * 16];

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

    if (mouse_x > 624) mouse_x = 624; 
    if (mouse_y > 464) mouse_y = 464; 

	mouse_left_button = first_byte & 0x01;
}

void save_background() {
    unsigned short pitch = *(unsigned short*)0x8010;
    unsigned int lfb_addr = *(unsigned int*)0x8028;
    unsigned char *fb = (unsigned char *)lfb_addr;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            int cur_x = mouse_x + j;
            int cur_y = mouse_y + i;

            if (cur_x >= 0 && cur_x < 640 && cur_y >= 0 && cur_y < 480) {
                unsigned int offset = (cur_y * pitch) + (cur_x * 3);
                
                unsigned long b = fb[offset];
                unsigned long g = fb[offset + 1];
                unsigned long r = fb[offset + 2];
                
                cursor_back_buffer[i * 16 + j] = b | (g << 8) | (r << 16);
            }
        }
    }
}

void restore_background() {
    unsigned short pitch = *(unsigned short*)0x8010;
    unsigned int lfb_addr = *(unsigned int*)0x8028;
    unsigned char *fb = (unsigned char *)lfb_addr;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            int old_x = old_mouse_x + j;
            int old_y = old_mouse_y + i;

            if (old_x >= 0 && old_x < 640 && old_y >= 0 && old_y < 480) {
                unsigned int offset = (old_y * pitch) + (old_x * 3);
                unsigned long color = cursor_back_buffer[i * 16 + j];

                fb[offset]     = (unsigned char)(color & 0xFF);      
                fb[offset + 1] = (unsigned char)((color >> 8) & 0xFF); 
                fb[offset + 2] = (unsigned char)((color >> 16) & 0xFF); 
            }
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
		if (mouse_x >= 190 && mouse_x <= 326 && mouse_y >= 48 && mouse_y <= 74) {
			if (current_mode != 1) {
				current_mode = 1;
				ncount = 1;
			}
		}
		
		if (mouse_x >= 20 && mouse_x <= 156 && mouse_y >= 48 && mouse_y <= 74) {
			if (current_mode != 0) {
				current_mode = 0;
				ncount = 1;
			}
		}

		if (current_mode == 1 && mouse_x >= 156 && mouse_x <= 316 && mouse_y >= 100 && mouse_y <= 118) {
            current_mode = 2;
			ncount = 1;
        }

		if (current_mode == 1 && mouse_x >= 156 && mouse_x <= 316 && mouse_y >= 200 && mouse_y <= 218) {
			current_mode = 3;
			ncount = 1;
		}

		if (current_mode == 2 && mouse_y >= 440 && mouse_y <= 480) {
			show_crt_window = 1;
			ncount = 1;
		}
	}
}