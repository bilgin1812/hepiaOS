#include "types.h"
#include "kernel.h"
#include "syscall.c"

#define TEST

//string to test
char tab[]="... Kernel Loaded! \0";
uchar *sect="test ecriture \0";
char *testwrite="fichier ecrit \0";

char test;
char *ptr;
uchar buf[512];
uchar buf1[512];

void kernel() {
	
 	init_syscalls(); 

 	#ifdef TEST 

	//interrupt(0x80,1,tab,0,0,0);
	////interrupt(0x80,1,testwrite,0,0);

	#endif

	interrupt(0x80,4,5,sect);


	interrupt(0x80,3,5,buf);

   // interrupt(0x80,4,10,sect);
	//
	interrupt(0x80,3,10,buf);


	interrupt(0x80,4,7,"helooooo");


	interrupt(0x80,3,7,buf1);
	

	//interrupt(0x80,1,buf,0,0);

	while(1);
}



