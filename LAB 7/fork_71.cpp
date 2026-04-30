/*--------------------------------------------------------------------------- 
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERIA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACION
*
* Curso: CC3086 Programacion de Microprocesadores
* Actividad: Ejemplo Laboratorio 07
* Descripcion: Llamada al sistema para creacion de procesos hijos
------------------------------------------------------------------------------*/
#include <iostream>
#include <sys/wait.h>  // Solo disponible en UNIX
#include <unistd.h>    // Solo disponible en UNIX
#include <cstdlib>     // Para EXIT_SUCCESS y EXIT_FAILURE

using namespace std;  // Importa TODO el espacio de nombres

int main() {
    // Crea la variable a, solicita ingreso de su valor por teclado
    int a;
    cout << "Please enter the value to be stored in a: ";
    cin >> a;
    cout << "\n";

    pid_t res;
    res = fork();

    // Si el hijo no se crea correctamente, ERROR
    if (res < 0) {
        perror("fork not created");
        exit(EXIT_FAILURE);
    }
    // Si el proceso actual es el padre, el OS detecta un PID válido
    else if (res > 0) {
        cout << "Printed from Parent Num. Process: " << getpid() << endl;
        a = a + 5;
        cout << "a printed from Parent: " << a << "\n" << endl;
        wait(nullptr);
    }
    // Si el proceso actual es Child, el OS no detecta un ID válido,
    // localmente ID = 0, implica que ha sido creado desde otro proceso
    else { // == 0
        cout << "Printed from Child Num. Process: " << getpid() << endl;

        a = a + 10;
        cout << "a printed from Child: " << a << "\n" << endl;
        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}