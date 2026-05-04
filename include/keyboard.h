#ifndef KEYBOARD_H
#define KEYBOARD_H

void input_wait_string(char *buffer);

void handle_hotkeys(int code);
extern int get_scancode();

#endif