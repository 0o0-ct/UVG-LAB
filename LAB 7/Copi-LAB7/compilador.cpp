#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>
#include <chrono>
#include <cmath>
#include <cstdlib>

using namespace std;

void decode_and_execute(const string& instr, int instr_num, bool is_parent) {
    // Iniciar cronometro
    auto start = chrono::high_resolution_clock::now();
    
    string proc_type = is_parent ? "padre" : "hijo";
    pid_t pid = getpid();
    
    cout << "Proceso " << proc_type << " ejecutando instruccion " << instr_num << ", PID: " << pid << "\n";
    cout << "Instruccion " << instr_num << ": " << instr << "\n";
    
    bool valid = true;
    if (instr.length() != 7) valid = false;
    for (char c : instr) {
        if (c != '0' && c != '1') valid = false;
    }
    
    if (!valid) {
        cout << "Resultado: Error. Instruccion invalida (formato incorrecto)\n";
    } else {
        string opcode_str = instr.substr(0, 3);
        string a_str = instr.substr(3, 2);
        string b_str = instr.substr(5, 2);
        
        int opcode = stoi(opcode_str, nullptr, 2);
        int a = stoi(a_str, nullptr, 2);
        int b = stoi(b_str, nullptr, 2);
        
        string op_name;
        bool error = false;
        string error_msg = "";
        int resultado = 0;
        
        switch (opcode) {
            case 0: op_name = "Suma"; resultado = a + b; break;
            case 1: op_name = "Resta"; resultado = a - b; break;
            case 2: op_name = "Multiplicacion"; resultado = a * b; break;
            case 3: 
                op_name = "Division"; 
                if (b == 0) { error = true; error_msg = "Error. Division por cero"; }
                else { resultado = a / b; }
                break;
            case 5: op_name = "Potencia"; resultado = (int)pow(a, b); break;
            case 6: 
                op_name = "Modulo"; 
                if (b == 0) { error = true; error_msg = "Error. Modulo por cero"; }
                else { resultado = a % b; }
                break;
            default: 
                op_name = "Desconocido"; 
                error = true; error_msg = "Error. OPCODE no soportado"; 
                break;
        }
        
        cout << "OPCODE: " << opcode << " (" << op_name << ")\n\n";
        cout << "A: " << a << "\n";
        cout << "B: " << b << "\n";
        
        if (error) {
            cout << "Resultado: " << error_msg << "\n";
        } else {
            cout << "Resultado: " << resultado << "\n";
        }
    }
    
    // Finalizar cronometro
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    
    cout << "\nPID: " << pid << " - Tiempo de ejecucion: " << duration.count() << " microsegundos.\n\n";
}

int main() {
    cout << "Ingresa instrucciones binarias (separadas por espacios): ";
    string input;
    getline(cin, input);
    cout << "\n";
    
    vector<string> instructions;
    stringstream ss(input);
    string token;
    while (ss >> token) {
        instructions.push_back(token);
    }
    
    int n = instructions.size();
    if (n < 3) {
        cout << "Error: Se deben ingresar al menos 3 instrucciones binarias.\n";
        return 1;
    }
    
    // Crear procesos hijos
    for (int i = 1; i < n; ++i) {
        pid_t pid = fork();
        
        if (pid < 0) {
            cerr << "Error al crear el proceso hijo con fork().\n";
            exit(1);
        } else if (pid == 0) {
            // Este es el proceso hijo
            decode_and_execute(instructions[i], i + 1, false);
            exit(0); // El hijo debe terminar aqui
        }
    }
    
    // El proceso padre ejecuta la primera instruccion
    decode_and_execute(instructions[0], 1, true);
    
    // El padre espera a todos sus hijos
    for (int i = 1; i < n; ++i) {
        wait(NULL);
    }
    
    return 0;
}
