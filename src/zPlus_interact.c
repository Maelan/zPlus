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



/*int  getNum
  (unsigned* nump)
{
	int r;
	wint_t c;
	
	r = wscanf(L"%u", nump);
	/* On débarrasse la table. :) * /
	while((c = fgetwc(stdin)) != L'\n'  &&  c != WEOF);
	
	return r == 1;
}



unsigned  askNum
  (const wchar_t* msg)
{
	unsigned num;
	int r;
	
	do {
		if(msg)
			fputws(msg, stdout);
		r = getNum(&num);
	} while(!r);
	
	return num;
}*/



unsigned  askNum
  (const wchar_t* msg)
{
	unsigned num;
	int r;
	wint_t c;
	
	do {
		if(msg)
			fputws(msg, stdout);
		r = wscanf(L"%u", &num);
		/* On débarrasse la table. :) */
		while((c = fgetwc(stdin)) != L'\n'  &&  c != WEOF);
	} while(r != 1);
	
	return num;
}



unsigned  choose
  (unsigned min, unsigned max, const wchar_t* msg)
{
	unsigned choice;
	
	if(!msg)
		msg = L"Votre choix : ";
	
	do
		choice = askNum(msg);
	while(choice < min   ||  choice > max);
	
	return choice;
}



unsigned  menu
  (const Menu* m)
{
	unsigned choice, max;
	
	
	fputws(m->title, stdout);
	putwchar(L'\n');
	
	max = m->start;
	for(unsigned i = 0;  i < m->n;  i++) {
		wprintf(L"  %u%.0i. %ls\n",
		  max,
		  (m->entries[i].extent) ? -(max + m->entries[i].extent) : 0,
		  m->entries[i].label
		 );
		max += 1 + m->entries[i].extent;
	}
	
	choice = choose(m->start, max-1, NULL);
	putwchar(L'\n');
	
	if(m->entries[choice].sub)
		menu(m->entries[choice].sub);
	else if(m->entries[choice].proc)
		m->entries[choice].proc(choice);
	
	return choice;
}
