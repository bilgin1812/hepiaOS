#ifndef TYPES_H
#define TYPES_H

//call system with name 
#define PRINT_STRING 1
#define READ_STRING  2
#define READ_SECTOR  3
#define WRITE_SECTOR 4
#define GET_STAT	 5
#define READ_FILE	 6
#define REMOVE_FILE  7
#define ITER	     8


#define SECTOR_SFS  	10  //premier secteur ou on met sfs.img
#define SECTOR_SB   	10
#define SECTOR_BM   	12
#define SECTOR_FE   	14
#define SECTOR_FC   	48 //secor_sfs +34 
#define INDEX_BLOCK 	34
#define NBR_MAX_FILE 	64

#define TAILLE_SECTOR 512

typedef unsigned int uint;
typedef unsigned char byte;
typedef unsigned char uchar;

typedef struct stat_file {
	char filename[32];
	int size;
} stat_file;



#endif
