/**
 *----------------------------------------
 * primeFork.cpp (Versión Mejorada - Ejercicio 5)
 * ---------------------------------------
 * UNIVERSIDAD DEL VALLE DE GUATEMALA
 * CC3086 - Programación de microprocesadores
 * ---------------------------------------
 * Imprime los números primos en un rango de 10^n
 * Implementación usando fork y wait
 * ----------------------------------------
 */

#include <iostream> 
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h>
#include <cstdlib>
#include <cmath>

using namespace std;

bool isPrime (int number)
{
	if (number <= 1) return false;
	
	if (number == 2) return true;
	
	if (number % 2 == 0) return false;
	
	for (int divisor = 3; divisor <= sqrt(number); divisor += 2)
	{
		if (number % divisor == 0)
		{
			return false; 
		}
	}
	
	return true; 
}

void primeTest(int thisNumber, int thisLimit)
{
	int posicion = 1;
	while (thisNumber < thisLimit)
	{
		thisNumber++;
		if(isPrime(thisNumber))
		{
			if(posicion < 10)
			{
				cout<<thisNumber<<" ";
				++posicion;
			}
			else
			{
				cout<<thisNumber<<endl;
				posicion=1;
			}
		}
	}
	cout<<endl;
}

int main()
{
	int exponente; 
	cout << "Ingrese el exponente x para calcular el limite superior 10^x." << endl;
    cout << "Ejemplo: si ingresa 3, se buscaran primos entre 0 y 1000 (10^3)." << endl;
	cout << "x = ";
	
	cin >> exponente;
	
	long limit = pow(10, exponente);
	cout << endl;
	cout << "Se buscaran numeros primos en el rango [0, " <<limit<< "]." << endl;
	cout << "Iniciando calculo..." << endl;
	cout << endl;
	
	pid_t pid = fork();

	if(pid == 0)
	{
		cout << "\nNumeros primos calculados por el hijo: " << endl;
		primeTest(0, limit/2);
		exit(0); 
	}
	else if (pid > 0)
	{
		
		cout << "\nNumeros primos calculados por el padre: " << endl;
		primeTest((limit/2)+1, limit);
		
		wait(NULL); 
	}
	else 
	{
		cerr << "Error al crear el proceso hijo." << endl;
		return 1;
	}
	
	return 0;
}
