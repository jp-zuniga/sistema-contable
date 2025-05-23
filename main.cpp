////////////////////////////////////////////////////////////
// Proyecto de curso de la asignatura Lógica y Algoritmos //
////////////////////////////////////////////////////////////
// -- Realizado por: Joaquín Zúñiga                       //
// -- Entregado: junio, 2024                              //
// -- Modificado: mayo, 2025                              //
////////////////////////////////////////////////////////////

#include <cstdint>
#include <filesystem>
#include <iostream>

#include "headers/colors.h"
#include "headers/menus.h"
#include "headers/util_funcs.h"

namespace fs = std::filesystem;


///////////////////////////////////////////////////////////////////////////////////////////////////


int main() {
    #ifdef _WIN32
        // activar UTF-8 encoding en los terminales de Windows
        SetConsoleOutputCP(CP_UTF8);
    #endif

    // crear folder si no existe
    if (!fs::exists("datos")) {
        // avisar si hubo un error al crear el folder
        if (!fs::create_directory("datos")) {
            set_color(Red);
            std::cout << "\n   No se pudo crear el folder para los archivos de datos!";
            reset_color();

            std::cin.ignore();
            std::cin.get();
            return 0;
        }
    }

    // para almacenar los returns de los menus
    int32_t init = 0;
    int32_t acceder = 0;

    do {
        limpiar_pantalla();
        init = inicio();

        if (init == 1) {
            acceder = 0;
            set_color(LightYellow);
            std::cout << "      Abriendo menú...";
            reset_color();
            esperar(1000);

            while (acceder != 3) {
                // mientras no se quiera regresar a la pantalla principal
                acceder = principal();
            }
        }

        else if (init == 2) {
            set_color(Red);
            std::cout << "      Cerrando programa...";
            reset_color();
            break;
        }

        else {
            set_color(Red);
            std::cout << "      Opción inválida...";
            reset_color();
            esperar(1000);
            continue;
        }

    // mientras no se seleccione cerrar el programa
    } while (init != 2);

    esperar(2000);
    std::cout << std::endl;
    return 0;
}
