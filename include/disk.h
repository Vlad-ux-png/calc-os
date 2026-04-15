#ifndef DISK_H
#define DISK_H

#define APPS_START_SECTOR 150  
#define MAX_APPS 16           
#define APP_DATA_START 200

struct File {
	char name[32];
	char content[476];
	int exists;
};

struct App {
	char name[32];
	int start_lba;    
	int size_sectors; 
	int exists;
};

/* * Прототипи функцій для роботи з ATA (LBA28)
 */

 /**
  * Читає один сектор з диска за вказаною адресою.
  * @param sector Номер сектора (LBA)
  * @param buffer Вказівник на буфер (мінімум 512 байт / 256 unsigned short)
  */
void ata_read_sector(int sector, unsigned short *buffer);

/**
 * Записує один сектор на диск.
 * @param sector Номер сектора (LBA)
 * @param buffer Вказівник на дані для запису (512 байт)
 */
void ata_write_sector(int sector, unsigned short *buffer);

/**
 * Очищує лише ту кількись пам'яті, яку може зайняти CalcOS
 */
void format_disk();

#endif