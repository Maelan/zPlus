#include "zPlus_main.h"
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_ttf.h>





Context  gcontext;



unsigned  askTry
  (PlayerDatas* player, unsigned min, unsigned max)
{
	wprintf(L"[%ls] ",  (player) ? player->name : IA_NAME);
	
	/* Demande au joueur. */
	if(player)
		return askNum(L"Tentez un nombre : ");
	
	/* Intelligence artificielle : dichotomie. */
	else {
		unsigned try = (min+max) / 2;
		/* Un peu de sel, histoire de moins s’emmerder : écart aléatoire d’au
		   plus 1/5 de l’intervalle. */
		try += (int)( rand() % (max-min+1) - (max-min)/2 ) / 5;
		/* Affichage pour simuler la saisie à l’écran. */
		wprintf(L"%u\n", try);
		return try;
	}
}



void  play
  (unsigned mode, unsigned diff, PlayerDatas* playerG, PlayerDatas* playerA)
{
	unsigned max, ans, try;
	unsigned knownMin, knownMax;
	unsigned tries, optimaxTries;
	clock_t chrono;
	
	/* Tirage des paramètres. */
	max = ( diff == 0 )
	  ? askNum(L"Maximum : ")
	  : _pow(10, diff);
	
	ans = ( mode == 2 )
	  ? choose(0, max, L"Nombre mystère : ")
	  : alea(max);
	
	knownMin = 0;
	knownMax = max;
	tries = 0;
	chrono = clock();
	do {
		tries++;
		/*wprintf(L"Intervalle : [[%u ; %u]].\n",
		  knownMin, knownMax);*/
		try = askTry(playerG, knownMin, knownMax);
		wprintf(
		  L"  -> %u : c'est %ls.\n",
		  try,
		  (try < ans)
			? L"plus"
			: (try > ans)
			  ? L"moins"
			  : L"ça"
		 );
		if(try < ans  &&  knownMin < try)
			knownMin = try;
		if(ans < try  &&  try < knownMax)
			knownMax = try;
	} while(try != ans);
	chrono = clock() - chrono;
	
	optimaxTries = _log2(max) + 1;
	
	/* Affichage des résultats. */
	wprintf(
	  L"La réponse était %u.\n"
	  L"    coups : %u  (pas plus de %u si vous êtes malin)\n"
	  L"    score : %+i\n"
	  L"    temps : %.2fs  (%.2fs en moyenne par coup).\n\n",
	  ans,
	  tries,
	  optimaxTries, optimaxTries - tries,
	  (double)chrono / CLOCKS_PER_SEC, (double)chrono / CLOCKS_PER_SEC / tries
	 );
	
	/* Enregistrement des résultats. */
	if(playerG) {
		playerG->levels[0].played ++;
		if(diff) {
			playerG->levels[diff].played     ++;
			playerG->levels[diff].totalTries += tries;
			if( tries < playerG->levels[diff].minTries
				|| !playerG->levels[diff].minTries )
				playerG->levels[diff].minTries = tries;
		}
		wprintf(
		  L"[%ls]  Statistiques pour le niveau %u :\n"
		  L"    parties jouées : %u\n"
		  L"    score moyen :    %+.2f\n"
		  L"    meilleur score : %+i\n\n",
		  playerG->name, diff,
		  playerG->levels[diff].played,
		  optimaxTries
		    - playerG->levels[diff].totalTries
		      / (double)playerG->levels[diff].played,
		  optimaxTries - playerG->levels[diff].minTries
		 );
	}
}





/* Main. */



void  playMenuProc
  (unsigned diff)
{
	/* Joueur qui doit deviner (« Guesser »). */
	PlayerDatas* playerG = ( gcontext.mode != 0 )
	  ? gcontext.datas->players + gcontext.datas->player1
	  : NULL;
	/* Joueur qui détient la réponse (« Answerer »). */
	PlayerDatas* playerA = ( gcontext.mode == 2 )
	  ? gcontext.datas->players + gcontext.datas->player2
	  : NULL;
	
	play(gcontext.mode, diff, playerG, playerA);
}



int  main
  (int ac, char** av)
{
	const Menu diffMenu = {
		L"Difficulté :",
		6, 0, NULL,
		{ { 0, L"personnalisé                    (0 - n)",    NULL, playMenuProc },
		  { 0, L"facile                          (0 - 10)",   NULL, playMenuProc },
		  { 0, L"moyen                           (0 - 100)",  NULL, playMenuProc },
		  { 0, L"difficile                       (0 - 1000)", NULL, playMenuProc },
		  { 5, L"de plus en plus diabolique !  (0 - 10^n)",   NULL, playMenuProc },
		  { 0, L"Retour au choix du mode de jeu.",            NULL, NULL } }
	};
	
	const Menu modeMenu = {
		L"Modes de jeu :",
		4, 0, &gcontext.mode,
		{ { 0, L"IA         (programme / programme)", &diffMenu, NULL },
		  { 0, L"solo       (programme / joueur)",    &diffMenu, NULL },
		  { 0, L"2 joueurs     (joueur / joueur)",    &diffMenu, NULL },
		  { 0, L"Retour au menu principal.",          NULL,      NULL } }
	};
	
	const Menu mainMenu = {
		L"Menu principal :",
		4, 1, NULL,
		{ { 0, L"Jouer.",          &modeMenu, NULL },
		  { 0, L"Statistiques.",   NULL,      NULL },
		  { 0, L"Joueurs.",        NULL,      NULL },
		  { 0, L"Quitter le jeu.", NULL,      NULL } }
	};
	
	/* Initialisations absconses. */
	srand(time(NULL));
	setlocale(LC_ALL, "");
	//setConsoleOutputEncoding(1);
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTTHREAD);
	TTF_Init();
	SDL_EnableUNICODE(1);
	
	/* Chargement du fichier de données. */
	gcontext.datas = loadDatas();
	
	/* Menu principal. */
	menu(&mainMenu);			
	
	/* Fin du programme. */
	//setConsoleOutputEncoding(0);
	TTF_Quit();
	SDL_Quit();
	unloadDatas(gcontext.datas);
	
	return EXIT_SUCCESS;
}
