#include "zPlus_interact.h"





void  setConsoleOutputEncoding
  (int set)
{
 #ifdef _WIN32
	
	static UINT oldCP;
	
	if(set) {
		oldCP = GetConsoleOutputCP();
		SetConsoleOutputCP(GetACP());
	} else
		SetConsoleOutputCP(oldCP);
	
 #else
	
	return;
	
 #endif
}



void  clearConsole
  (void)
{
	system(
 #ifdef _WIN32
	  "cls"
 #else
	  "clear"
 #endif
	);
}



void  clearInput
  (FILE* in)
{
	wint_t c;
	while((c = fgetwc(in)) != L'\n'  &&  c != WEOF);
}



unsigned  askNum
  (const wchar_t* msg)
{
	unsigned num;
	int r;
	
	if(!msg)
		msg = L"Votre choix : ";
	
	do {
		fputws(msg, stdout);
		msg = L"> ";
		r = wscanf(L"%u", &num);
		/* On débarrasse la table. :) */
		clearInput(stdin);
	} while(r != 1);
	
	return num;
}



unsigned  choose
  (unsigned min, unsigned max, const wchar_t* msg)
{
	unsigned choice;
	
	do
		choice = askNum(msg);
	while((choice < min  ||  choice > max)  &&  choice != UINT_MAX);
	
	return choice;
}



void  menu
  (Menu* m)
{
	while(0xfeel) {
		unsigned choice, max;
		
		/* Génération du menu. */
		if(m->gen)
			m->gen(m);
		
		putwchar(L'\n');
		
		/* Affichage du titre. */
		fputws(m->title, stdout);
		putwchar(L'\n');
		
		/* Affichage des entrées. */
		max = m->start;
		for(unsigned i = 0;  i < m->n;  i++) {
			wprintf(L"  %2u%.i. %ls\n",
			  max,
			  (m->entries[i].extent) ? -(max + m->entries[i].extent) : 0,
			  m->entries[i].label
			 );
			max += 1 + m->entries[i].extent;
		}
		
		/* Choix de l’utilisateur. */
		choice = choose(m->start, max-1, NULL);
		//putwchar(L'\n');
		if(m->pchoice)
			*m->pchoice = choice;
		
		/* Mouhahahahahahaha… Hurlez ! */
		clearConsole();
		
		unsigned i = 0;
		for(unsigned sum = m->start;  sum < choice;  sum += 1 + m->entries[i].extent)
			i++;
		
		/* Action associée (sous-menu, fonction, ou quitter sinon). */
		if(m->entries[i].sub)
			menu(m->entries[i].sub);
		else if(m->entries[i].proc)
			m->entries[i].proc(choice);
		else
			break;
	}
		
	/* Destruction du menu. */
	if(m->des)
		m->des(m);
}
