#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "struct.h"
#define CHECK_BIT(var, pos) ((var) &(1<<pos))


void init_tab(char *ptr, int taille);
sSuperStruct initStructure(char *name, uint16_t taille);
void sfsadd(sSuperStruct *sfs , char *filename);

char* sfslist(sSuperStruct* s, int *size_liste);
void copy_chain(char *str1, char *str2, int *compteur);
char *image; //buffer représentant l'image du disk
sSuperStruct sfs;



//copie de chaine de caractère
void copy_chain(char *str1, char *str2, int *compteur){
	int i=0;
	printf("dans copy_chain \n");
	//printf("%s \n", str2);
	for(i=0; i<32; i++){
		str1[*compteur]=str2[i];
		printf("%c", str1[*compteur]);
		(*compteur)++;

	}
	printf("\n");
	str1[*compteur]='n';
	(*compteur)++;
	printf("fin copy %s \n", str1);
}

//on assure que l'on ait une continuité
void init_tab(char *ptr, int taille){
	char *crt=ptr;
	int i=0;
	for (i=0; i<taille; i++){
		crt[i]='\0';
	}
}

void init_byte(byte *ptr, int taille){
	byte *crt=ptr;
	int i=0;
	for (i=0; i<taille; i++){
		crt[i]=0;
	}
}


sSuperStruct initStructure(char *name, uint16_t taille){
	int16_t i=0;
	char tab[taille*2*512];

	image=tab;

	sSuperStruct sfs;
	init_tab(sfs.sb.signature, 8);

	//remplissage de la signature
	while(name[i]!='\0'){
		sfs.sb.signature[i]=name[i];
		i++;
	}

	//remplissage structure complète
	sfs.sb.nbBlockTotal=taille;
	sfs.sb.nbBlockSuperBlock=1;
	sfs.sb.nbSectBlock=2;
	sfs.sb.nbBlockBitmap=1; //1 block
	sfs.sb.tailleFileEntries=256; //nombre de bytes pour une file entrie
	sfs.sb.nbBlockFileEntries=16;
	sfs.sb.nbBlockData=taille-(sfs.sb.nbBlockFileEntries+sfs.sb.nbBlockSuperBlock+ sfs.sb.nbBlockBitmap);
	init_tab(sfs.fe, 64);
	int j=0;
	for (j=0; j<64; j++){
		sfs.fe[j].size=0;
		init_tab(sfs.fe[j].name, 32);
		init_tab(sfs.fe[j].indexBlock, 111);
	}

	int a;
	for (a=0;  a<1024; a++ )
		init_byte(sfs.fc.data[a], 1024);

	init_byte(sfs.sbit.tabBitmap, 1024);

	return sfs;

}



sSuperStruct createSfs(char* name, int16_t taille){
	sSuperStruct sfs;
	sfs=initStructure(name, taille);

	return sfs;
}


//recherche dans le tableau de bitmap la première place libre->retourne le numéro du bit
int get_bit_free(){
	int i;
	int j;
	int compteur=0;
	printf("dans free_bit \n");
	for (i=0; i<1024; i++){
		//parcours des bits du byte
		printf("%d \n",sfs.sbit.tabBitmap[i] );
		for (j=0; j<8; j++ ){
			if (sfs.sbit.tabBitmap[i]&(1<<j)){
				compteur++;
			}
			else{
				//printf(sfs.sbit.tabBitmap[i]);
				printf("On sort de get_bit et le bit %d \n", compteur);
				return compteur;
			}
		}

	}
	printf("On sort de get_bit2 \n");
	return compteur;

}

// check si le fichier existe
bool fileExists(const char* file) {
	struct stat buf;
	return (stat(file, &buf) == 0);
}

void sfsadd(sSuperStruct *s, char *name){
	printf("dans sfs add avec fichier : %s \n", name);
	if(fileExists(name)){
		int compteur=0;
		int add_taille=0; //addition des tailles->permet de récupérer la taille du bitmap
		int num_bit;
		//quelle place est encore disponible dans les files entries
		if(s->fe[0].size!=0)
		printf("le premier fichier est %s \n",s->fe[0].name);

		// CHECK_BIT(var, pos) ((var) &(1<<pos))
		while( CHECK_BIT(s->fe[compteur].name[0], 6) ){
		//while(s->fe[compteur].size!=0){
			//printf("parcours fichier %s",s->fe[compteur].name);
			compteur++;
		}
		printf("on recupere la taille des num_bit \n");
		num_bit=get_bit_free();
		printf("on a récuperer les bits \n");

		// printf("nous sommes au %d compteur et add_taille vaut %d num_bit %d \n", compteur, add_taille, num_bit);



		//récupération taille fichier
		struct stat st;

		stat(name, &st);
		printf("la taille du fichier est %d \n", st.st_size);
		strcpy(s->fe[compteur].name, name);
		s->fe[compteur].size=st.st_size;
		int num=0;
		//remplissage des bytes du bitmap
		int nbre_bloc= (st.st_size/1024)+1; //obtient le nombre de bloc == le nombre de bits du bitmap

		int byte_asked; //le byte du tab_bitmap sur lequel l'on travail
		byte buf[1024];
		init_byte(buf, 1024);
		int i, fp;
		fp=fopen(name, "r");
		for(i=num_bit; i<num_bit+nbre_bloc; i++){
			byte_asked= i/8; //quel byte de tabBitmap est affecté?
			s->fe[compteur].indexBlock[num]=i; //enregistre le numéro du bloc dans la file entries
			s->sbit.tabBitmap[byte_asked]=s->sbit.tabBitmap[byte_asked]^(1<<(i%8)); //enregistre la présence du fichier sur le bit XXX byte_asked
			printf("remplissage du tabBitmap et le byte vaut %d \n",s->sbit.tabBitmap[byte_asked]);
			fread(buf, 1024,1, fp); //lecture des bytes du fichier demandée
			printf("LE BUFFER contient %s \n", buf);

			file_to_buffer(s->fc.data[i], buf, 1024);
			printf("Après copies l'on a dans le tableau %s \n", s->fc.data[i]);
			num++;
		}

		fclose(fp);
	} else {
		printf("Fichier existe pas : %s \n\n", name);
	}
}

