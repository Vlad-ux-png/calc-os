#include <disk.h>
#include <stdio.h>
#include <utils.h>
#include <mouse.h>
#include <keyboard.h>

char command[256];
char filename[8];
char filecontent[256];

void prompt() {
refresh:
    ncount = 0;

    draw_rect(0, 0, 640, 400, 0x00BFFFUL);
    draw_rect(0, 0, 640, 40, 0x000080UL);

    x = 276;
    y = 10;
    print("CalcOS", 0xFFFFFF);

    if (current_mode == 0) {
        draw_rect(0, 80, 640, 480, 0x000000UL);

        draw_button(20, 48, 136, 26, "Terminal", 0xFF0000UL, 0x000000);
        draw_button(190, 48, 136, 26, "Explorer", 0x0000FFUL, 0x000000);
        x = 0;
        y = 96;
    }
    else if (current_mode == 2) {
        draw_rect(0, 80, 640, 480, 0x00BFFFUL);

        draw_button(20, 48, 136, 26, "Terminal", 0x0000FFUL, 0x000000);
        draw_button(190, 48, 136, 26, "Explorer", 0xFF0000UL, 0x000000);

        draw_button(0, 440, 640, 40, "F2 - Create a new file", 0x000080UL, 0xFFFFFF);

        int file_count = 0;
        struct File f;

        for (int i = 0; i < MAX_FILES; i++) {
            ata_read_sector(MAX_SECTORS + i, (unsigned short *)&f);

            if (f.exists == 1) {
                int col = file_count % 3;
                int row = file_count / 3;

                int icon_x = (16 + col * 100) * 2;
                int icon_y = (50 + row * 34) * 2;

                draw_rect(icon_x, icon_y, 160, 48, 0xFFFFFFUL);

                x = icon_x + 16;
                y = icon_y + 16;

                print(f.name, 0x000000);

                file_count++;
            }
        }

        if (show_crt_window == 1) {
            draw_rect(104, 100, 432, 260, 0x808080UL);
            draw_rect(100, 96, 432, 260, 0xC0C0C0UL);
            draw_rect(104, 100, 424, 252, 0xFFFFFFUL);

            draw_rect(104, 100, 424, 32, 0x1E4B8BUL);

            x = 118;
            y = 104;
            print("Create a new file", 0xFFFFFF);

            x = 118;
            y = 140;
            print("Name:", 0x000000);
            draw_rect(118, 152, 404, 24, 0xA0A0A0UL);
            draw_rect(120, 154, 400, 20, 0xF0F0F0UL);

            x = 118;
            y = 190;
            print("Text:", 0x000000);
            draw_rect(118, 202, 404, 80, 0xA0A0A0UL);
            draw_rect(120, 204, 400, 76, 0xF8F8F8UL);

            x = 118;
            y = 290;
            print("Press Enter to save", 0x505050);
        }
    }
    else if (current_mode == 3) {
        draw_rect(0, 80, 640, 480, 0x00BFFFUL);
        
        draw_button(20, 48, 136, 26, "Terminal", 0x0000FFUL, 0x000000);
        draw_button(190, 48, 136, 26, "Explorer", 0xFF0000UL, 0x000000);

        int file_count = 0;
        struct File f;
        struct App a;

        for (int i = 0; i < MAX_APPS; i++) {
            ata_read_sector(APPS_START_SECTOR + i, (unsigned short *)&a);
            if (a.exists == 1) {
                int col = file_count % 3;
                int row = file_count / 3;

                int icon_x = (16 + col * 100) * 2;
                int icon_y = (50 + row * 34) * 2;

                draw_rect(icon_x, icon_y, 160, 48, 0xFFFFFFUL);

                x = icon_x + 16;
                y = icon_y + 16;
                print(a.name, 0x000000);

                file_count++;
            }
        }
    }
    else {
        draw_rect(0, 80, 640, 480, 0x00BFFFUL);

        draw_button(20, 48, 136, 26, "Terminal", 0x0000FFUL, 0x000000);
        draw_button(190, 48, 136, 26, "Explorer", 0xFF0000UL, 0x000000);

        if (is_button_files == 1) {
            draw_button(156, 100, 320, 36, "Files", 0x000000UL, 0xFFFFFF);
            draw_button(156, 200, 320, 36, "Apps", 0x808080UL, 0xFFFFFF);
        }
        else if (is_button_apps == 1) {
            draw_button(156, 100, 320, 36, "Files", 0x808080UL, 0xFFFFFF);
            draw_button(156, 200, 320, 36, "Apps", 0x000000UL, 0xFFFFFF);
        }
    }

    while (1) {
        if (ncount == 1) goto refresh;

        update_system();

        if (ncount == 1) goto refresh;

        if (current_mode == 0) {
            print("> ", 0xFFFFFF);
            input_wait_string(command);

            if (ncount == 1) goto refresh;

            print("\n", 0xFFFFFF);

            if (compare_strings(command, "help")) {
                print("Available commands:\n", 0xFFFFFF);
                print("  help - show this message\n", 0xFFFFFF);
                print("  cln  - clear the screen\n", 0xFFFFFF);
                print("  dir  - list all files\n", 0xFFFFFF);
                print("  crt  - create a new text file\n", 0xFFFFFF);
                print("  draw - draw a rectangle\n", 0xFFFFFF);
                print("  run  - run program from disk\n", 0xFFFFFF);
                print("  apps - available apps\n", 0xFFFFFF);
                print("  cat - view the contents of the file\n", 0xFFFFFF);
                print("  fmt  - format disk\n", 0xFFFFFF);
            }
            else if (compare_strings(command, "cln")) {
                screen_clear();
                ncount = 1;
            }
            else if (compare_strings(command, "dir")) {
                struct File f;
                for (int i = 0; i < MAX_FILES; i++) {
                    ata_read_sector(MAX_SECTORS + i, (unsigned short *)&f);
                    if (f.exists == 1) {
                        print(f.name, 0xFFFFFF);
                        print("\n", 0xFFFFFF);
                    }
                }
            }
            else if (compare_strings(command, "cat")) {
                char name[8];

                print("Enter file name: ", 0xFFFFFF);
                input_wait_string(name);
                print("\n", 0xFFFFFF);

                struct File f;
                for (int i = 0; i < MAX_FILES; i++) {
                    ata_read_sector(MAX_SECTORS + i, (unsigned short *)&f);
                    if (compare_strings(name, f.name) && f.exists == 1) {
                        print(f.content, 0xFFFFFF);
                        print("\n", 0xFFFFFF);
                    }
                }
            }
            else if (compare_strings(command, "crt")) {
                struct File f;
                print("Name: ", 0xFFFFFF);
                input_wait_string(filename);
                print("\nContent: ", 0xFFFFFF);
                input_wait_string(filecontent);
                print("\n", 0xFFFFFF);

                for (int i = 0; i < MAX_FILES; i++) {
                    ata_read_sector(MAX_SECTORS + i, (unsigned short *)&f);
                    if (f.exists == 0) {
                        copy_string(f.name, filename);
                        copy_string(f.content, filecontent);
                        f.exists = 1;
                        ata_write_sector(MAX_SECTORS + i, (unsigned short *)&f);
                        break;
                    }
                }
            }
            else if (compare_strings(command, "draw")) {
                char val[16];
                int r_w, r_h, r_x, r_y;

                print("Enter width: ", 0xFFFFFF);
                input_wait_string(val);
                r_w = atoi(val);

                print("\nEnter height: ", 0xFFFFFF);
                input_wait_string(val);
                r_h = atoi(val);

                print("\nEnter x: ", 0xFFFFFF);
                input_wait_string(val);
                r_x = atoi(val);

                print("\nEnter y: ", 0xFFFFFF);
                input_wait_string(val);
                r_y = atoi(val);

                print("\n", 0xFFFFFF);
                draw_rect(r_x, r_y, r_w, r_h, 0xFFFFFF);
            }
            else if (compare_strings(command, "fmt")) {
                format_disk();
                print("\nDone!\n", 0xFFFFFF);
                ncount = 1;
            }
            else if (compare_strings(command, "run")) {
                char appname[32];
                print("App name: ", 0xFFFFFF);
                input_wait_string(appname);
                run_app(appname);
            }
            else if (compare_strings(command, "apps")) {
                struct App a;
                int apps_found = 0;

                print("\n", 0xFFFFFF);

                for (int i = 0; i < MAX_APPS; i++) {
                    ata_read_sector(150 + i, (unsigned short *)&a);
                    if (a.exists == 1) {
                        print(a.name, 0xFFFFFF);
                        print("\n", 0xFFFFFF);
                        apps_found++;
                    }
                }

                if (apps_found == 0) {
                    print("No apps installed.\n", 0xFFFFFF);
                }
            }
            else {
                if (command[0] != '\0') {
                    print("Uncnown command. Type 'help'\n", 0xFFFFFF);
                }
            }
        }
        else if (current_mode == 2) {
            int code = get_scancode();
            if (code != 0) {
                handle_hotkeys(code);
                if (code == 0x3C) {
                    show_crt_window = 1;
                    ncount = 1;
                }
            }

            if (ncount == 1) goto refresh;

            if (show_crt_window == 1) {
                struct File f;

                is_window_crt = 1;

                for (int i = 0; i < 256; i++) {
                    filename[i] = 0;
                    filecontent[i] = 0;
                }

                x = 126;
                y = 156;
                input_wait_string(filename);

                if (ncount == 1) goto refresh;

                x = 126;
                y = 214;
                input_wait_string(filecontent);

                if (ncount == 1) goto refresh;

                for (int i = 0; i < MAX_FILES; i++) {
                    ata_read_sector(MAX_SECTORS + i, (unsigned short *)&f);
                    if (f.exists == 0) {
                        copy_string(f.name, filename);
                        copy_string(f.content, filecontent);
                        f.exists = 1;
                        ata_write_sector(MAX_SECTORS + i, (unsigned short *)&f);
                        break;
                    }
                }

                show_crt_window = 0;
                is_window_crt = 0;
                ncount = 1;
            }
        }
        else {
            int code = get_scancode();
            if (code != 0) handle_hotkeys(code);
            if (ncount == 1) goto refresh;
        }
    }
}

void __attribute__((section(".text.entry"))) kernel_main() {
    init_palette();
    init_mouse();
	screen_clear();
	prompt();
}