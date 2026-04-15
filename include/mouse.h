#ifndef MOUSE_H
#define MOUSE_H

/* --- Зовнішні функції (з ASM або інших модулів) --- */
extern void mouse_wait();
extern void mouse_wait_input();
extern void init_mouse();
extern int get_scancode();

/* --- Глобальні змінні стану миші --- */
/* extern дозволяє використовувати ці змінні в будь-якому .c файлі */
extern int mouse_x;
extern int mouse_y;
extern int old_mouse_x;
extern int old_mouse_y;
extern int mouse_left_button;
extern unsigned char mouse_cursor[8][8];
extern unsigned char cursor_back_buffer[64];
extern int show_file_content;
extern char open_file_name[32];
extern char open_file_text[476];

/* --- Прототипи функцій миші --- */
void mouse_handler();
void draw_cursor(int mx, int my);
void save_background();
void restore_background();
void check_ui_clicks();

#endif