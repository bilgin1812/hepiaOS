
#include "types.h"

/* retourne la taille d'un string */
int string_length(char *str){
        int i;
        while (str[i]!='\0')
                i++;
        return i;
}

/* comparaison de deux string */
int str_egal(char *str1, char* str2, int length){
        int i=0;
        while (i < length){
                if(str1[i] != str2[i]) return 1;
                i++;
        }
        return 0;
}

/* Copie string */
char* strncpy(char *s1, char *s2, int n){
	char *s = s1;
	while (n > 0 && *s2 != '\0') {
		*s++ = *s2++;
		--n;
	}
	while (n > 0) {
		*s++ = '\0';
		--n;
	}
	return s1;
}
/********************/


int compare_string(char *first_string, char *second_string){
	int i=0;
	int j=0;	
	while(first_string[i]==second_string[j]){
		if(first_string[i]=='\0' && second_string[j]=='\0')
			return 1;
		i++;
		j++;
	}
	//on arrive a la fin de la boucle
	return -1;

}



//lecture du fichier
int read_file(char *filename, uchar *buf){



	 int offst=0;
	 int debut_index_block = INDEX_BLOCK;
	 int sector_courant=SECTOR_FE; //sector_fileEntry=14
	 int tmp1;
	 int tmp2;
	 int cpt=0;
	 int taille_contenu_sector;
	 int index_file=INDEX_BLOCK;
	 int k;

	 uchar contenu[TAILLE_SECTOR];
	 stat_file st;
	 st.size=10;
	 //interrupt(0x80,PRINT_STRING, " cherche fichier");
		for(k=0; k<NBR_MAX_FILE;k++){
			interrupt(0x80,READ_SECTOR,sector_courant, contenu,0,0); 
			
			if(offst==0){
				offst = TAILLE_SECTOR/2; //256
			}
			else{
				offst=0;
				sector_courant++;
			}

			//on a trouve le bon file_entry
			if(compare_string(filename,&contenu[offst]) ==1)
			{
				interrupt(0x80,PRINT_STRING, " fichier trouve ",0,0);
				/* tableau des index */
				debut_index_block += offst;
					/* lire le contenue */

				/*  Parcour sur les tables index */
				do {
				
					tmp1 = contenu[debut_index_block++];
					tmp2 =contenu[debut_index_block++];
					index_file = tmp1+(tmp2<<8);
					/* sector contenue */
					sector_courant = SECTOR_FC + index_file;
					/* lire le contenue */
					interrupt(0x80,READ_SECTOR,sector_courant, contenu,0,0);
					/* on met contenue dans buf */
					strncpy(buf,&contenu, TAILLE_SECTOR);
					interrupt(0x80,PRINT_STRING,contenu,0,0);
					 
					/* */
					if (string_length(contenu) == TAILLE_SECTOR)
					{
						interrupt(0x80,READ_SECTOR,sector_courant+1, contenu,0,0);
						strncpy(buf, &contenu, TAILLE_SECTOR); // copy the content in the buffer

					}
				} while (index_file != 0);
				interrupt(0x80,PRINT_STRING, " fin ! fichier lu ");
				
					return 1;
					/* */

					break;
			} // fin if(compare)
			

		}	//fin boucle for
	return -1;
}


int remove_file(char *filename)
{

	 int offst=0;
	 int debut_index_block = INDEX_BLOCK;
	 int sector_courant=SECTOR_FE; //sector_fileEntry=14
	 int tmp1;
	 int tmp2;
	 int cpt=0;
	 int taille_contenu_sector;
	 int index_file=INDEX_BLOCK;
	 int bitmap_sector=SECTOR_BM;
	 int k;
	 unsigned int indexBitmap;
		unsigned int decalage;

	 uchar contenu[TAILLE_SECTOR];
	 uchar bitmap[TAILLE_SECTOR];
	 stat_file st;
	 st.size=10;

		for(k=0; k<NBR_MAX_FILE;k++){
			interrupt(0x80,READ_SECTOR,sector_courant, contenu,0,0); 
			
			if(offst==0){
				offst = TAILLE_SECTOR/2;
			}
			else{
				offst=0;
				sector_courant++;
			}

			//on a trouve le bon file_entry
			if(str_egal(filename,&contenu[offst]) ==0 )
			{
				interrupt(0x80,PRINT_STRING, " fichier trouve ");
				/* on met 0 au debut du nom du fichier */
				contenu[offst]='0';
				interrupt(0x80, WRITE_SECTOR, sector_courant, contenu, 0, 0);

				/* lire bitmap */
				interrupt(0x80,READ_SECTOR,bitmap_sector, bitmap,0,0);

				debut_index_block += offst;
					/* lire le contenue */

				/*  Parcour sur les tables index */
				do {
				
					tmp1 = contenu[debut_index_block++];
					tmp2 =contenu[debut_index_block++];
					index_file = tmp1+(tmp2<<8);

					indexBitmap = index_file/8;
					decalage = (index_file%8)-1;


					bitmap[indexBitmap] &= ~(1<<decalage);


				} while (index_file != 0);


				interrupt(0x80, WRITE_SECTOR, bitmap_sector, bitmap, 0, 0);
				
				interrupt(0x80,PRINT_STRING, " fichier efface ");
				
					return 1;
					/* */

					break;
			} // fin if(compare)
			

		}	//fin boucle for
	return -1;

}


int get_stat(char *filename,stat_file *stat)
{
        char *string;
        char taille[2];
        int test ;
        test = iter(filename);   
        if(test){

                interrupt(0x80, PRINT_STRING, "Fichier_trouve \0");
                //string=itoa(stat->size, taille);
                interrupt(0x80, PRINT_STRING, stat->filename);
                //interrupt(0x80, PRINT_STRING, string);
        }
}


/********Iteration***********/


//source : StackOverflow : http://stackoverflow.com/questions/9655202/how-to-convert-integer-to-string-in-c

/*
char* itoa(int i, char b[]){
    char digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}
*/

//fonction d'itération, parcours de tous les fichiers présent
int iter(char *filename){
        int test;
        int i;
        uint j;
        char taille_file[2];
        char *taille;
        int retour; //si il y a qqch en argument il faudra teste
        uchar buf[512]; //lecture du secteur
        uchar file[32];
        int size = 0; // taille fichier
        int sect = 0; // secteur à chercher
        int next = 0; // 2 fe par secteur, variable pour savoir lequel lire
        interrupt(0x80, PRINT_STRING, "fonction iter \0");
        read_sector( SECTOR_SFS + 4 + sect, buf);

        retour = string_length(filename);

        while( (sect < 4) && ((buf[0+next]) & (1<<6))){
               
                //jump au system de fichier
                if(next == 0){
                        // secteur du systeme de fichier + 2sect(superblock) + 2sect(Bitmap)
                        //JUMP 256 bytes plus loin après verification
                        read_sector( SECTOR_SFS + 4 + sect, buf);
                }

                //
                for(i = 0; i < 32; i ++)
                        file[i] = buf[i + next];
                       
                //on désire un test sur filename                       
                 if (retour>0)
                         test=str_egal(file, filename, 32);
                 if (test=0)
                        test =1;
                else
                        test=0;        
                       
                       
                // le size est sur 2 bytes, on concatène les int en int8
                size = buf[32 + next] | buf[33 + next] << 8;   
                //taille = itoa(size, taille_file);            
                if(next == 0)
                        next = 256;
                else {
                        next = 0;
                        sect++;
                }
                interrupt(0x80, PRINT_STRING, file);
               
               
        }

       
        return test;

} //fin iter




/*****************************/
