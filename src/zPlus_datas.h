#ifndef INCLUDED_ZPLUS_DATAS
#define INCLUDED_ZPLUS_DATAS

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <errno.h>
#include <SDL/SDL.h>    /* pour le type Uint32 */
#include "zPlus_utils.h"





#define  DATAS_PATH           "zPlus_datas.bin"
#define  MAX_PLAYERS          9
#define  MAX_PLAYER_NAME      31
#define  DEFAULT_PLAYER_NAME  L"<sans nom>"
#define  IA_NAME              L"<IA>"



/* Statistiques d’un joueur pour un niveau de difficulté. */
typedef struct {
	unsigned played;    /* nombre de parties jouées */
	struct {
		unsigned tries;    /* nombre de coups */
	    Uint32 chrono;     /* temps */
	} total,    /* nombres totaux toutes parties confondues */
	  min;      /* meilleures performances atteintes pour une partie */
} PlayerLevelDatas;

/* Informations sur un joueur. */
typedef struct {
	/* nom (ou pseudonyme) du joueur */
	wchar_t name[MAX_PLAYER_NAME+1];
	/* statistiques pour chaque niveau de difficulté ; la case [0] correspond au
	   total (et non au niveau « personnalisé »), et seul le nombre total de
	   parties est utilisé. */
	PlayerLevelDatas levels[10];
} PlayerDatas;

/* Données de tous les joueurs. */
typedef struct {
	unsigned nPlayers,       /* nombre total de joueurs enregistrés */
	         player1,        /* index du joueur 1 */
	         player2;        /* index du joueur 2 */
	PlayerDatas players[];   /* tous les joueurs enregitrés */
} Datas;



/* Structure des données dans le fichier binaire :
 * [unsigned]    nPlayers
 * [unsigned]    player1 (la dernière fois)
 * [unsigned]    player2 (la dernière fois)
 * [PlayerDatas] players[nPlayers]
 * Il doit y avoir au moins un joueur, le premier, le joueur « par défaut ».
 */

#define  SIZEOF_DATAS_HEADER  \
	( sizeof(unsigned) + sizeof(unsigned) + sizeof(unsigned) )



Datas*  createDatas
  (Datas* old, int newPlayers);


Datas*  loadDatas
  (void);


void  writeDatas
  (const Datas* datas);


void  unloadDatas
  (Datas* datas);



void  addPlayer
  (Datas** pdatas, const wchar_t* name);


void  deletePlayer
  (Datas** pdatas, unsigned playerID);



void  updateLevelStats
  (PlayerDatas* player, unsigned lvl, unsigned tries, Uint32 chrono);

void  printLevelStats
  (PlayerDatas* player, unsigned lvl);


void  printPlayerStats
  (PlayerDatas* player);



#endif    /* header not included */
