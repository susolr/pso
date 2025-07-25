/**
 * @file pruebas.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <signal.h>     // Para la función kill
#include <sys/types.h>  // Para el tipo pid_t
#include <unistd.h>     // Para la función getpid, fork

#include <cstdlib>  // Para la función system
#include <iostream>

int main() {
    std::string comando = "top";  // Comando a ejecutar

    // Creamos un nuevo proceso hijo
    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "Error al crear el proceso hijo." << std::endl;
        return 1;
    } else if (pid == 0) {
        // Este es el proceso hijo
        std::cout << "Soy el proceso hijo con PID: " << getpid() << std::endl;
        // Ejecutamos el comando
        system(comando.c_str());
        // El proceso hijo termina aquí
        exit(0);
    } else {
        // Este es el proceso padre
        std::cout << "Soy el proceso padre con PID: " << getpid() << std::endl;
        std::cout << "El PID del proceso hijo es: " << pid << std::endl;

        // Esperar un momento antes de terminar el proceso hijo
        sleep(10);

        // Terminar el proceso hijo utilizando la función kill
        kill(pid, SIGKILL);
        kill(pid, SIGTERM);
    }

    return 0;
}
