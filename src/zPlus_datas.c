#include "zPlus_datas.h"





Datas* createDatas(Datas* old, int newPlayers) {
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
	
	return datas;
}



Datas* loadDatas(void) {
	Datas* datas;
	FILE* file;
	//unsigned long fileSz;
	size_t r;
	unsigned nPlayers;
	
	/* Ouverture ou création du fichier binaire en lecture et écriture. */
	file = fopen(DATAS_PATH, "rb");
	if(!file  &&  errno != ENOENT) {
		fputws(L"Erreur à l’ouverture en lecture du fichier ", stderr);
		perror(DATAS_PATH);
		fputws(L"Les données par défaut seront utilisées", stderr);
	}
	
	/* Écriture du fichier par défaut si nécessaire. */
	if(!file/*  ||  fileSz < SIZEOF_DATAS_HEADER + sizeof(*datas->players)*/) {
		datas = createDatas(NULL, 1);
		wcscpy(datas->players[0].name, DEFAULT_PLAYER_NAME);
	}
	
	else {
		/* Lecture de la 1ère info pour déterminer la taille attendue. */
		fread(&nPlayers, sizeof(nPlayers), 1, file);
		
		/* Allocation de mémoire. */
		datas = createDatas(NULL, nPlayers);
		
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
	FILE* file = fopen(DATAS_PATH, "wb");
	if(!file) {
		fputws(L"Erreur à l’ouverture du fichier de données ", stderr);
		perror(DATAS_PATH);
		fputws(L"Les données de cette exécution seront perdues.\n", stderr);
		return;
	}
	fwrite( &datas->nPlayers, sizeof(datas->nPlayers), 1,               file );
	fwrite( &datas->player1,  sizeof(datas->player1),  1,               file );
	fwrite( &datas->player2,  sizeof(datas->player2),  1,               file );
	fwrite( datas->players,   sizeof(*datas->players), datas->nPlayers, file );
	fclose(file);
}



void unloadDatas(Datas* datas) {
	writeDatas(datas);
	free(datas);
}





void  addPlayer
  (Datas** pdatas, const wchar_t* name)
{
	if((*pdatas)->nPlayers == MAX_PLAYERS) {
		fputws(
		  L"Impossible de créer un nouveau profil,"
		  L" le nombre maximal est déjà atteint.\n",
		  stderr);
		return;
	}
	
	*pdatas = createDatas(*pdatas, +1);
	wcscpy((*pdatas)->players[(*pdatas)->nPlayers-1].name, name);
}



void  deletePlayer
  (Datas** pdatas, unsigned playerID)
{
	if(!playerID  || playerID >= (*pdatas)->nPlayers)
		return;
	
	memmove(
	  (*pdatas)->players + playerID,
	  (*pdatas)->players + playerID + 1,
	  ( (*pdatas)->nPlayers - playerID - 1 ) * sizeof(*(*pdatas)->players)
	 );
	
	if(playerID == (*pdatas)->player1)
		(*pdatas)->player1 = 0;
	else if(playerID < (*pdatas)->player1)
		(*pdatas)->player1 --;
	
	if(playerID == (*pdatas)->player2)
		(*pdatas)->player2 = 0;
	else if(playerID < (*pdatas)->player2)
		(*pdatas)->player2 --;
	
	*pdatas = createDatas(*pdatas, -1);
}





void  updateLevelStats
  (PlayerDatas* player, unsigned lvl, unsigned tries, Uint32 chrono)
{
	player->levels[lvl].played       ++;
	player->levels[lvl].total.tries  += tries;
	player->levels[lvl].total.chrono += chrono;
	if( tries < player->levels[lvl].min.tries
	 || !player->levels[lvl].min.tries )
		player->levels[lvl].min.tries = tries;
	if( chrono < player->levels[lvl].min.chrono
	 || !player->levels[lvl].min.chrono )
		player->levels[lvl].min.chrono = chrono;
}



void  printLevelStats
  (PlayerDatas* player, unsigned lvl)
{
	unsigned optimaxTries = _log2(_pow(10, lvl)) + 1;
	
	wprintf(
	  L"[%ls] Statistiques pour le niveau %u :\n"
	  L"    parties jouées : %u\n"
	  L"    score moyen :    %+.2f\n"
	  L"    meilleur score : %+i\n"
	  L"    temps moyen :    %.2fs  (soit %.2fs par coup)\n"
	  L"    meilleur temps : %.2fs\n\n",
	  player->name, lvl,
	  player->levels[lvl].played,
	  optimaxTries
	    - player->levels[lvl].total.tries
	      / (double)player->levels[lvl].played,
	  optimaxTries - player->levels[lvl].min.tries,
	  player->levels[lvl].total.chrono / 1000.
	    / (double)player->levels[lvl].played,
	  player->levels[lvl].total.chrono / 1000.
	    / (double)player->levels[lvl].total.tries,
	 player->levels[lvl].min.chrono / 1000.
	);
}



void  printPlayerStats
  (PlayerDatas* player)
{
	wprintf( L"[%ls] Total de parties jouées : %u\n\n",
	  player->name,
	  player->levels[0].played
	 );
	
	for(unsigned i = 1;  i < 10;  i++)
		if(player->levels[i].played)
			printLevelStats(player, i);
}
