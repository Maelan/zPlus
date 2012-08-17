#include "zPlus_main.h"
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_ttf.h>





/* Contexte global. */

Context  gcontext;





/* Jeu. */

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
		/* Parce qu’un type qui répond instantanément, c’est de la concurrence
		   déloyale. */
		fflush(stdout);    /* pour afficher le prompt sans attendre le nombre */
		SDL_Delay(100 * (rand()%13 + 5));
		/* Affichage pour simuler la saisie à l’écran. */
		wprintf(L"%u\n", try);
		return try;
	}
}



void  play
  (unsigned mode, unsigned lvl, PlayerDatas* playerG, PlayerDatas* playerA)
{
	const wchar_t* diffMsg[10] = {
		[0]= L"Vous ne voulez pas plutôt jouer au morpion ?",
		[1]= L"À petit joueur, petits nombres.",
		[2]= L"Bonne partie !",
		[3]= L"Vous prenez des risques, dites-donc.",
		[4]= L"Vous en voulez encore ?!",
		[5]= L"Puisse le hasard être avec vous.",
		[6]= L"Je vous offre le café.",
		[7]= L"Vous vous ennuyiez ?",
		[8]= L"Quand je pense que certains travaillent…",
		[9]= L"À l’année prochaine !"
	};
	
	unsigned max, ans, try;
	unsigned knownMin, knownMax;
	unsigned tries, optimaxTries;
	Uint32 chrono;
	
	/* Tirage des paramètres. */
	max = ( lvl == 0 )
	  ? choose(1, 1000000000, L"Maximum : ")
	  : _pow(10, lvl);
	
	ans = ( mode == 2 )
	  ? choose(0, max, L"Nombre mystère : ")
	  : alea(max);
	
	wprintf( L"Le nombre mystère est entre 0 et %u. %ls\n",
	  max, diffMsg[_log10(max)] );
	
	knownMin = 0;
	knownMax = max;
	tries = 0;
	chrono = SDL_GetTicks();
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
	chrono = SDL_GetTicks() - chrono;
	
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
	  chrono / 1000.,  chrono / 1000. / tries
	 );
	
	/* Enregistrement et affichage des stats pour le niveau. */
	if(playerG) {
		playerG->levels[0].played ++;
		if(lvl) {
			updateLevelStats(playerG, lvl, tries, chrono);
			printLevelStats(playerG, lvl);
		}
	}
}





/* Fonctions de menu : génération, destruction et procédures. */



Menu playerListMenu = {
	generateStatMenu, destroyStatMenu, NULL,
	L"Liste des joueurs :",
	0, .n = 0
};


Menu playerMenu = {
	NULL, NULL, &gcontext.mode,
	L"Options des joueurs :",
	1, 5,
	{ { 0, L"Créer un profil",              NULL,            createPlayerProc },
	  { 0, L"Changer de profil",            &playerListMenu, NULL             },
	  { 0, L"Changer de profil (joueur 2)", &playerListMenu, NULL             },
	  { 0, L"Supprimer un profil",          &playerListMenu, NULL             },
	  { 0, L"Retour au menu principal.",    NULL,            NULL             } }
};


Menu statMenu = {
	generateStatMenu, destroyStatMenu, NULL,
	L"Statistiques des joueurs :",
	0, .n = 0
};


Menu lvlMenu = {
	NULL, NULL, NULL,
	L"Difficulté :",
	0, 6,
	{ { 0, L"personnalisée            (0 - n)",    NULL, playProc },
	  { 0, L"facile                   (0 - 10)",   NULL, playProc },
	  { 0, L"moyen                    (0 - 100)",  NULL, playProc },
	  { 0, L"difficile                (0 - 1000)", NULL, playProc },
	  { 5, L"de plus en plus fou !  (0 - 10^n)",   NULL, playProc },
	  { 0, L"Retour au menu principal.",           NULL, NULL     } }
};


Menu mainMenu = {
	NULL, NULL, &gcontext.mode,
	L"Menu principal & mode de jeu :",
	0, 6,
	{ { 0, L"IA         (programme / programme)", &lvlMenu,    NULL },
	  { 0, L"solo       (programme / joueur)",    &lvlMenu,    NULL },
	  { 0, L"2 joueurs     (joueur / joueur)",    &lvlMenu,    NULL },
	  { 0, L"Statistiques.",                      &statMenu,   NULL },
	  { 0, L"Joueurs.",                           &playerMenu, NULL },
	  { 0, L"Quitter le jeu.",                    NULL,        NULL } }
};



void  playProc
  (unsigned lvl)
{
	/* Joueur qui doit deviner (« Guesser »). */
	PlayerDatas* playerG = ( gcontext.mode != 0 )
	  ? gcontext.datas->players + gcontext.datas->player1
	  : NULL;
	/* Joueur qui détient la réponse (« Answerer »). */
	PlayerDatas* playerA = ( gcontext.mode == 2 )
	  ? gcontext.datas->players + gcontext.datas->player2
	  : NULL;
	
	play(gcontext.mode, lvl, playerG, playerA);
}



