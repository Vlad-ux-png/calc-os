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

    draw_rect(0, 0, 640, 400, 1);
    draw_rect(0, 0, 640, 36, 0);

    x = 272;
    y = 10;
    print("CalcOS", 1);

    if (current_mode == 0) {
        draw_rect(0, 80, 640, 320, 0);
        draw_rect(20, 48, 136, 26, 0);
        draw_rect(190, 48, 136, 26, 2);

        x = 24;
        y = 52;
        print("Terminal", 1);

        x = 194;
        y = 52;
        print("Explorer", 0);

        x = 0;
        y = 96;
    }
    else if (current_mode == 2) {
        draw_rect(0, 80, 640, 320, 0);
        draw_rect(20, 48, 136, 26, 2);
        draw_rect(190, 48, 136, 26, 0);
        draw_rect(0, 360, 640, 36, 1);

        x = 24;
        y = 52;
        print("Terminal", 0);

        x = 194;
        y = 52;
        print("Explorer", 1);

        x = 144;
        y = 370;
        print("F2 - Create a new file", 0);

        int file_count = 0;
        struct File f;

        for (int i = 0; i < MAX_FILES; i++) {
            ata_read_sector(MAX_SECTORS + i, (unsigned short *)&f);

            if (f.exists == 1) {
                int col = file_count % 3;
                int row = file_count / 3;

                int icon_x = (16 + col * 100) * 2;
                int icon_y = (50 + row * 34) * 2;

                draw_rect(icon_x, icon_y, 160, 48, 1);

                x = icon_x + 16;
                y = icon_y + 16;

                print(f.name, 0);

                file_count++;
            }
        }

        if (show_crt_window == 1) {
            draw_rect(120, 80, 400, 240, 1);
            draw_rect(124, 84, 392, 232, 0);

            x = 200;
            y = 100;
            print("Create a new file", 1);

            x = 140;
            y = 160;
            print("Name:", 1);

            x = 140;
            y = 220;
            print("Text:", 1);
        }
    }
    else if (current_mode == 3) {
        draw_rect(0, 80, 640, 320, 0);
        draw_rect(20, 48, 136, 26, 2);
        draw_rect(190, 48, 136, 26, 0);

        x = 24;
        y = 52;
        print("Terminal", 0);

        x = 194;
        y = 52;
        print("Explorer", 1);

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

                draw_rect(icon_x, icon_y, 160, 48, 1);

                x = icon_x + 16;
                y = icon_y + 16;
                print(a.name, 0);

                file_count++;
            }
        }
    }
    else {
        draw_rect(0, 80, 640, 320, 1);
        draw_rect(20, 48, 136, 26, 2);
        draw_rect(190, 48, 136, 26, 0);

        if (is_button_files == 1) {
            draw_rect(156, 100, 320, 36, 0);
            draw_rect(156, 200, 320, 36, 2);

            x = 276;
            y = 110;
            print("Files", 1);

            x = 276;
            y = 210;
            print("Apps", 0);
        }
        else if (is_button_apps == 1) {
            draw_rect(156, 100, 320, 36, 2);
            draw_rect(156, 200, 320, 36, 0);

            x = 276;
            y = 110;
            print("Files", 0);

            x = 276;
            y = 210;
            print("Apps", 1);
        }

        x = 24;
        y = 52;
        print("Terminal", 0);

        x = 194;
        y = 52;
        print("Explorer", 1);
    }

    while (1) {
        if (ncount == 1) goto refresh;

        update_system();

        if (ncount == 1) goto refresh;

        if (current_mode == 0) {
            print("> ", 1);
            input_wait_string(command);

            if (ncount == 1) goto refresh;

            print("\n", 1);

            if (compare_strings(command, "help")) {
                print("Available commands:\n", 1);
                print("  help - show this message\n", 1);
                print("  cln  - clear the screen\n", 1);
                print("  dir  - list all files\n", 1);
                print("  crt  - create a new text file\n", 1);
                print("  draw - draw a rectangle\n", 1);
                print("  run  - run program from disk\n", 1);
                print("  apps - available apps\n", 1);
                print("  cat - view the contents of the file\n", 1);
                print("  fmt  - format disk\n", 1);
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
                        print(f.name, 1);
                        print("\n", 1);
                    }
                }
            }
            else if (compare_strings(command, "cat")) {
                char name[8];

                print("Enter file name: ", 1);
                input_wait_string(name);
                print("\n", 1);

                struct File f;
                for (int i = 0; i < MAX_FILES; i++) {
                    ata_read_sector(MAX_SECTORS + i, (unsigned short *)&f);
                    if (compare_strings(name, f.name) && f.exists == 1) {
                        print(f.content, 1);
                        print("\n", 1);
                    }
                }
            }
            else if (compare_strings(command, "crt")) {
                struct File f;
                print("Name: ", 1);
                input_wait_string(filename);
                print("\nContent: ", 1);
                input_wait_string(filecontent);
                print("\n", 1);

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

                print("Enter width: ", 1);
                input_wait_string(val);
                r_w = atoi(val);

                print("\nEnter height: ", 1);
                input_wait_string(val);
                r_h = atoi(val);

                print("\nEnter x: ", 1);
                input_wait_string(val);
                r_x = atoi(val);

                print("\nEnter y: ", 1);
                input_wait_string(val);
                r_y = atoi(val);

                print("\n", 1);
                draw_rect(r_x*2, r_y*2, r_w*2, r_h*2, 1);
            }
            else if (compare_strings(command, "fmt")) {
                format_disk();
                print("\nDone!\n", 1);
                ncount = 1;
            }
            else if (compare_strings(command, "run")) {
                char appname[32];
                print("App name: ", 1);
                input_wait_string(appname);
                run_app(appname);
            }
            else if (compare_strings(command, "apps")) {
                struct App a;
                int apps_found = 0;

                print("\n", 1);

                for (int i = 0; i < MAX_APPS; i++) {
                    ata_read_sector(150 + i, (unsigned short *)&a);
                    if (a.exists == 1) {
                        print(a.name, 1);
                        print("\n", 1);
                        apps_found++;
                    }
                }

                if (apps_found == 0) {
                    print("No apps installed.\n", 1);
                }
            }
            else {
                if (command[0] != '\0') {
                    print("Uncnown command. Type 'help'\n", 1);
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

                x = 260;
                y = 160;
                input_wait_string(filename);

                if (ncount == 1) goto refresh;

                x = 300;
                y = 220;
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