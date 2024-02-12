#include <iostream>
#include "haromszogmatrix.h"
#include <fstream>
#include <iomanip>

using namespace std;

haromszogmatrix* letrehoz(int n, bool t)
{
	haromszogmatrix* A = new haromszogmatrix;
	unsigned long long int m = n;
	m = m * (m + 1) / 2;
	A->v = new double[m] {0};
	A->meret = n;
	A->tipus = t;
	return A;
}

void felszabadit(haromszogmatrix* A)
{
	delete[] A->v;
	delete A;
}

haromszogmatrix* beolvas_standard(int n, bool t)
{
	haromszogmatrix* A = letrehoz(n, t);

	for (int i = 0; i < A->meret; i++)
	{
		for (int j = 0; j < A->meret; j++)
		{
			double ertek;
			cin >> ertek;
			ertekadas(A, i, j, ertek);
		}
	}
	return A;
}

haromszogmatrix* beolvas_file(const char* f)
{
	int n;
	bool t;
	ifstream fin(f);
	fin >> n;
	fin >> t;
	haromszogmatrix* A = letrehoz(n, t);

	for (int i = 0; i < A->meret; i++)
	{
		for (int j = 0; j < A->meret; j++)
		{
			double ertek;
			fin >> ertek;
			ertekadas(A, i, j, ertek);
		}
	}
	return A;
}

double erteklekerdezes(haromszogmatrix* A, int sor, int oszlop)
{
	if (A->tipus == false)
	{
		if (sor < oszlop)
		{
			return 0;
		}
		return A->v[sor * (sor + 1) / 2 + oszlop];
	}
	if (oszlop < sor)
	{
		return 0;
	}
	return A->v[(sor * A->meret) - ((sor - 1) * sor / 2) + (oszlop + 1) - (sor + 1)];
}

void ertekadas(haromszogmatrix* A, int sor, int oszlop, double ertek)
{
	if (A->tipus == false)
	{
		if (oszlop <= sor)
		{
			A->v[sor * (sor + 1) / 2 + oszlop] = ertek;
		}
	}
	else
	{
		if (oszlop >= sor)
		{
			A->v[(sor * A->meret) - ((sor - 1) * sor / 2) + (oszlop + 1) - (sor + 1)] = ertek;
		}
	}
}


void kiir_standard(haromszogmatrix* A)
{
	for (int i = 0; i < A->meret; i++)
	{
		for (int j = 0; j < A->meret; j++)
		{
			cout << erteklekerdezes(A, i, j) << " ";
		}
		cout << endl;
	}
}

void kiir_file(const char* f, haromszogmatrix* A)
{
	ofstream fout(f, std::ofstream::app);
	for (int i = 0; i < A->meret; i++)
	{
		for (int j = 0; j < A->meret; j++)
		{
			fout << erteklekerdezes(A, i, j) << " ";
		}
		fout << endl;
	}
}

void beallit(haromszogmatrix* A, double ertek)
{

	for (int i = 0; i < A->meret; i++)
	{
		for (int j = 0; j < A->meret; j++)
		{
			ertekadas(A, i, j, ertek);
		}
	}
}

haromszogmatrix* tukrozott(haromszogmatrix* A)
{
	haromszogmatrix* At;
	At = letrehoz(A->meret, !A->tipus);
	if (A->tipus == true)
	{
		for (int i = 0; i < A->meret; i++)
		{
			for (int j = i; j < A->meret; j++)
			{
				ertekadas(At, j, i, erteklekerdezes(A, i, j));
			}
		}
	}
	else
	{
		for (int i = 0; i < A->meret; i++)
		{
			for (int j = 0; j <= i; j++)
			{
				ertekadas(At, j, i, erteklekerdezes(A, i, j));
			}
		}
	}
	return At;
}

haromszogmatrix* masol(haromszogmatrix* A)
{
	haromszogmatrix* B = letrehoz(A->meret, A->tipus);
	for (int i = 0; i < A->meret; i++)
	{
		for (int j = 0; j < A->meret; j++)
		{
			ertekadas(B, i, j, erteklekerdezes(A, i, j));
		}
	}
	return B;
}

int meret(haromszogmatrix* A)
{
	return A->meret;
}

bool tipus(haromszogmatrix* A)
{
	return A->tipus;
}