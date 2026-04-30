/*---------------------------------------------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERÍA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACIÓN
*
* Curso: CC3086 Programación de Microprocesadores
* Actividad: Ejemplo Laboratorio 07
* Descripción: Llamada al sistema para creación de mútiples procesos hijos 
------------------------------------------------------------------------------*/
#include <iostream>
#include <sys/wait.h>
#include <sys/types.h> 
#include <unistd.h>
#include <cstdlib>

int main() 
{ 
    fork(); 
    fork(); 
    //fork(); 
    printf("Hello World! I´m process %d and my parent is %d.\n", getpid(), getppid()); 
    return EXIT_SUCCESS;
} 