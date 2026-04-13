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

void draw_rect(int x, int y, int width, int height, unsigned char color) {
	unsigned char *vga = (unsigned char *)0xA0000;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int curr_x = x + j;
			int curr_y = y + i;

			if (curr_x >= 0 && curr_x < 320 && curr_y >= 0 && curr_y < 200) {
				vga[curr_y * 320 + curr_x] = color;
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