extern int get_scancode();

int cursor = 0;

unsigned char *vga = (unsigned char*)0xb8000;

void put_char(char s, char color) {
	if (s == '\n') {
		int current_row = cursor / 160;

		cursor = (current_row + 1) * 160;
	}
	else {
		vga[cursor] = s;
		vga[cursor + 1] = color;
		cursor += 2;
	}
}

void screen_clear() {
	cursor = 0;
	for (int i = 0; i < 2000; i++) {
		put_char(' ', 0x0F);
	}
	cursor = 0;
}

void print(char* message, char color) {
	int i = 0;
	while (message[i] != '\0') {
		put_char(message[i], color);
		i++;
	}
}

void input_wait_string(char *buffer) {
	int i = 0;
	int code;

	while (1) {
		code = get_scancode();

		if (code == 0x1C) {
			buffer[i] = '\0';
			put_char(' ', 0x0F);
			return;
		}

		char letter = 0;

		switch (code) {
		case 0x18: letter = 'o'; break;
		case 0x23: letter = 'h'; break;
		case 0x2E: letter = 'c'; break;
		case 0x1E: letter = 'a'; break;
		case 0x26: letter = 'l'; break;
		case 0x12: letter = 'e'; break;
		case 0x13: letter = 'r'; break;
		case 0x2D: letter = 'x'; break;
		case 0x17: letter = 'i'; break;
		case 0x14: letter = 't'; break;
		case 0x19: letter = 'p'; break;
		case 0x10: letter = 'q'; break;
		case 0x11: letter = 'w'; break;
		case 0x15: letter = 'y'; break;
		case 0x16: letter = 'u'; break;
		case 0x1F: letter = 's'; break;
		case 0x20: letter = 'd'; break;
		case 0x21: letter = 'f'; break;
		case 0x22: letter = 'g'; break;
		case 0x24: letter = 'j'; break;
		case 0x25: letter = 'k'; break;
		case 0x2C: letter = 'z'; break;
		case 0x2F: letter = 'v'; break;
		case 0x30: letter = 'b'; break;
		case 0x31: letter = 'n'; break;
		case 0x32: letter = 'm'; break;
		case 0x39: letter = ' '; break;
		case 0x02: letter = '1'; break;
		case 0x03: letter = '2'; break;
		case 0x04: letter = '3'; break;
		case 0x05: letter = '4'; break;
		case 0x06: letter = '5'; break;
		case 0x07: letter = '6'; break;
		case 0x08: letter = '7'; break;
		case 0x09: letter = '8'; break;
		case 0x0A: letter = '9'; break;
		case 0x0B: letter = '0'; break;
		case 0x0C: letter = '-'; break;
		case 0x0D: letter = '='; break;
		case 0x0E:
			if (i > 0) {
				i--; 
				cursor -= 2;
				put_char(' ', 0x0F);
				cursor -= 2;
			}
			letter = 0; 
			break;

		default:
			letter = 0; 
			break;
		}

		if (letter != 0 && i < 255) { 
			put_char(letter, 0x0F);  
			buffer[i] = letter;      
			i++;
		}
	}
}

int compare_strings(char *s1, char *s2) {
	int i = 0;
	while (s1[i] != '\0' && s2[i] != '\0') {
		if (s1[i] != s2[i]) return 0; 
		i++;
	}
	return (s1[i] == s2[i]);
}

void print_number(int n, char color) {
	if (n == 0) {
		put_char('0', color);
		return;
	}

	char buf[10];
	int i = 0;

	while (n > 0) {
		buf[i++] = (n % 10) + '0';
		n /= 10;
	}

	while (i > 0) {
		put_char(buf[--i], color);
	}
}

int input_wait_number() {
	int result = 0;

	while (1) {
		int code = get_scancode();

		if (code == 0x1C) {
			put_char(' ', 0x0F);
			return result;
		}

		if (code >= 2 && code <= 11) {
			int digit = (code == 11) ? 0 : (code - 1);

			result = result * 10 + digit;
			put_char(digit + '0', 0x0E);
		}
	}
}

void run_calculator() {
	print("\n----Caluclator----\n", 0x0B);
	put_char(' ', 0x0F);

	print("Enter first number: ", 7);
	int first = input_wait_number();

	print("Enter second number: ", 7);
	int second = input_wait_number();

	int result = first + second;

	print("Result: ", 0x0A);

	print_number(result, 0x0F);

	print("\n\nPress ENTER to exit...\n", 8);
	while (get_scancode() != 0x1C);
}

void __attribute__((section(".text.entry"))) kernel_main() {
	char cmd[256];
	screen_clear();

	while (1) {
		print("CALC OS>", 0x0E);

		input_wait_string(cmd); 

		if (compare_strings(cmd, "cln")) {
			screen_clear();
		}
		else if (compare_strings(cmd, "help")) {
			print("\nCommands: cls, help, calc\n", 0x07);
		}
		else if (compare_strings(cmd, "calc")) {
			run_calculator();
		}
		else {
			print("\nBad command!\n", 0x04);
		}
	}
}