#include "types.h"
#include "kernel.h"

#define Decal(AH, AL)   (AH<<8|AL)
#define registre(AH, AL)   (AH<<8|AL)
//int interrupt(int number, int ax, int bx, int cx, int dx, int di)


char tab[]="salut";
byte t[] = "bonjour !";
byte sect[512];
char test;
char *ptr;

struct packet
{
	uchar sz;
	uchar reserved;
	uint count;
	uint offset;    //adresse
	uint segment;
	uint index;		//firstpacket
	uint index2[3];  //0

};

void kernel() {
	
 	init_syscalls(); 
 	interrupt(0x80,1,':',0,0,0);
 	interrupt(0x80,1,' ',0,0,0);
	interrupt(0x80,2,tab,0,0,0);
	interrupt(0x80,3,0,0,0,0);
	interrupt(0x80,4,0,0,0,0);


	interrupt(0x80,6,5,t,0,0);
	interrupt(0x80,5,1,sect,0,0);
	while(1);
}


extern void syscall_handler(uint syscall_nb, uint arg1, uint arg2, uint arg3) {

	switch(syscall_nb){
		case 1:{
				print_char((char) arg1);
				break;
			}
			
		case 2:
			print_string(arg1); //passe l'adresse du tableau
			break;
		case 3:
			read_char();
			break;
		case 4:
			read_string();
			break;
		case 5:
			read_sector(arg1, arg2);
			print_string((char) arg2);
			break;
		case 6:
			write_sector(arg1, arg2);
			break;
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
	print_char(buf);

	return buf;
	

} 





/* Int13
AH = 02h
AL = number of sectors to read (must be nonzero)
CH = low eight bits of cylinder number
CL = sector number 1-63 (bits 0-5)
high two bits of cylinder (bits 6-7, hard disk only)
DH = head number
DL = drive number (bit 7 set for hard disk)
ES:BX -> data buffer
*/
void read_sector(int sector, byte *buf){
	interrupt(0x13,Decal(0x02,1),  buf , Decal(0, sector),  0,0);
}

/* Int13
AH = 03h
AL = number of sectors to write (must be nonzero)
CH = low eight bits of cylinder number
CL = sector number 1-63 (bits 0-5)
high two bits of cylinder (bits 6-7, hard disk only)
DH = head number
DL = drive number (bit 7 set for hard disk)
ES:BX -> data buffer
*/

void write_sector(int sector, byte *buf){
	interrupt(0x13,Decal(0x03,1), buf, Decal(0, sector),  0,0);
}

