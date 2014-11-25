#include "syscall.h"

#define Decal(AH, AL)   (AH<<8|AL)
#define registre(AH, AL)   (AH<<8|AL)
#define TEST
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

		default:
		 		 interrupt(0x10, Decal(0x0e, 'c') ,0 ,0 ,0 ,0);
	}

}




void print_char(char c){
	if(c!='X')
   		 interrupt(0x10, Decal(0x0e, c) ,0 ,0 ,0 ,0);

   //implementation de saut de ligne
   //en modifiant la page		
   	else 
   		interrupt(0x10, Decal(0x0e, ' ') ,0,0 ,0 ,0);  
    
}



void print_string(char *str){
	int i=0;
	int offset;
	int ligne=0;
	int colonne=0;

	
	
	while (str[i]!='\0'){

		if(str[i]=='\n'){ //passe à la ligne
			ligne++;
			colonne=0;
			print_char('X');
			}
			
		else{
				print_char(str[i]);
							
			}

			i++;
						

	}
	

}

char read_char(){
	//lecture d'un char
 	byte value;
 	char ascii = interrupt(0x16,Decal(0x00,0), 0, 0, 0,0);
	print_char(ascii);
 	return ascii;

} 

char read_string(char *buf){
	int compteur=0;
	char lettre= read_char();
	while(lettre!='\r'){
		buf[compteur]=lettre;
		lettre= read_char();
		compteur++;
	}	
	//on ferme la chaine
	buf[compteur]='\0';
	//print_char(buf);

	return buf;
	

} 




void write_sector(int sector_number, uchar *buf){


	packet p;
	p.size = 0x10;
	p.reserved = 0;
	p.sect_count = 1;
	p.buf_offset = buf;
	//adr segment est defini dans le fichier .asm
	p.first_sect[0] = sector_number;
	p.first_sect[1] = 0;
	p.first_sect[2] = 0;
	p.first_sect[3] = 0;
	write_sector_raw(0x80, &p);
}



void read_sector(int sector_number, uchar *buf){
	packet p;
	p.size = 0x10;
	p.reserved = 0;
	p.sect_count = 1;
	p.buf_offset = buf;
	//adr segment est defini dans le fichier .asm
	p.first_sect[0] = sector_number;
	p.first_sect[1] = 0;
	p.first_sect[2] = 0;
	p.first_sect[3] = 0;
	read_sector_raw(0x80, &p);

	print_string(p.buf_offset);
}


