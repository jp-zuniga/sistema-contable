///////////////////////////////////////////////////////////////////////
// Implementación de la funcionalidad del módulo Gestión de Clientes //
///////////////////////////////////////////////////////////////////////

#ifndef CLIENTES_CPP
#define CLIENTES_CPP

#include <cstdint>
#include <iostream>
#include <string>

#include "../headers/clientes.h"
#include "../headers/colors.h"
#include "../headers/util_funcs.h"
#include "../headers/variables.h"


///////////////////////////////////////////////////////////////////////////////////////////////////


int32_t buscar_cliente(const std::string &input) {
    for (int32_t i = 0; i < num_clientes; i++) {
        // verificar nombre y ID, ya que ambos pueden ser input
        if (
            registro_clientes[i].nombre == input
            ||
            registro_clientes[i].id == input
        ) {
            return i;
        }
    }

    return -1;  // no se encontró el cliente
}


///////////////////////////////////////////////////////////////////////////////////////////////////


bool registrar_clientes() {
    limpiar_pantalla();
    std::string temp_ID = "";

    if (!leer_archivos(archivo_clientes)) {
        return false;
    }

    do {
        set_color(Cyan);
        std::cout << "\n                                Cliente #" << num_clientes + 1 << ":\n";
        std::cout << "   ---------------------------------------------------------------------\n";
        reset_color();

        // procesar input para el ID
        while (true) {
            pedir_str("ID", temp_ID);

            if (buscar_cliente(temp_ID) == -1) {
                // el ID ingresado es válido: podemos asignarlo y salir del loop
                registro_clientes[num_clientes].id = temp_ID;
                break;
            }

            // informar al usuario que tiene que intentar de nuevo
            set_color(Red);
            std::cout << "   ERROR: ID ya registrado...";
            reset_color();

            esperar(1500);
            std::cout << "\n";
        }

        pedir_str("nombre", registro_clientes[num_clientes].nombre);
        pedir_str("contacto", registro_clientes[num_clientes].contacto);
        num_clientes++;

    } while (si_no("\n   ¿Desea registrar más clientes? (sí/no) ") == 1);

    if (!escribir_archivos(archivo_clientes)) {
        return false;
    }

    std::cout << "   ";
    esperar(500);
    reset_color();

    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


bool mostrar_clientes() {
    limpiar_pantalla();
    if (!leer_archivos(archivo_clientes)) {
        return false;
    }

    if (checkear_registro(num_clientes)) {
        // no hay nada que mostrar
        return true;
    }

    set_color(Green);
    std::cout << "\n   Mostrando clientes registrados...";
    esperar(500);

    for (int32_t i = 0; i < num_clientes; i++) {
        set_color(Cyan);
        std::cout << "\n\n                                Cliente #" << i + 1 << ":\n";
        std::cout << "   ---------------------------------------------------------------------\n";

        set_color(LightYellow);
        std::cout << "   ID: " << registro_clientes[i].id << "\n";
        std::cout << "   Nombre: " << registro_clientes[i].nombre << "\n";
        std::cout << "   Contacto: " << registro_clientes[i].contacto << "\n";
        std::cout << "   ";

        esperar(800);
    }

    set_color(Cyan);
    std::cout << "\n   ---------------------------------------------------------------------\n";
    std::cout << "   Presione 'Enter' para continuar...";
    reset_color();

    std::cin.ignore();
    std::cin.get();
    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


bool editar_cliente() {
    limpiar_pantalla();
    std::string id = "";
    int32_t indice = 0;
    int32_t info = 0;

    if (!leer_archivos(archivo_clientes)) {
        return false;
    }

    if (checkear_registro(num_clientes)) {
        return true;
    }

    std::cout << "\n";
    pedir_str("ID o nombre del cliente a editar", id);
    indice = buscar_cliente(id);

    set_color(Teal);
    std::cout << "\n   Buscando cliente...";
    esperar(800);

    if (indice == -1) {
        set_color(Red);
        std::cout << "\n   ERROR: cliente ingresado no está registrado...";
        esperar(2250);
        reset_color();
        return true;
    }

    set_color(Green);
    std::cout << "\n   Cliente encontrado!";
    esperar(500);

    set_color(Cyan);
    std::cout << "\n\n                                Cliente #" << indice + 1 << ":\n";
    std::cout << "   ---------------------------------------------------------------------";

    while (info != 1 && info != 2) {
        set_color(LightYellow);
        std::cout << "\n\n   ¿Qué información quiere editar?";
        std::cout << "\n   1. Nombre\n   2. Contacto\n";
        set_color(Teal);

        std::cout << "   Ingrese su opción: ";
        std::cin >> info;

        if (std::cin.fail()) {
            menu_error();
            continue;
        }

        std::cout << "\n";
        reset_color();

        if (info == 1) {
            pedir_str("nombre", registro_clientes[indice].nombre);
        } else if (info == 2) {
            pedir_str("contacto", registro_clientes[indice].contacto);
        } else {
            set_color(Red);
            std::cout << "   Opción inválida...";
            esperar(1000);
            reset_color();
        }
    }

    std::cout << "   ";
    esperar(500);
    set_color(Green);

    if (escribir_archivos(archivo_clientes)) {
        set_color(Green);
        std::cout << "\n   *********************************************************************";
        std::cout << "\n                             Cliente editado...";
    } else {
        return false;
    }

    esperar(2250);
    reset_color();

    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


int32_t eliminar_cliente(const std::string &id) {
    int32_t indice = 0;

    if (!leer_archivos(archivo_clientes)) {
        return -2;
    }

    if (checkear_registro(num_clientes)) {
        return 1;
    }

    indice = buscar_cliente(id);
    if (indice < 0) {
        return indice;
    }

    // mover todos los clientes un espacio a la izquierda
    // para no dejar un índice vacío en medio del arreglo
    for (int32_t i = indice; i < num_clientes - 1; i++) {
        registro_clientes[i] = registro_clientes[i + 1];
    }

    num_clientes--;

    if (escribir_archivos(archivo_clientes)) {
        return 0;
    } else {
        return -2;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
