#include <disk.h>
#include <stdio.h>
#include <mouse.h>
#include <utils.h>
#include <keyboard.h>

int compare_strings(char *str1, char *str2) {
	int i = 0;
	while (str1[i] == str2[i]) {
		if (str1[i] == '\0') {
			return 1;
		}
		i++;
	}
	return 0;
}

void copy_string(char *dest, char *src) {
	int i = 0;
	while (src[i] != '\0') {
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

void draw_rect(int x, int y, int width, int height, unsigned long color) {
    unsigned short pitch = *(unsigned short*)0x8010;
    unsigned int lfb_addr = *(unsigned int*)0x8028;
    unsigned char *fb = (unsigned char *)lfb_addr;

    unsigned char r = (unsigned char)((color & 0xFF0000UL) >> 16);
    unsigned char g = (unsigned char)((color & 0x00FF00UL) >> 8);
    unsigned char b = (unsigned char)(color & 0x0000FFUL);

    for (int i = 0; i < height; i++) {
        unsigned int row_offset = (y + i) * pitch;
        for (int j = 0; j < width; j++) {
            int cur_x = x + j;
            int cur_y = y + i;

            if (cur_x >= 0 && cur_x < 640 && cur_y >= 0 && cur_y < 480) {
                unsigned int offset = row_offset + (cur_x * 3);
                
                fb[offset]     = b; 
                fb[offset + 1] = g; 
                fb[offset + 2] = r;
            }
        }
    }
}

int atoi(char *str) {
	int res = 0;
	for (int i = 0; str[i] != '\0'; ++i) {
		if (str[i] >= '0' && str[i] <= '9') {
			res = res * 10 + str[i] - '0';
		}
	}
	return res;
}

void draw_button(int _x, int _y, int _width, int _height, char *_msg, unsigned long color, unsigned long text_color) {
    draw_rect(_x, _y, _width, _height, color);
    x = _x + 4;
    y = _y + 4;

    print(_msg, text_color);
}