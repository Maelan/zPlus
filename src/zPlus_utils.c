#include "zPlus_utils.h"





unsigned  _log2
  (unsigned n)
{
	unsigned r;
	
	for(r = UINT_MAX;  n;  n /= 2)
		r++;
	
	return r;
}



unsigned  _log10
  (unsigned n)
{
	unsigned r;
	
	for(r = UINT_MAX;  n;  n /= 10)
		r++;
	
	return r;
	
	/*for(r = 0;  n >= 10;  n /= 10)
		r++;
	
	return (n == 1) ? r : UINT_MAX;*/
}



unsigned _pow
  (unsigned b, unsigned p)
{
	int acc;
	
	for(acc = 1;  p;  p /= 2) {
		if(p % 2)
			acc *= b;
		b *= b;
	}
	
	return acc;
}



unsigned  alea
  (unsigned max)
{
	return rand() % (max+1);
}
