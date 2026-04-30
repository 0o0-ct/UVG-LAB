/**
 *----------------------------------------
 * primeFork.cpp
 * ---------------------------------------
 * UNIVERSIDAD DEL VALLE DE GUATEMALA
 * CC3086 - Programación de microprocesadores
 * Fecha: 2026/01/25
 * References:
 * "Predicates" - The C++ Standard Library (Josuttis)
 * pp.226-227
 * "Shared Objects and Synchronization" - The Art of 
 * Multiprocessor Programming (Herlihy & Shavit)
 * pp.3-5
 * ---------------------------------------
 * Imprime los números primos en un rango de 10^n
 * Implementación simplista usando fork
 * ----------------------------------------
 */

#include <iostream> 
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cmath>

using namespace std;

/*
  Verificar si el número es primo usando divisiones sucesivas:
  Recibe: un número entero
  Devuelve: false 0 ,1 y nums no primos, true p/nums sí primos
*/
bool isPrime (int number)
{
	int divisor;
	if (number == 0 || number == 1)
	{	return false; }
	
	for (divisor = number/2; number%divisor != 0; --divisor)
	{	; }
	
	if (divisor != 1)
	{	return false; }

	else
	{	return true; }
}

/*
  Genera numeros primos de forma secuencial en un rango.
  Recibe: un numero inicial y un limite superior
  Imprime: los numeros primos encontrados entre ambos valores, 
  organizados en filas de 10 elementos
*/
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

/*
  Solicita un exponente y calcula un limite superior para buscar numeros primos:
  Recibe: un caracter numérico que representa el exponente x en 10^x
  Ejecuta: divide el rango de busqueda entre proceso padre e hijo usando fork()
*/
int main()
{
	int exponente = 2;
	char expChar;
	cout << "Ingrese el exponente x para calcular el limite superior 10^x." << endl;
    cout << "Ejemplo: si ingresa 3, se buscaran primos entre 0 y 1000 (10^3)." << endl;
	cout << "x = ";
	cin >> expChar;
	
	// Los números en ASCII  inician en el valor decimal 48
	exponente = int(expChar) - 48;
	
	long limit = pow(10,exponente);
	cout << endl;
	cout << "Se buscaran numeros primos en el rango [0, " <<limit<< "]." << endl;
	cout << "Iniciando calculo..." << endl;
	cout << endl;
	cout << "Los siguientes valores son números primos:" << endl;
	long i = 0;

	if(fork()==0)
	{
		// Asigna la primera pitad de la tarea al hijo
		cout << "\nNumeros primos calculados por el hijo: " << endl;
		primeTest(i,limit/2);
	}
	else
	{
		// Asigna la segunda mitad al padre
		cout << "\nNumeros primos calculados por el padre: " << endl;
		primeTest((limit/2)+1,limit);
	}
	
	return 0;
}