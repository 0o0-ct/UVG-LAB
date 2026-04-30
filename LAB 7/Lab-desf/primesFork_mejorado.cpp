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
#include <sys/wait.h> // Necesario para wait()
#include <unistd.h>
#include <cstdlib>
#include <cmath>

using namespace std;

/*
  Verificar si el número es primo:
  - Maneja números negativos y el 0,1 devolviendo false.
  - Es mucho más eficiente porque solo busca divisores hasta la raíz cuadrada del número.
*/
bool isPrime (int number)
{
	// Descartar números negativos, 0 y 1 inmediatamente
	if (number <= 1) return false;
	
	// El 2 es el único número par que es primo
	if (number == 2) return true;
	
	// Descartar cualquier otro número par inmediatamente
	if (number % 2 == 0) return false;
	
	// Buscar divisores solo hasta la raíz cuadrada del número, y saltando de 2 en 2
	for (int divisor = 3; divisor <= sqrt(number); divisor += 2)
	{
		if (number % divisor == 0)
		{
			return false; // Si encuentra un divisor, no es primo
		}
	}
	
	return true; // Si no encontró divisores, sí es primo
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

int main()
{
	int exponente; // Ahora es int en lugar de char
	
	cout << "Ingrese el exponente x para calcular el limite superior 10^x." << endl;
    cout << "Ejemplo: si ingresa 3, se buscaran primos entre 0 y 1000 (10^3)." << endl;
	cout << "x = ";
	
	// Leemos directamente como un número entero. 
	// Esto hace que el programa sea más robusto porque permite ingresar exponentes de más de 1 dígito (ej. 10),
	// cosa que con un "char" era imposible porque un char solo guarda un símbolo a la vez.
	cin >> exponente;
	
	long limit = pow(10, exponente);
	cout << endl;
	cout << "Se buscaran numeros primos en el rango [0, " <<limit<< "]." << endl;
	cout << "Iniciando calculo..." << endl;
	cout << endl;
	
	pid_t pid = fork();

	if(pid == 0)
	{
		// Proceso Hijo: Asigna la primera mitad de la tarea
		cout << "\nNumeros primos calculados por el hijo: " << endl;
		primeTest(0, limit/2);
		exit(0); // Buena práctica: asegurar que el hijo termine explícitamente aquí
	}
	else if (pid > 0)
	{
		// Proceso Padre: Asigna la segunda mitad
		cout << "\nNumeros primos calculados por el padre: " << endl;
		primeTest((limit/2)+1, limit);
		
		// El padre debe esperar al hijo para evitar que quede como proceso zombie
		wait(NULL); 
	}
	else 
	{
		cerr << "Error al crear el proceso hijo." << endl;
		return 1;
	}
	
	return 0;
}
