
#include "types.h"
#define Decal(AH, AL)   (AH<<8|AL)
#define registre(AH, AL)   (AH<<8|AL)
#define TEST

char read_string(char *buf);  
char read_char();

//call system
void print_char(char c);
void print_string(char *str);
void read_sector(int sector, uchar *buf);
void write_sector(int sector, uchar *buf);


extern int get_stat(char *filename, struct stat_st *stat);
extern int read_file(char *filename, unsigned char *buf);
extern int remove_file(char *filename);
extern int iter(int a);



//int interrupt(int number, int ax, int bx, int cx, int dx, int di)

//packet for reading and writing sector
typedef struct packet {
	uchar size;
	uchar reserved;
	uint sect_count;
	uint buf_offset;
	uint buf_segment;
	uint first_sect[4];

} packet;

//handler

extern void syscall_handler(uint syscall_nb, uint arg1, uint arg2, uint arg3) {

	switch(syscall_nb){

		case 1:
			print_string(arg1); //passe l'adresse du tableau
			break;
		case 2:
			print_string((char) read_string(arg1));
			break;
		case 3:
			read_sector(arg1,arg2);
			break;
		case 4:
			write_sector(arg1, arg2);
			break;
		case 5:
			get_stat(arg1,arg2);
			break;
		case 6:
			read_file(arg1,arg2);
			break;
		case 7:
			remove_file(arg1);
			break;
		case 8:
			iter(arg1);
			break;

	}

}




void print_char(char c){
	if(c!='X')
   		 interrupt(0x10, Decal(0x0e, c) ,0 ,0 ,0 ,0);

   //changer page for next line		
   	else 
   		interrupt(0x10, Decal(0x0e, ' ') ,0,0 ,0 ,0);  
    
}



void print_string(char *str){
	int i=0;
	int offset;
	int ligne=0;
	int colonne=0;

	
	
	while (str[i]!='\0'){  // while not end , end is with 0 at the end of the string

		if(str[i]=='\n'){  // change the line
			ligne++;
			colonne=0;
			print_char('X');
			}
			
		else{
				print_char(str[i]);  // print all of the caracters with appel func.
							
			}

			i++;
						

	}
	

}

char read_char(){
	//read char with interruption 16
 	byte value;
 	char ascii = interrupt(0x16,Decal(0x00,0), 0, 0, 0,0);
	print_char(ascii);
 	return ascii;

} 

char read_string(char *buf){
	//length string
	int compteur=0;
	//read caracter , call function
	char lettre= read_char();
	//reading all of the caracters entered
	while(lettre!='\r'){
		buf[compteur]=lettre;
		lettre= read_char();
		compteur++;
	}	
	//put 0 at the end of the string
	buf[compteur]='\0';
	//return string read
	return buf;
	

} 




void write_sector(int sector_number, uchar *buf){


	packet p;
	p.size = 0x10;
	p.reserved = 0;
	p.sect_count = 1;
	p.buf_offset = buf;
	//adr segment int the file  .asm
	p.first_sect[0] = sector_number;
	p.first_sect[1] = 0;
	p.first_sect[2] = 0;
	p.first_sect[3] = 0;
	//call function .asm
	write_sector_raw(0x80, &p);
}



void read_sector(int sector_number, uchar *buf){
	packet p;
	p.size = 0x10;
	p.reserved = 0;
	p.sect_count = 1;
	p.buf_offset = buf;
	//adr segment is in the file .asm
	p.first_sect[0] = sector_number;
	p.first_sect[1] = 0;
	p.first_sect[2] = 0;
	p.first_sect[3] = 0;
	//call function asm
	read_sector_raw(0x80, &p);

}


