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

void draw_gray_rect(int x, int y, int width, int height) {
    for (int i = 0; i < height; i = i + 1) {
        for (int j = 0; j < width; j = j + 1) {
            int curr_x = x + j;
            int curr_y = y + i;

            if (curr_x >= 0 && curr_x < 640 && curr_y >= 0 && curr_y < 480) {
                int is_white = (curr_x + curr_y) % 2 == 0;
                
                unsigned char *row = &VIDEO_MEMORY[curr_y * 80];
                unsigned char mask = 128 >> (curr_x % 8);

                if (is_white) {
                    row[curr_x / 8] = row[curr_x / 8] | mask;
                } else {
                    row[curr_x / 8] = row[curr_x / 8] & ~mask;
                }
            }
        }
    }
}

void draw_rect(int x, int y, int width, int height, unsigned char color) {
    if (color == 2) {
        draw_gray_rect(x, y, width, height);
        return;
    }

    for (int i = 0; i < height; i = i + 1) {
        for (int j = 0; j < width; j = j + 1) {
            int curr_x = x + j;
            int curr_y = y + i;

            if (curr_x >= 0 && curr_x < 640 && curr_y >= 0 && curr_y < 480) {
                unsigned char *ptr = &VIDEO_MEMORY[curr_y * 80 + (curr_x / 8)];
                unsigned char mask = 128 >> (curr_x % 8);

                if (color > 0) {
                    *ptr = *ptr | mask;   
                } else {
                    *ptr = *ptr & ~mask;   
                }
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

void draw_button(int _x, int _y, int _width, int _height, char *_msg, unsigned char color, unsigned char text_color) {
    draw_rect(_x, _y, _width, _height, color);
    x = _x + 4;
    y = _y + 4;

    print(_msg, text_color);
}