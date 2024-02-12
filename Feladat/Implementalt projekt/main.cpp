#include <iostream>
#include "haromszogmatrix.h"
#include <fstream>

using namespace std;

bool LU(double**, haromszogmatrix*, haromszogmatrix*, int); //felbont egy matrixot LU faktorizalassal

double determinans(haromszogmatrix*);							//kiszamitja a haromszogmatrix determinansat

double determinans(haromszogmatrix*, haromszogmatrix*);		//kiszamolja egy matrix determinansat az L es U matrix segitsegevel

void matrix_szoroz(double**, haromszogmatrix*, haromszogmatrix*); //osszeszoroz ket haromszogmatrixot

bool ellenoriz(int, double**, haromszogmatrix*, haromszogmatrix*);	//ellenorzi ha helyes az LU felbontas (a ket haromszogumatrix szorzata kiadja-e az eredeti matrixot)

void megold_egyenletrendszer(haromszogmatrix*, haromszogmatrix*, double*, int);	//megold egy egyenletrendszert

void kiir_beolvasott(int, double**, double*);		//kiiraj a beolvasott adatokat

void kiir_ismeretlenek(int, double*);						//kiirja az egyenletrendszer megoldasait

void beolvas(int&, double**, double*);				//beolvassa az adatokat

void hiba();										//hiba uzenet a hibas LU felbontas eseten

void megold_profitok(int, double**, double*);		//megoldja a feladatot (tartalmaz hibakezelest, LU febontast, egyenletrendszermegoldast) 

int main()
{
	ofstream fout("profitok.txt");
	double** a;
	double* e;
	int n;
	cout << "\033[0;32mHany tagja van a Pistike altal atalalt csoportnak? Valasz (3 - 100 kozotti egesz szam):\033[0;37m ";
	cin >> n;						//matrix meretenek a beolvasasa
	a = new double* [n];
	for (int i = 0; i < n; i++)
	{
		a[i] = new double[n];
	}
	e = new double[n];				//a beolvasando matrix es szabadtagok tomb meretenek a lefoglalasa 
	cout << endl;

	beolvas(n, a, e);				//adatok beolvasasa
	kiir_beolvasott(n, a, e);		//beolvasott adatok kiiratasa a fileba
	megold_profitok(n, a, e);		//feladat megoldasa

	return 0;
}

bool LU(double** matrix, haromszogmatrix* also, haromszogmatrix* felso, int n)
{
	/*
		LU faktrizalas pivot vallasztassal, ugy,
		hogy a pivotvallasztas soran kialakuljon a ket haromszogumantrix
	*/
	double** matrix_seged = new double* [n];
	for (int i = 0; i < n; i++)
	{
		matrix_seged[i] = new double[n];
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i == j)
			{
				ertekadas(also, i, j, 1);
			}
			ertekadas(felso, i, j, matrix[i][j]);
			matrix_seged[i][j] = matrix[i][j];
		}
	}

	int baj = false;

	for (int k = 0; k < n - 1; k++)
	{
		for (int i = k + 1; i < n; i++)
		{
			double v1 = matrix_seged[i][k];
			double v2 = matrix_seged[k][k];
			if (v2 == 0)
			{
				baj = true;
			}
			ertekadas(also, i, k, v1 / v2);
			for (int j = k + 1; j < n; j++)
			{
				v1 = matrix_seged[i][j];
				v2 = erteklekerdezes(also, i, k);
				double v3 = matrix_seged[k][j];
				matrix_seged[i][j] = v1 - v2 * v3;
				ertekadas(felso, i, j, matrix_seged[i][j]);
			}
			ertekadas(felso, i, k, 0);
			matrix_seged[i][k] = 0;
		}
	}
	for (int i = 0; i < n; i++)
	{
		delete[] matrix_seged[i];
	}
	delete[] matrix_seged;

	if (ellenoriz(n, matrix, also, felso) == false || baj == true)	//felbontas helyessegenek a vizsgalata
	{
		return false;
	}
	return true;
}

double determinans(haromszogmatrix* m)
{
	double det = 1;
	int n = meret(m);
	for (int i = 0; i < n; i++)
	{
		double v = erteklekerdezes(m, i, i);
		det *= v;
		if (det == 0)
		{
			return det;
		}
	}
	return det;
}

double determinans(haromszogmatrix* also, haromszogmatrix* felso)
{
	return determinans(also) * determinans(felso);
}

void matrix_szoroz(double** eredmeny, haromszogmatrix* also, haromszogmatrix* felso)
{
	int n = meret(also);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			eredmeny[i][j] = 0;

			for (int k = 0; k < n; k++)
			{
				double v1 = erteklekerdezes(also, i, k);
				double v2 = erteklekerdezes(felso, k, j);
				eredmeny[i][j] += v1 * v2;
			}
		}
	}
}

bool ellenoriz(int n, double** matrix, haromszogmatrix* also, haromszogmatrix* felso)
{
	double** eredmeny = new double* [n];
	for (int i = 0; i < n; i++)
	{
		eredmeny[i] = new double[n];
	}
	matrix_szoroz(eredmeny, also, felso);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (eredmeny[i][j] - matrix[i][j] > 0.1 && eredmeny[i][j] - matrix[i][j] < -0.1) // toleranciaval bir mivel a double szamokkal valo szamitas soran nem pontos
			{
				for (int i = 0; i < n; i++)
				{
					delete[] eredmeny[i];
				}
				delete[] eredmeny;
				return false;
			}
		}
	}
	return true;
}

