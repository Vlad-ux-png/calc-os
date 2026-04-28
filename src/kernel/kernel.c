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

	draw_rect(0, 0, 320, 200, 0x0B);
	draw_rect(0, 0, 320, 18, 0x01);

	x = 136;
	y = 5;
	print("CalcOS", 0x0F);

	if (current_mode == 0) {
		draw_rect(0, 40, 320, 160, 0x00);
		draw_rect(10, 24, 68, 13, 0x0F);
		draw_rect(95, 24, 68, 13, 0x28);

		x = 12;
		y = 26;
		print("Terminal", 0x00);

		x = 97;
		y = 26;
		print("Explorer", 0x00);

		x = 0;
		y = 48;
	}
	else if (current_mode == 2) {
		draw_rect(0, 40, 320, 160, 0x0B);
		draw_rect(10, 24, 68, 13, 0x28);
		draw_rect(95, 24, 68, 13, 0x0F);
		draw_rect(0, 180, 320, 18, 0x01);

		x = 12;
		y = 26;
		print("Terminal", 0x00);

		x = 97;
		y = 26;
		print("Explorer", 0x00);

		x = 72;  
		y = 185; 
		print("F2 - Create a new file", 0x0F);

		int file_count = 0;
		struct File f;

		for (int i = 0; i < MAX_FILES; i++) {
			ata_read_sector(MAX_SECTORS + i, (unsigned short *)&f);

			if (f.exists == 1) {
				int col = file_count % 3;
				int row = file_count / 3;

				int icon_x = 16 + (col * 100);

				int icon_y = 50 + (row * 34);

				draw_rect(icon_x, icon_y, 80, 24, 0x0F);

				x = icon_x + 8;
				y = icon_y + 8;

				print(f.name, 0x00);

				file_count++;
			}
		}

		if (show_crt_window == 1) {
			draw_rect(60, 40, 200, 120, 0x07); 
			draw_rect(62, 42, 196, 116, 0x0F); 

			x = 100;
			y = 50;
			print("Create a new file", 0x00);

			x = 70; 
			y = 80;  
			print("Name:", 0);

			x = 70; 
			y = 110;
			print("Text:", 0);
		}
	}
	else if (current_mode == 3) {
		draw_rect(0, 40, 320, 160, 0x0B);
		draw_rect(10, 24, 68, 13, 0x28);
		draw_rect(95, 24, 68, 13, 0x0F);

		x = 12;
		y = 26;
		print("Terminal", 0x00);

		x = 97;
		y = 26;
		print("Explorer", 0x00);

		int file_count = 0;
    		struct File f;
    		struct App a;

    		for (int i = 0; i < MAX_APPS; i++) {
       		 	ata_read_sector(APPS_START_SECTOR + i, (unsigned short *)&a);
       		 	if (a.exists == 1) {
          		  	int col = file_count % 3;
          		  	int row = file_count / 3;
           		 	int icon_x = 16 + (col * 100);
           		 	int icon_y = 50 + (row * 34);

           			 draw_rect(icon_x, icon_y, 80, 24, 0x0A); 
           			 x = icon_x + 8; y = icon_y + 8;
           			 print(a.name, 0x00);
            			 file_count++;
        		}
    		}
	}
	else {
		draw_rect(0, 40, 320, 160, 0x0B);
		draw_rect(10, 24, 68, 13, 0x28);
		draw_rect(95, 24, 68, 13, 0x0F);

		if (is_button_files == 1) {
			draw_rect(78, 50, 160, 18, 0x28);
			draw_rect(78, 100, 160, 18, 0x0F);

			x = 138;
			y = 55;
			print("Files", 0x00);
			x = 138;
			y = 105;
			print("Apps", 0x00);
		}
		else if (is_button_apps == 1) {
			draw_rect(78, 50, 160, 18, 0x0F);
			draw_rect(78, 100, 160, 18, 0x28);
			x = 138;
			y = 55;
			print("Files", 0x00);
			x = 138;
			y = 105;
			print("Apps", 0x00);
		}
		x = 12;
		y = 26;
		print("Terminal", 0x00);

		x = 97;
		y = 26;
		print("Explorer", 0x00);
	}

		while (1) {
			if (ncount == 1) {
				goto refresh;
			}

			update_system();

			if (ncount == 1) {
				goto refresh;
			}

			if (current_mode == 0) {
				print("> ", 0x0F);
				input_wait_string(command);

				if (ncount == 1) {
					goto refresh;
				}

				print("\n", 0x0F);

				if (compare_strings(command, "help")) {
					print("Available commands:\n", 0x0E);
					print("  help - show this message\n", 0x0F);
					print("  cln  - clear the screen\n", 0x0F);
					print("  dir  - list all files\n", 0x0F);
					print("  crt  - create a new text file\n", 0x0F);
					print("  draw - draw a rectangle\n", 0x0F);
					print("  run  - run program from disk\n", 0x0F);
					print("  apps - available apps\n", 0x0F);
					print("  fmt  - format disk\n", 0x0F);
				}
				else if (compare_strings(command, "cln")) {
					screen_clear();
					ncount = 1;
				}
				else if (compare_strings(command, "dir")) {
					struct File f;
					for (int i = 0; i < MAX_FILES; i = i + 1) {
						ata_read_sector(MAX_SECTORS + i, (unsigned short *)&f);
						if (f.exists == 1) {
							print("-", 0x0B);
							print(f.name, 0x0F);
							print("\n", 0x0F);
							print(f.content, 0x0F);
							print("\n", 0x0F);
						}
					}
				}
				else if (compare_strings(command, "crt")) {
					struct File f;
					print("Name: ", 0x0B);
					input_wait_string(filename);
					print("\nContent: ", 0x0B);
					input_wait_string(filecontent);
					print("\n", 0x0F);

					for (int i = 0; i < MAX_FILES; i = i + 1) {
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
					print("Enter width: ", 0x0B);
					input_wait_string(val);
					r_w = atoi(val);
					print("\n", 0x0F);
					print("Enter height: ", 0x0B);
					input_wait_string(val);
					r_h = atoi(val);
					print("\n", 0x0F);
					print("Enter x: ", 0x0B);
					input_wait_string(val);
					r_x = atoi(val);
					print("\n", 0x0F);
					print("Enter y: ", 0x0B);
					input_wait_string(val);
					r_y = atoi(val);
					print("\n", 0x0F);
					draw_rect(r_x, r_y, r_w, r_h, 0x0C);
				}
				else if (compare_strings(command, "fmt")) {
					format_disk();
					print("\nDone!\n", 0x0A);
					ncount = 1;
				}
				else if (compare_strings(command, "run")) {
					char appname[32];
					print("App name: ", 0x0B);
					input_wait_string(appname);
					print("\n", 0x0F);

					struct App a;
					int found = 0;

					for (int i = 0; i < MAX_APPS; i = i + 1) {
						ata_read_sector(APPS_START_SECTOR + i, (unsigned short *)&a);

						if (a.exists && compare_strings(a.name, appname)) {
							unsigned char *load_address = (unsigned char *)0x100000;

							for (int s = 0; s < a.size_sectors; s = s + 1) {
								ata_read_sector(a.start_lba + s, (unsigned short *)(load_address + (s * 512)));
							}

							void(*run_me)() = (void(*)())0x100000;
							run_me();
							found = 1;
							break;
						}
					}
					if (!found) {
						print("App not found!\n", 0x0C);
					}
				}
				else if (compare_strings(command, "apps")) {
					struct App a;
					int apps_found = 0;

					print("\n", 0x0E); 

					for (int i = 0; i < MAX_APPS; i = i + 1) {
						ata_read_sector(150 + i, (unsigned short *)&a);

						if (a.exists == 1) {
							print("  - ", 0x0B);  
							print(a.name, 0x0F);  
							print("\n", 0x0F);
							apps_found++;
						}
					}

					if (apps_found == 0) {
						print("No apps installed.\n", 0x0C); 
					}
				}
				else {
					if (command[0] != '\0') {
						print("Uncnown command. Type 'help'\n", 0x0C);
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

				if (ncount == 1) {
					goto refresh;
				}

				if (show_crt_window == 1) {
					struct File f;

					is_window_crt = 1;

					for (int i = 0; i < 256; i++) { 
						filename[i] = 0; filecontent[i] = 0; 
					}

					x = 130; 
					y = 80;
					input_wait_string(filename);

					if (ncount == 1) {
						goto refresh;
					}

					x = 150;
					y = 110;
					input_wait_string(filecontent);

					if (ncount == 1) {
						goto refresh;
					}

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
				if (code != 0) {
					handle_hotkeys(code); 
				}
				if (ncount == 1) {
					goto refresh;
				}
			}
		}
	}

void __attribute__((section(".text.entry"))) kernel_main() {
	init_mouse();
	screen_clear();
	prompt();
}