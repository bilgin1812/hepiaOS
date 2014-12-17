#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
//fichier des structures





//typedef unsigned short int uint ;
typedef unsigned char byte;


//structure récupération information fichier
typedef struct __attribute__((__packed__)) sFileContent{
	byte data[256][1024];
}sFileContent;

typedef struct __attribute__((__packed__)) sFileEntry{
	byte name[32];
	uint16_t size; //2byte
	uint16_t indexBlock[222]; // index du block contenant les données
}sFileEntry;


typedef struct __attribute__((__packed__)) sBitmap{
	// 1 block = 2 secteurs(512) = 1024 bytes
	byte tabBitmap[1024];
}sBitmap;


typedef struct __attribute__((__packed__)) sSuperblock{
	char signature[8]; // signature du système de fichiers (8 bytes): SFSv0100
    uint16_t nbBlockTotal;
	uint16_t nbBlockSuperBlock;
	uint16_t nbSectBlock; // nombre de secteurs par block (2)
	uint16_t nbBlockBitmap; // taille du bitmap en blocks (1)
	uint16_t tailleFileEntries; // taille d'une file entry en bytes (256)
	uint16_t nbBlockFileEntries; // nombre de blocks réservés aux file entries (16)
	uint16_t nbBlockData; // nombre de blocks réservés pour le contenu des fichiers
	uint16_t padding[1006];
}sSuperblock;

typedef struct __attribute__((__packed__)) sSuperStruct{
	sSuperblock sb;
	sBitmap sbit;
	sFileEntry fe[64]; // file entries-> 4 par bloc
	sFileContent fc;
}sSuperStruct;

//structure servant à la lecture des informations
typedef struct __attribute__((aligned(1))) sStat_st{
	uint16_t size;
	int saut_secteur;
	int position; //position dans la fileEntry
}sStat_st;








#endif // STRUCT_H_INCLUDED
