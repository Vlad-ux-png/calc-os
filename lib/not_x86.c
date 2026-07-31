#include <stdint.h>

void itoa2(int n, char* s) {
    int i = 0;
    int is_negative = 0;

    if (n == 0) {
        s[i++] = '0';
        s[i] = '\0';
        return;
    }

    if (n < 0) {
        is_negative = 1;
        n = -n;
    }

    while (n > 0) {
        s[i++] = (n % 10) + '0';
        n /= 10;
    }

    if (is_negative) {
        s[i++] = '-';
    }

    s[i] = '\0';
    
    for (int j = 0; j < i / 2; j++) {
        char temp = s[j];
        s[j] = s[i - j - 1]; 
        s[i - j - 1] = temp;
    }
}

void htoa2(int n, char str[]) {
    str[0] = '0';
    str[1] = 'x';
    const char *hex_chars = "0123456789ABCDEF";
    
    for (int i = 7; i >= 0; i--) {
        str[i + 2] = hex_chars[n & 0xF];
        n >>= 4;
    }
    str[10] = '\0';
}

int cmp_strings(const char *str1, const char *str2) {
    int i = 0;
    while (str1[i] == str2[i]) {
		    if (str1[i] == '\0') {
			      return 1;
		    }
		    i++;
	  }
	  return 0;
}
