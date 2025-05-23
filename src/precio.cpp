///////////////////////////////////////////////////////////////////////////
// Implementación de la funcionalidad relacionada al precio del producto //
///////////////////////////////////////////////////////////////////////////

#ifndef PRECIO_CPP
#define PRECIO_CPP

#include <iostream>
#include <ostream>

#include "../headers/colors.h"
#include "../headers/precio.h"
#include "../headers/util_funcs.h"
#include "../headers/variables.h"


///////////////////////////////////////////////////////////////////////////////////////////////////


bool registrar_precio() {
    precio = pedir_float("el precio del producto (en C$)");

    if (!escribir_archivos(archivo_precio)) {
        return false;
    }

    set_color(Green);
    std::cout << "   ";
    esperar(250);

    std::cout << "Precio actualizado a: C$" << precio << "...";
    esperar(1000);
    reset_color();

    return true;
}


bool mostrar_precio() {
    if (!leer_archivos(archivo_precio)) {
        return false;
    }

    if (checkear_registro(precio)) {
        // no hay nada que mostrar
        return true;
    }

    set_color(Green);
    std::cout << "\n   Precio: C$" << precio;
    esperar(2500);
    reset_color();

    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