void  createPlayerProc
  (unsigned foo)
{
	wchar_t buf[MAX_PLAYER_NAME+1];
	wchar_t* p;
	
	wprintf(L"Nom (max. %u caractères) : ", MAX_PLAYER_NAME);
	fgetws(buf, MAX_PLAYER_NAME+1, stdin);
	if( (p = wcschr(buf, L'\n')) )
		*p = L'\0';
	else
		clearInput(stdin);
	
	if(!*buf) {
		fputws(L"Opération annulée.\n\n", stderr);
		return;
	}
	
	addPlayer(&gcontext.datas, buf);
	switchPlayer1Proc(gcontext.datas->nPlayers - 1);
}



void  deletePlayerProc
  (unsigned playerID)
{
	deletePlayer(&gcontext.datas, playerID);
}



void  switchPlayer1Proc
  (unsigned playerID)
{
	gcontext.datas->player1 = playerID;
	wprintf( L"Bonjour %ls ! Vous êtes maintenant le joueur 1. :)\n",
	  gcontext.datas->players[playerID].name );
}



void  switchPlayer2Proc
  (unsigned playerID)
{
	gcontext.datas->player2 = playerID;
	wprintf( L"Bonjour %ls ! Vous êtes maintenant le joueur 2. d8)\n",
	  gcontext.datas->players[playerID].name );
}



void  printPlayerStatsProc
  (unsigned playerID)
{
	printPlayerStats(gcontext.datas->players + playerID);
}



#define  MAX_STATMENU_LABEL_SIZE  (5 + MAX_PLAYER_NAME + 4 + 22)



void  generateStatMenu
  (Menu* m)
{ 
	unsigned i;
	
	for(i = 0;  i < gcontext.datas->nPlayers;  i++) {
		if(!m->entries[i].label)
			m->entries[i].label =
			  malloc((MAX_STATMENU_LABEL_SIZE+1) * sizeof(wchar_t));
		if(!m->entries[i].label) {
			fputws(L"Erreur mémoire : ", stderr);
			perror(NULL);
			exit(EXIT_FAILURE);
		}
		
		swprintf( m->entries[i].label, MAX_STATMENU_LABEL_SIZE+1,
		  L"[%lc]  %-*ls    parties jouées : %u",
		  (i == gcontext.datas->player1)
		    ?  L'1'
		    : (i == gcontext.datas->player2)
		      ?  L'2'
		      :  L' ',
		  MAX_PLAYER_NAME,
		  gcontext.datas->players[i].name,
		  gcontext.datas->players[i].levels[0].played
		 );
		
		m->entries[i].sub = NULL;
		if(m == &statMenu)
			m->entries[i].proc = printPlayerStatsProc;
		else if(gcontext.mode == 2)
			m->entries[i].proc = switchPlayer1Proc;
		else if(gcontext.mode == 3)
			m->entries[i].proc = switchPlayer2Proc;
		else /* gcontext.mode == 4 */
			m->entries[i].proc = deletePlayerProc;
	}
	
	m->entries[i].label = L"Retour au menu précédent.";
	m->entries[i].sub =   NULL;
	m->entries[i].proc =  NULL;
	
	m->n = gcontext.datas->nPlayers + 1;
}



void  destroyStatMenu
  (Menu* m)
{
	m->entries[--m->n].label = NULL;
	
	while(m->n--) {
		free(m->entries[m->n].label);
		m->entries[m->n].label = NULL;
	}
}





/* Main. */



int  main
  (int ac, char** av)
{
	SDL_Thread* extraTh;
	
	/* Initialisations absconses. */
	srand(time(NULL));
	setlocale(LC_ALL, "");
	setConsoleOutputEncoding(1);
	SDL_Init(/*SDL_INIT_VIDEO |*/ SDL_INIT_EVENTTHREAD);
	//TTF_Init();
	SDL_EnableUNICODE(1);
	
	extraTh = SDL_CreateThread(extra, NULL);
	if(!extraTh) {
		fwprintf(stderr,
		  L"Erreur à la création du thread bonus."
		  L" La SDL nous dit (merci la SDL) : %s",
		  SDL_GetError()
		 );
	}
	
	/* Chargement du fichier de données. */
	gcontext.datas = loadDatas();
	
	SDL_WaitThread(extraTh, NULL);
	
	/* Menu principal → déroulement du programme. */
	menu(&mainMenu);			
	
	/* Fin du programme. */
	setConsoleOutputEncoding(0);
	//TTF_Quit();
	SDL_Quit();
	unloadDatas(gcontext.datas);
	
	return EXIT_SUCCESS;
}
