#ifndef MOUSE_H
#define MOUSE_H

extern void mouse_wait();
extern void mouse_wait_input();
extern void init_mouse();
extern int get_scancode();

extern int mouse_x;
extern int mouse_y;
extern int old_mouse_x;
extern int old_mouse_y;
extern int mouse_left_button;
extern unsigned char mouse_cursor[16][16];
extern unsigned char cursor_back_buffer[256];

void mouse_handler();
void draw_cursor(int mx, int my);
void save_background();
void restore_background();
void check_ui_clicks();

#endif