void file_to_buffer(byte *data, byte *values, int size){
	int i=0;
	for (i=0; i<size; i++)
		data[i]=values[i];
	// data=values;
	printf("file to buffer montre la phrase %s \n", data);

}

//liste de l'ensemble des fichiers disponibles
char* sfslist(sSuperStruct* s, int *size_liste){
	uint i=0;
	uint j=0;
	char liste[2048]; //tout est stocké dans un tableau
	printf("on est dans sfslist \n");
	int compteur=0;
	//while ((s->fe[i].name[0]<<7)>>7){
	while(s->fe[i].size!=0){

		printf("ok \n");
		printf("on copie %s \n", s->fe[i].name );
		copy_chain(liste,s->fe[i].name, &compteur);
		printf("compteur vaut %d \n", compteur);
		i++;
	}
	*size_liste=compteur;
	liste[2047]='\0';
	printf("la liste \n");
	printf("%s \n", liste);
	return liste;
}


void createImage(){

	int compteur=0;
	FILE *fp = fopen("sfs.img","wb");
	//printf("o.k fichier ouvert %d \n", *fp);
	byte j=0;
	//18 blocks sont réservé pour superblock et métadonné
	printf("taille de sb est %d \n", sizeof(sfs.sb.nbBlockFileEntries));
	int i;
	for (i=0;i<sfs.sb.nbBlockTotal*1024; i++)
		fwrite(&j, sizeof(j), 1, fp);
	close(fp);

	fp = fopen("sfs.img","wb");
	fwrite(&sfs.sb, sizeof(sfs.sb),1, fp);
	fwrite(&sfs.sbit.tabBitmap, sizeof(uint8_t), 1024, fp);

	//inscription de tout le file entries
	/*
	while(sfs.fe[compteur].size!=0){
		compteur++;
	}
	*/

	//fwrite(&sfs.fe, sizeof(sfs.fe), compteur, fp);
	fwrite(&sfs.fe, sizeof(sfs.fe), 64, fp); // on a 64 file entries
	printf("sb est ecrit \n");
	close (fp);


	/*fwrite(&sfs.fe, sizeof(sfs.fe), 16, fp);
	fwrite(&sfs.fc, sizeof(uint8_t), 1024*1024, fp);
	printf("FICHIER ECRIIIIIIIT \n");

	close (fp);*/
}


void print_file(byte *data){
	int i=0;
	printf("dans printf file le tableau est composé du texte suivant \n");
	while(data[i]!='\0'){
		printf("%c", data[i]);
		i++;
	}
	printf("\n");

}

void setBitmapZero( sSuperStruct *s, int index ){
	int bit = index / 8;
	int reste = index % 8;
	// met le bit à 0
	printf("BITZERO %d %d %d \n", index, bit, reste);
	s->sbit.tabBitmap[bit] &= ~(1 << reste);
}
void sfsdel(sSuperStruct *s, char* filename){

	printf("DELETE %s \n", filename);
	int i = 0;
	// chercher le bon file entry
	while( strcmp(s->fe[i].name, filename) && i < 64) {
		i++;
	}
	printf("DELETE %d %s %s \n", i, filename, s->fe[i].name);
	if(i == 64){
		printf("Impossible de supprimer le fichier, il n'existe pas \n");
	} else {
		// on met le name et le size à 0
		int j;
		char buf[32];
		init_tab(buf, 32 );
		printf("DELETE BUF %s \n", buf);
		strcpy(s->fe[i].name, buf);
		printf("DELETE NAME: %s :ICI \n", s->fe[i].name);
		printf("DELETE SIZE: %d :ICI \n", s->fe[i].size);
		for(j=0; j <= (s->fe[i].size/1024); j++){
			// on met les bit à zero dans le bitmap
			setBitmapZero( s, s->fe[i].indexBlock[j] );
		}
		s->fe[i].size=0;
	}
}

int main()
{

	printf("Hello world! \n");
	int i=0;
	char *liste;
	int16_t taille_liste=0;
	int16_t nb_block= 500;
	sfs = createSfs("sfstest", nb_block);
	printf("dans createSfs la signature est %s \n", sfs.sb.signature);
	printf("taille de sfs en block %d \n", sfs.sb.nbBlockTotal);
	sfsadd(&sfs, "fichier.txt");
	sfsadd(&sfs, "fichier2.txt");


	liste=sfslist(&sfs,&taille_liste);
	printf("Dans main liste est \n");
	for(i=0; i<taille_liste; i++)
		printf("%c", liste[i]);
	printf("\n");;

	printf("sfs.name[0] %s \n", sfs.fe[0].name);
	printf("sfs.name[1] %s \n", sfs.fe[1].name);
	printf("sfs.name[2] %s \n", sfs.fe[2].name);
	print_file(sfs.fc.data[2]);
	createImage();
	return 0;
}
