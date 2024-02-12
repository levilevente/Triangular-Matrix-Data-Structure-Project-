#ifndef HMATRIX
#define HMATRIX

#include <iostream>

using namespace std;

typedef struct haromszogmatrix {
	double* v;
	int meret;
	bool tipus;
}haromszogumatrix;


//letrehozas
haromszogmatrix* letrehoz(int, bool);//letrehozza az n meretu es t tipuso haromszogmatrixot								



//felszabaditas
void felszabadit(haromszogmatrix*);					//felszabaditsa a lefoglalt haromszogmatrixot	



//beolvaso fugvenyek
haromszogmatrix* beolvas_standard(int, bool);			//beolvassa a standard bemenetrol egy haromszogmatrixot (elore lefoglalt)	
haromszogmatrix* beolvas_file(const char*);				//beolvassa egy megadott filebol a megadott haromszogmatrix meretet tipusat es a haromszogmatrixot



//ertekkezelo fugvenyek
double erteklekerdezes(haromszogmatrix*, int, int);	//visszateriti az (i, j) pozicion levo erteket
void ertekadas(haromszogmatrix*, int, int, double);	//folulirja az (i, j) pozicion levo erteket a megadott szammal



//kiirato fugvenyek
void kiir_standard(haromszogmatrix*);				//kiirja a standard bemenetre a megadott haromszogmatrixot
void kiir_file(const char*, haromszogmatrix*);		//kiirja egy megadott fileba a amegadott haromszogmatrixot



//atalakito fugvenyek
haromszogmatrix* tukrozott(haromszogmatrix*);						//visszateriti egy adott haromszogmatrix transponaltjat
void beallit(haromszogmatrix*, double);							//beallitja a haromszogumatrix elemeit egy adott ertekkel
haromszogmatrix* masol(haromszogmatrix*);							//visszaterit a parameterkent megadott haromszogumatrixrol egy masolatot	


//lekerdezo fugvenyek
int meret(haromszogmatrix*);	//visszateriti egy adott haromszogmatrix meretet 
bool tipus(haromszogmatrix*);	//visszateriti a haromszogmatrix tipusat
#endif 