void megold_egyenletrendszer(haromszogmatrix* also, haromszogmatrix* felso, double* e, int n)
{
	double* y = new double[n] {0};
	double* x = new double[n] {0};
	//megoldja az Ly = b elo helyettesitessel (forward substitution)
	for (int i = 0; i < n; i++)
	{
		double s = 0;
		for (int j = 0; j < i; j++)
		{
			double v = erteklekerdezes(also, i, j);
			s += v * y[j];
		}
		y[i] = e[i] - s;
	}

	//megoldja az Ux = y egyenletrendszert uto helyettesitessel (backward substitution)
	for (int i = n - 1; i >= 0; i--)
	{
		double s = 0;
		for (int j = i + 1; j < n; j++)
		{
			double v = erteklekerdezes(felso, i, j);
			s += v * x[j];
		}
		double v = erteklekerdezes(felso, i, i);
		x[i] = (y[i] - s) / v;
	}

	kiir_ismeretlenek(n, x);


	delete[] y;
	delete[] x;
}

void kiir_beolvasott(int n, double** a, double* e)
{
	ofstream fout("profitok.txt", std::ofstream::app);
	fout << endl << endl;
	fout << "A beolvasott adatok a kovetkezok: " << endl << endl;
	for (int i = 0; i < n; i++)
	{
		fout << "Az x darab bitcoin amit a(z) " << i + 1 << ". szemely altal adott arany-bevetel szerint lehet felirni: " << e[i] << endl;
		fout << "A(z) " << i + 1 << ". szemely arany-bevetelei: ";
		for (int j = 0; j < n; j++)
		{
			fout << a[i][j] << " ";
		}
		fout << endl << endl;
	}
	fout << endl << endl;
}

void kiir_ismeretlenek(int n, double* x)
{
	ofstream fout("profitok.txt", std::ofstream::app);
	fout << "A szemelyek nyeresege/vesztessege: " << endl;
	for (int i = 0; i < n; i++)
	{
		if (x[i] < 0)
		{
			fout << i + 1 << ". szemely: (vesztesseg)\t" << x[i] << " darab bitcoin" << endl;
		}
		else
		{
			if (x[i] > 0)
			{
				fout << i + 1 << ". szemely: (nyereseg)\t\t" << x[i] << " darab bitcoin " << endl;
			}
			else
			{
				fout << i + 1 << ". szemely: (semleges)\t" << x[i] << "darab bitcoin " << endl;
			}
		}
	}

	double max = x[0];
	for (int i = 1; i < n; i++)
	{
		if (x[i] > max)
		{
			max = x[i];
		}
	}
	cout << endl << endl;
	cout << "\033[0;32mAz(ok) a szemely(ek) akiktol Pistike kerhet befektetesi strategiakat: \033[0;33m" << endl;
	for (int i = 0; i < n; i++)
	{
		if (x[i] <= max + 0.001 && x[i] >= max - 0.001)
		{
			cout << i + 1 << ". szemely " << "(" << x[i] << " db bitcoin)\t\t";
			cout << endl;
		}
	}
	cout << endl << endl << "\033[0;37m";
}

void beolvas(int& n, double** a, double* e)
{

	cout << "\033[0;32mAdd meg a csoport tagjatol szerzett informaciokat.\033[0;37m" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "\033[0;32mA(z) \033[0;33m" << i + 1 << ". szemely \033[0;32maltal ismert profitok segitsegevel felirhato egyenlet: \033[0;37m";
		for (int j = 0; j < n; j++)
		{
			cin >> a[i][j];
		}
		cin >> e[i];
	}
	cout << endl;

}

void hiba()
{
	cout << "\033[0;31mA feladat az LU faktorizalast hasznalja az egyenletrendszer megoldasahoz," << endl;
	cout << "ezert nem minden esetben lehet az egyenlet matrixat (szabadtagok nelkul) felbonati. Ahoz, hogy" << endl;
	cout << "a matrixot lehessen felbontani LU faktorizalassal:" << endl;
	cout << "\t\t> a matrix determinansa nem szabad 0 legyen" << endl;
	cout << "\t\t> a matrix merete szigoruan nagyobb kell legyen mint ketto (n >= 3)." << endl;
	cout << "\t\t> a felbontas soran az L es U matrix szorzata ki kell adja az eredeti matrixot.\033[0;37m" << endl << endl;
}

void megold_profitok(int n, double** a, double* e)
{
	haromszogmatrix* also, * felso;
	also = letrehoz(n, false);
	felso = letrehoz(n, true);				//haromszogmatrixok lefoglalasa
	if (LU(a, also, felso, n) && n >= 3)	//ha a felbontas helyes es a meret nagyobb mint 2
	{
		double det = determinans(also, felso);	
		if (det != 0)						//ha a determinans nem 0 akkor helyes a felbontas lehet folytatni
		{
			megold_egyenletrendszer(also, felso, e, n);	//egyenletrendszer megoldasa
		}
		else
		{
			hiba();			//a determinans 0 hibauzenetek kiiratasa
		}
	}
	else
	{
		hiba();		//a determinans 0 hibauzenetek kiiratasa
	}
	felszabadit(also);
	felszabadit(felso);
}