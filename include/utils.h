#ifndef UTILS_H
#define UTILS_H

/**
 * ������� ��� �����.
 * @return 1, ���� ����� ���������; 0, ���� ��.
 */
int compare_strings(char *str1, char *str2);

/**
 * ����� ����� �� src � dest.
 * ���� ����-��������� � ����.
 */
void copy_string(char *dest, char *src);

/**
 * ���������� ����� ���� � ���� ����� (int).
 */
int atoi(char *str);

/**
 * ����� ���������� ����������� � �������'�� VGA (0xA0000).
 * @param x, y ���������� ����� ��������� ����
 * @param width, height ������
 * @param color ������ ������� (0-255)
 */
void draw_rect(int x, int y, int width, int height, unsigned char color);
void draw_button(int _x, int _y, int _width, int _height, char *_msg, unsigned char color, unsigned char text_color);

#endif