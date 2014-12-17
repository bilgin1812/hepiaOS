#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

#define TAILLE 10
#define BLOCK 256
#define LENGTH 32
#define BIT 8
#define MAX_INDEX 111

bool LengthSupported(char filename[LENGTH]);
//void init_tab(char *ptr, int taille);
sSuperStruct initStructure(char *name, uint16_t taille);
void sfsadd(sSuperStruct *sfs , char filename[LENGTH]);
int FileSize(char filename[LENGTH]);
char* sfslist(sSuperStruct* s, int *size_liste);
void copy_chain(char *str1, char *str2, int *compteur);
char *image; //buffer représentant l'image du disk
 sSuperStruct sfs;



 //copie de chaine de caractère
 void copy_chain(char *str1, char *str2, int *compteur){
	int i=0;
	printf("dans copy_chain");
	//printf("%s\n", str2);
	for(i=0; i<32; i++){
		str1[*compteur]=str2[i];
		printf("%c", str1[*compteur]);
		(*compteur)++;

	}
	printf("\n");
    str1[*compteur]='\n';
    (*compteur)++;
    printf("\n fin copy %s", str1);
}

/*
//on assure que l'on ait une continuité
void init_tab(char *ptr, int taille){
    char *crt=ptr;
    int i=0;
    for (i=0; i<taille; i++){
        crt[i]='\0';
    }
}
*/

sSuperStruct initStructure(char *name, uint16_t taille){
    int16_t i=0;
    char tab[taille*2*512];

    image=tab;

    sSuperStruct sfs;
   // init_tab(sfs.sb.signature, 8);

    while(name[i]!='\0'){
        sfs.sb.signature[i]=name[i];
        i++;
    }
    sfs.sb.nbBlockTotal=taille;
    sfs.sb.nbBlockSuperBlock=1;
    sfs.sb.nbSectBlock=2;
    sfs.sb.nbBlockBitmap=1; //1 block
    sfs.sb.tailleFileEntries=256; //nombre de bytes pour une file entrie
    sfs.sb.nbBlockFileEntries=16;
    sfs.sb.nbBlockData=taille-(sfs.sb.nbBlockFileEntries+sfs.sb.nbBlockSuperBlock+ sfs.sb.nbBlockBitmap);
    //init_tab(sfs.fe, 64);
    int j=0;
    /*
    for (j=0; j<64; j++){
     init_tab(sfs.fe[j].name, 32);
     init_tab(sfs.fe[j].indexBlock, 222);
     }

    int a;
    for (a=0;  a<256; a++ )
    init_tab(sfs.fc.data[a], 1024);
    init_tab(sfs.sbit.tabBitmap, 1024);
    */
    return sfs;

}

sSuperStruct createSfs(char* name, int16_t taille){
 sSuperStruct sfs;
 sfs=initStructure("sfs", taille);
 int compteur=0;
 char *t= &sfs;
 int j;
 for( j=0; j<sfs.sb.nbBlockSuperBlock*2*512; j++){

    image[j]=t[j];

 }
 //remplissage disk

return sfs;
}


//remplissage taille bitmap

void sfsadd(sSuperStruct *sfs , char filename[LENGTH]){
	// verifie que l'on peut stocker le fichier
	bool supported = LengthSupported(filename);
	if (supported == true){
		int i, j, r, h = 0;

		// lecture du fichier et recuperation du contenu
		FILE *fp;
		fp = fopen(filename, "r");
		char contenu[TAILLE];
		bool fileInit = false;
		int nbindex = 0; // nombre de tabIndexes qu'on aura dans le file entry
		int taille = 0;

		// recupere l'id du fichier pour l'ajouter
		int idFiles = 0;
		while (sfs->fe[idFiles].size != 0)
			idFiles++;

		// lecture du bitmap
		for (i=0; i< BLOCK/BIT; i++){
			for (j=0; j< BIT; j++){

				if (nbindex > MAX_INDEX)
					break;

				int bit = (sfs->sbit.tabBitmap[i]&(1<<j));
				if (bit == 0){ // si bit block libre
					if (fp == NULL){
						printf("impossible d'ouvrir le fichier\n");
						return;
					}else{
						sfs->sbit.tabBitmap[i] |= (1<<j) ; // bit block utilisé

						// initalisation du file entry avec le nom et la taille
						if (fileInit == false){
							// recupere le nom du fichier
							while (filename[h] != '\0'){
								sfs->fe[idFiles].name[h] = filename[h];
								h++;
							}
							// recuperer taille fichier
							sfs->fe[idFiles].size = FileSize(filename);
							fileInit = true;

						}
						// recupere l'index dans lequel est pour ranger le contenu (filecontent)
						int index = (i*BIT)+j+1;
						sfs->fe[idFiles].indexBlock[nbindex] = index;

						// se positionne à l'endroit où le bout de contenu s'est arreter
						if (nbindex > 0)
							fseek(fp, ((TAILLE-1)*nbindex), 0);

						fgets(contenu, TAILLE, fp); // recupere contenu fichier

						// ajoute le contenu du fichier par block dans le fileContent correspondant
						for (r=0; r< TAILLE; r++){
							sfs->fc.data[index][r] = contenu[r];
							taille++;
						}

						// tant qu'on arrive pas à la fin du fichier
						if(taille <= sfs->fe[idFiles].size){
							// vide le tableau contenant le contenu
							for (r=0; r< TAILLE; r++)
								contenu[r] = 0;

							nbindex++; // prochain tabindex

						}else{
							fileInit = false;
							return;
						}
					}
				}

			}
		}
	}else{
		printf("Impossible de stocker %s dans notre systeme de fichier car il est trop grand\n",filename);
		return;
	}
}

// Calcule la taille du fichier
int FileSize(char filename[LENGTH]){
	// ouvre le fichier pour le parcourir
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL){
		printf("impossible d'ouvrir le fichier\n");
		return;
	}

	// calcul la taille du fichier
	int taille = 0;
	while(fgetc(fp) != EOF)
		taille++;

	return taille;
}

//liste de l'ensemble des fichiers disponibles
char* sfslist(sSuperStruct* s, int *size_liste){
    uint i=0;
	uint j=0;
	char liste[2048]; //tout est stocké dans un tableau
    printf("on est dans sfslist\n");
    int compteur=0;
	//while ((s->fe[i].name[0]<<7)>>7){
	while(s->fe[i].size!=0){

        printf("ok\n");
        printf("on copie %s\n", s->fe[i].name );
        copy_chain(liste,s->fe[i].name, &compteur);
        printf("compteur vaut %d", compteur);
		i++;
	}
    *size_liste=compteur;
	liste[2047]='\0';
	printf("la liste\n");
	printf("%s", liste);
	return liste;
}


// Verifie si la taille du fichier peut être stoquer dans le systeme
bool LengthSupported(char filename[LENGTH]){
	int size = FileSize(filename);
	if (size > (MAX_INDEX * TAILLE))
		return false;
	return true;

}


void main()
{

    printf("Hello world!\n");
    int i=0;
    char *liste;
    int taille_liste=0;
    int16_t nb_block= 500;
    sfs = createSfs("sfstest", nb_block);
    printf("taille de sfs en block %d\n", sfs.sb.nbBlockTotal);
    sfsadd(&sfs, "test.txt");
    sfsadd(&sfs, "tiki.txt");
    sfsadd(&sfs, "toto.txt");
    liste=sfslist(&sfs,&taille_liste);
    for(i=0; i<taille_liste; i++)
        printf("%c", liste[i]);

}
