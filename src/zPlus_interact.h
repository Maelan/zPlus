#ifndef INCLUDED_ZPLUS_INTERACT
#define INCLUDED_ZPLUS_INTERACT

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "zPlus_datas.h"

#ifdef  _WIN32
	#include <windows.h>
#endif





#define  MAX_MENU_ENTRIES  MAX_PLAYERS+1





void  setConsoleOutputEncoding
  (int set);


void  clearConsole
  (void);


void  clearInput
  (FILE* in);


unsigned  askNum
  (const wchar_t* msg);


unsigned  choose
  (unsigned min, unsigned max, const wchar_t* msg);





typedef  struct Menu  Menu;


/* Type de fonction utilisée pour générer un menu. */
typedef void (*MenuGenerator)(Menu*);

/* Type de fonction utilisée pour détruire un menu. */
typedef void (*MenuDestructor)(Menu*);

/* Type de fonction exécutée par une entrée de menu. */
typedef void (*MenuProc)(unsigned);

/* Menu. */
struct Menu {
	MenuGenerator gen;     /* (facultatif) fontion pour générer le menu */
	MenuDestructor des;    /* (facultatif) fontion pour détruire le menu */
	unsigned* pchoice;     /* (facultatif) adresse où écrire le choix */
	wchar_t* title;        /* le titre à afficher */
	unsigned start;        /* premier n° (si on veut démarrer à 1 ou à 0) */
	unsigned n;            /* le nombre d’entrées */
	/* chaque entrée du menu : */
	struct {
		unsigned extent;   /* plage supplémentaire de choix associés */
		wchar_t* label;    /* le texte affiché */
		Menu* sub;         /* l’action associée : un sous-menu …    */
		MenuProc proc;     /* … ou une fonction (si `sub` est NULL) */
	} entries[MAX_MENU_ENTRIES];
};


void  menu
  (Menu* menu);



#endif    /* header not included */
