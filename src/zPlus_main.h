#ifndef INCLUDED_ZPLUS_MAIN
#define INCLUDED_ZPLUS_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include <limits.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
//#include <SDL/SDL_ttf.h>
#include "zPlus_datas.h"
#include "zPlus_interact.h"
#include "zPlus_utils.h"
#include "zPlus_extra.h"





/* Contexte global. */


typedef struct {
	Datas* datas;
	unsigned mode;
} Context;

extern Context  gcontext;





/* Jeu. */


unsigned  askTry
  (PlayerDatas* player, unsigned min, unsigned max);

void  play
  (unsigned mode, unsigned lvl, PlayerDatas* playerG, PlayerDatas* playerA);





/* Fonctions de menu : génération, destruction et procédures. */


extern Menu  playerMenu;

extern Menu  statMenu;

extern Menu  lvlMenu;

extern Menu  mainMenu;



void  generateStatMenu
  (Menu* m);

void  destroyStatMenu
  (Menu* m);

void  createPlayerProc
  (unsigned foo);

void  deletePlayerProc
  (unsigned playerID);

void  switchPlayer1Proc
  (unsigned playerID);

void  switchPlayer2Proc
  (unsigned playerID);

void  playProc
  (unsigned lvl);



#endif    /* header not included */
