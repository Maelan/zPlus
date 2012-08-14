#ifndef INCLUDED_ZPLUS_INTERACT
#define INCLUDED_ZPLUS_INTERACT

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#ifdef  _WIN32
	#include <windows.h>
#endif





#define  MENU_MAX_ENTRIES  10





/* Type de fonction exécutée par une entrée de menu. */
typedef void (*MenuProc)(unsigned);

/* Menu. */
typedef struct Menu {
	wchar_t* title;    /* le titre à afficher */
	unsigned n;        /* le nombre d’entrées */
	unsigned start;    /* premier n° (si on veut démarrer à 1 ou à 0) */
	unsigned* pchoice; /* (facultatif) adresse où écrire le choix */
	/* chaque entrée du menu : */
	struct {
		unsigned extent;           /* nombre de choix supplémentaires associés */
		wchar_t* label;            /* le texte affiché */
		const struct Menu* sub;    /* l’action associée : un sous-menu …    */
		MenuProc proc;             /* … ou une fonction (si `sub` est NULL) */
	} entries[MENU_MAX_ENTRIES];
} Menu;





void  setConsoleOutputEncoding
  (int set);

void  clearConsole
  (void);

unsigned  askNum
  (const wchar_t* msg);

unsigned  choose
  (unsigned min, unsigned max, const wchar_t* msg);

void  menu
  (const Menu* menu);



#endif    /* header not included */
