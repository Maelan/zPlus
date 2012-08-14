#include "zPlus_datas.h"





Datas* createDatas(Datas* old, /*FILE* file,*/ int newPlayers) {
	Datas* datas;
	
	unsigned nPlayers = newPlayers + ((old) ? old->nPlayers : 0);
	
	datas = realloc(old, sizeof(*datas) + nPlayers * sizeof(*datas->players));
	if(!datas) {
		free(old);
		fputws(L"Erreur mémoire : ", stderr);
		perror(NULL);
		exit(EXIT_FAILURE);
	}
	
	if(newPlayers > 0)
		memset(datas->players + nPlayers - newPlayers, 0,
		  newPlayers * sizeof(*datas->players));
	if(nPlayers == newPlayers  ||  datas->player1 >= nPlayers)
		datas->player1 = 0;
	if(nPlayers == newPlayers  ||  datas->player2 >= nPlayers)
		datas->player2 = 0;
	datas->nPlayers = nPlayers;
	//datas->file = file;
	
	return datas;
}



Datas* loadDatas(void) {
	Datas* datas;
	FILE* file;
	//unsigned long fileSz;
	size_t r;
	unsigned nPlayers;
	
	/* Ouverture ou création du fichier binaire en lecture et écriture. */
	file = fopen(DATAS_PATH, "rb"/*"r+b"*/);
	/*if(!file  &&  errno == ENOENT)
		file = fopen(DATAS_PATH, "w+b");
	if(!file) {
		fputws(L"Erreur à l’ouverture du fichier de données.\n", stderr);
		perror(DATAS_PATH);
		exit(EXIT_FAILURE);
	}*/
	/*fileSz = ftell(file);
	rewind(file);*/
	if(!file  &&  errno != ENOENT) {
		fputws(L"Erreur à l’ouverture en lecture du fichier ", stderr);
		perror(DATAS_PATH);
		fputws(L"Les données par défaut seront utilisées", stderr);
	}
	
	/* Écriture du fichier par défaut si nécessaire. */
	if(!file/*  ||  fileSz < SIZEOF_DATAS_HEADER + sizeof(*datas->players)*/) {
		datas = createDatas(NULL, /*file,*/ 1);
		wcscpy(datas->players[0].name, DEFAULT_PLAYER_NAME);
		//writeDatas(datas);
	}
	
	else {
		/* Lecture de la 1ère info pour déterminer la taille attendue. */
		fread(&nPlayers, sizeof(nPlayers), 1, file);
		
		/* Allocation de mémoire. */
		datas = createDatas(NULL, /*file,*/ nPlayers);
		
		/* Lecture complète. */
		fread(     &datas->player1, sizeof(datas->player1),  1,        file );
		fread(     &datas->player2, sizeof(datas->player2),  1,        file );
		r = fread( datas->players,  sizeof(*datas->players), nPlayers, file );
		
		if(r < nPlayers) {
			fputws(L"Erreur au chargement des données : ", stderr);
			perror(NULL);
			datas->nPlayers = r;
		}
	}
	
	if(file)
		fclose(file);
	
	/* Retour de la fonction. :o */
	return datas;
}



void writeDatas(const Datas* datas) {
	//rewind(datas->file);
	FILE* file = fopen(DATAS_PATH, "wb");
	if(!file) {
		fputws(L"Erreur à l’ouverture du fichier de données ", stderr);
		perror(DATAS_PATH);
		fputws(L"Les données de cette exécution seront perdues.\n", stderr);
		return;
	}
	fwrite( &datas->nPlayers, sizeof(datas->nPlayers), 1,   /*datas->*/file );
	fwrite( &datas->player1,  sizeof(datas->player1),  1,   /*datas->*/file );
	fwrite( &datas->player2,  sizeof(datas->player2),  1,   /*datas->*/file );
	fwrite( datas->players,   sizeof(*datas->players), datas->nPlayers,
	                                                        /*datas->*/file );
	fclose(file);
}



void unloadDatas(Datas* datas) {
	writeDatas(datas);
	//fclose(datas->file);
	free(datas);
}
