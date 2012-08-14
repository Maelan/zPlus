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
#include <SDL/SDL_ttf.h>
#include "zPlus_datas.h"
#include "zPlus_interact.h"
#include "zPlus_utils.h"





typedef struct {
	Datas* datas;
	unsigned mode/*, diff*/;
} Context;

extern Context  gcontext;



unsigned  askTry
  (PlayerDatas* player, unsigned min, unsigned max);


void  play
  (unsigned mode, unsigned diff, PlayerDatas* playerG, PlayerDatas* playerA);



#endif    /* header not included */
