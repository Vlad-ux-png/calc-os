#ifndef UTILS_H
#define UTILS_H

/**
 * Порівнює два рядки.
 * @return 1, якщо рядки ідентичні; 0, якщо ні.
 */
int compare_strings(char *str1, char *str2);

/**
 * Копіює рядок із src у dest.
 * Додає нуль-термінатор в кінці.
 */
void copy_string(char *dest, char *src);

/**
 * Перетворює рядок цифр у ціле число (int).
 */
int atoi(char *str);

/**
 * Малює заповнений прямокутник у відеопам'яті VGA (0xA0000).
 * @param x, y Координати лівого верхнього кута
 * @param width, height Розміри
 * @param color Індекс кольору (0-255)
 */
void draw_rect(int x, int y, int width, int height, unsigned char color);

#endif