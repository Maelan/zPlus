#include "zPlus_extra.h"

static wchar_t  asciiArt[] =
	L"             _____   _____    \n"
	L"            /\\    \\ /\\    \\   \n"
	L"           /::\\    V::\\____\\  \n"
	L"           \\:::\\  /:::/    /  \n"
	L"            \\:::\\/:::/    /    _____  _                v 4.7.1.2\n"
	L"             \\::::::/    /    |  __ \\| |                        \n"
	L"              \\::::/____/     | |__) | |_   _ ___    ___  _   _ \n"
	L"      _____   /::::\\    \\     |  ___/| | | | / __|  / _ \\| | | |\n"
	L"     /\\    \\ /::::::\\    \\    | |    | | |_| \\__ \\ | (_) | |_| |\n"
	L"    /::\\    V:::/\\:::\\    \\   |_|    |_|\\__,_|___/  \\___/ \\__,_|\n"
	L"    \\:::\\  /:::/  \\:::\\____\\   __  __       _             ___   \n"
	L"     \\:::\\ \\::/    \\::/    /  |  \\/  |     (_)           |__ \\  \n"
	L"      \\:::\\ \\/____/ \\/____/   | \\  / | ___  _ _ __  ___     ) | \n"
	L"       \\:::\\    \\             | |\\/| |/ _ \\| | '_ \\/ __|   / /  \n"
	L"        \\:::\\    \\            | |  | | (_) | | | | \\__ \\  |_|   \n"
	L"         \\:::\\    \\           |_|  |_|\\___/|_|_| |_|___/  (_)   \n"
	L"          \\:::\\    \\          \n"
	L"           \\:::\\____\\         \n"
	L"            \\::/    /         \n"
	L"             \\/____/                       by Maëlan with many ♥\n"
/*	L"             _____   _____    \n"
	L"            /\    \ /\    \   \n"
	L"           /::\    V::\____\  \n"
	L"           \:::\  /:::/    /  \n"
	L"            \:::\/:::/    /    _____  _                v 4.7.1.2\n"
	L"             \::::::/    /    |  __ \| |                        \n"
	L"              \::::/____/     | |__) | |_   _ ___    ___  _   _ \n"
	L"      _____   /::::\    \     |  ___/| | | | / __|  / _ \| | | |\n"
	L"     /\    \ /::::::\    \    | |    | | |_| \__ \ | (_) | |_| |\n"
	L"    /::\    V:::/\:::\    \   |_|    |_|\__,_|___/  \___/ \__,_|\n"
	L"    \:::\  /:::/  \:::\____\   __  __       _             ___   \n"
	L"     \:::\ \::/    \::/    /  |  \/  |     (_)           |__ \  \n"
	L"      \:::\ \/____/ \/____/   | \  / | ___  _ _ __  ___     ) | \n"
	L"       \:::\    \             | |\/| |/ _ \| | '_ \/ __|   / /  \n"
	L"        \:::\    \            | |  | | (_) | | | | \__ \  |_|   \n"
	L"         \:::\    \           |_|  |_|\___/|_|_| |_|___/  (_)   \n"
	L"          \:::\    \          \n"
	L"           \:::\____\         \n"
	L"            \::/    /         \n"
	L"             \/____/                       by Maëlan with many ♥\n"*/
;



void  loadModule
  (const wchar_t* label)
{
	size_t len = wcslen(label),
	       ndots = 80 - len - 8;
	
	wprintf(L"  %ls ", label);
	
	for(size_t i;  ndots;  ndots -= i) {
		fflush(stdout);
		SDL_Delay(70);
		i = rand()%3;
		if(i > ndots)
			i = ndots;
		for(size_t j = i;  j;  j--)
			fputwc(L'.', stdout);
	}
	fputws(L" 100%\n", stdout);
}



int  extra
  (void* foo)
{
	const wchar_t* labels[] = {
		L"Chargement du système de jeu",
		L"Chargement de l’interface",
		L"Chargement des modules",
		L"Configuration des préférences",
		L"Initialisation des paramètres globaux"
	};
	
	/* Mouhaha. */
	clearConsole();
	
	for(size_t i = 0;  i < sizeof(labels)/sizeof(*labels);  i++)
		loadModule(labels[i]);
	
	SDL_Delay(1500);
	fputws(asciiArt, stdout);
	fputwc(L'\n', stdout);
	SDL_Delay(1000);
	
	return 0xf00;
}
