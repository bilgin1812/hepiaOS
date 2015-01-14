#include "types.h"
#include "kernel.h"

#define TEST  //for tester syscall etape3
//#define TEST_ETAPE2  //for tester syscall etape2


//buffer for test write
uchar *sect=" test ecriture \0";
//buffer for reading 
uchar *buf[512];
uchar *buf1[512];
uchar *buf2[512];

stat_file stat;

void kernel() {
	
 	init_syscalls(); 
 	//write ont the screen
 	interrupt(0x80,PRINT_STRING," Kernel loaded!  \0 ");

	
 	#ifdef TEST
 	interrupt(0x80,ITER,buf);
 	//interrupt(0x80,PRINT_STRING," --Lire fichier-- \0 ");
 	interrupt(0x80,READ_FILE,"fichier2.txt",buf1);
	interrupt(0x80,PRINT_STRING,buf1);
	interrupt(0x80,REMOVE_FILE,"fichier.txt");
	//interrupt(0x80,PRINT_STRING," --Efface fichier-- \0 ");
	interrupt(0x80,ITER,buf2);

	interrupt(0x80,GET_STAT,"fichier2.txt",&stat,0);
 	#endif

 	
 	#ifdef TEST_ETAPE2  // test des 4 premiers appels systems
 	//write buffer sect on the sector 5
	interrupt(0x80,WRITE_SECTOR,5,sect);
	//read sector 5
	interrupt(0x80,READ_SECTOR,5,buf1);
	//interrupt(0x80,PRINT_STRING,buf1);
	interrupt(0x80,PRINT_STRING,buf1 );

	//write string on the sector
	interrupt(0x80,WRITE_SECTOR,7," hello ");
	// read sector 7
	interrupt(0x80,READ_SECTOR,7,buf);
	interrupt(0x80,PRINT_STRING,buf);
	// read string entered by user
	interrupt(0x80,READ_SECTOR,10,buf1);
	interrupt(0x80,PRINT_STRING,buf1);
	#endif

	while(1);
}


