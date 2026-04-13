#include <disk.h>
#include <stdio.h>
#include <mouse.h>
#include <utils.h>
#include <keyboard.h>

struct File {
	char name[32];
	char content[476];
	int exists;
};

void ata_read_sector(int sector, unsigned short *buffer) {
	outb(0x1F2, 1);
	outb(0x1F3, sector);
	outb(0x1F4, 0);
	outb(0x1F5, 0);
	outb(0x1F6, 0xE0);
	outb(0x1F7, 0x20);

	while ((inb(0x1F7) & 0x08) == 0);

	for (int i = 0; i < 256; i = i + 1) {
		buffer[i] = inw(0x1F0);
	}
}

void ata_write_sector(int sector, unsigned short *buffer) {
	while ((inb(0x1F7) & 0x80) != 0);

	outb(0x1F2, 1);
	outb(0x1F3, sector);
	outb(0x1F4, 0);
	outb(0x1F5, 0);
	outb(0x1F6, 0xE0);
	outb(0x1F7, 0x30);

	while ((inb(0x1F7) & 0x08) == 0);

	for (int i = 0; i < 256; i = i + 1) {
		outw(0x1F0, buffer[i]);
	}
}

void format_disk() {
	struct File empty_f;

	empty_f.exists = 0;
	for (int i = 0; i < 32; i++) {
		empty_f.name[i] = 0;
	}
	for (int i = 0; i < 476; i++) {
		empty_f.content[i] = 0;
	}

	for (int i = 0; i < MAX_FILES; i++) {
		ata_write_sector(MAX_SECTORS + i, (unsigned short *)&empty_f);
	}

	for (int i = 0; i < MAX_FILES; i++) {
		ata_write_sector(MAX_SECTORS + i, (unsigned short *)&empty_f);

		for (int i = 0; i < 1000; i++) {
			asm volatile("nop");
		}
	}
}