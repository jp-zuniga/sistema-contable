/////////////////////////////////////////////////////////////////////
// Implementación de la funcionalidad del módulo Gestión de Costos //
/////////////////////////////////////////////////////////////////////

#ifndef COSTOS_CPP
#define COSTOS_CPP

#include <cstdint>
#include <iostream>
#include <string>

#include "../headers/colors.h"
#include "../headers/costos.h"
#include "../headers/util_funcs.h"
#include "../headers/variables.h"


///////////////////////////////////////////////////////////////////////////////////////////////////


int32_t buscar_costo_fijo(const std::string &id) {
    for (int32_t i = 0; i < num_costos_fijos; i++) {
        if (registro_costos_fijos[i].id == id) {
            return i;
        }
    }

    return -1;  // no se encontró el costo
}


int32_t buscar_costo_variable(const std::string &id) {
    for (int32_t i = 0; i < num_costos_variables; i++) {
        if (registro_costos_variables[i].id == id) {
            return i;
        }
    }

    return -1;  // no se encontró el costo
}


///////////////////////////////////////////////////////////////////////////////////////////////////


bool registrar_costos_fijos() {
    limpiar_pantalla();
    std::string temp_ID = "";

    if (!leer_archivos(archivo_costos_fijos)) {
        return false;
    }

    do {
        set_color(Cyan);
        std::cout << "\n                              Costo Fijo #" << num_costos_fijos + 1 << ":\n";
        std::cout << "   ---------------------------------------------------------------------\n";
        reset_color();

        // procesar input para el ID
        while (true) {
            pedir_str("ID", temp_ID);

            // el ID ingresado es válido, se asigna y termina el loop
            if (buscar_costo_fijo(temp_ID) == -1) {
                registro_costos_fijos[num_costos_fijos].id = temp_ID;
                break;
            }

            // imprimir error, continuar loop y volver a pedir ID
            set_color(Red);
            std::cout << "   ERROR: ID ya registrado...";
            reset_color();

            esperar(1500);
            std::cout << "\n";
        }

        registro_costos_fijos[num_costos_fijos].monto = pedir_float("monto (en C$)");
        pedir_str("descripción", registro_costos_fijos[num_costos_fijos].descripcion);

        num_costos_fijos++;

    } while (si_no("\n   ¿Desea registrar más costos fijos? (sí/no) ") == 1);

    if (!escribir_archivos(archivo_costos_fijos)) {
        return false;
    }

    std::cout << "   ";
    esperar(500);
    reset_color();

    return true;
}


bool registrar_costos_variables() {
    limpiar_pantalla();
    tm* time = obtener_fecha();

    std::string temp_ID = "";
    int32_t respuesta = 0;

    if (!leer_archivos(archivo_costos_variables)) {
        return false;
    }

    do {
        set_color(Cyan);
        std::cout << "\n                            Costo Variable #" << num_costos_variables + 1 << ":\n";
        std::cout << "   ---------------------------------------------------------------------\n";
        reset_color();

        // pedir ID del costo a registrar
        while (true) {
            pedir_str("ID", temp_ID);

            // solo asignar si el ID ingresado no está registrado
            if (buscar_costo_variable(temp_ID) == -1) {
                registro_costos_variables[num_costos_variables].id = temp_ID;
                break;
            }

            // imprimir error, continuar loop y volver a pedir ID
            set_color(Red);
            std::cout << "   ERROR: ID ya registrado...";
            reset_color();

            esperar(1500);
            std::cout << "\n";
        }

        registro_costos_variables[num_costos_variables].monto = pedir_float("monto (en C$)");

        limpiar_buffer();
        respuesta = si_no("\n   ¿Desea ingresar el mes manualmente? (si/no) ");

        // se quiere ingresar manualmente el mes
        if (respuesta == 1) {
            std::cout << "\n";
            while (true) {
                pedir_str(
                    "mes (en minúscula)",
                    registro_costos_variables[num_costos_variables].mes
                );

                if (checkear_mes(registro_costos_variables[num_costos_variables].mes) < 0) {
                    set_color(Red);
                    std::cout << "   ERROR: Mes inválido...";
                    reset_color();

                    esperar(1500);
                    std::cout << "\n";
                    continue;
                } else {
                    break;
                }
            }
        } else {
            // el usuario no quiere ingresar el mes, o ingresó un input inválido:
            if (respuesta == 0) {
                set_color(Teal);
                std::cout << "   De acuerdo, se usará el mes actual...";
            } else {
                set_color(Red);
                std::cout << "   ERROR: Input inválido, se usará el mes actual...";
            }

            reset_color();
            std::cout << "\n\n";
            registro_costos_variables[num_costos_variables].mes = meses[time->tm_mon];
        }

        pedir_str("descripción", registro_costos_variables[num_costos_variables].descripcion);
        num_costos_variables++;

    } while (si_no("\n   ¿Desea registrar más costos? (sí/no) ") == 1);

    if (!escribir_archivos(archivo_costos_variables)) {
        return false;
    }

    std::cout << "   ";
    esperar(500);
    reset_color();
    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


bool mostrar_costos_fijos() {
    limpiar_pantalla();
    if (!leer_archivos(archivo_costos_fijos)) {
        return false;
    }

    if (checkear_registro(num_costos_fijos)) {
        return true;
    }

    set_color(Green);
    std::cout << "\n   Mostrando costos fijos registrados...";
    esperar(800);

    std::cout << "\n";
    for (int32_t i = 0; i < num_costos_fijos; i++) {
        set_color(Cyan);
        std::cout << "\n                              Costo Fijo #" << i + 1 << ":\n";
        std::cout << "   ---------------------------------------------------------------------\n";
        set_color(LightYellow);

        std::cout << "   ID: " << registro_costos_fijos[i].id << "\n";
        std::cout << "   Monto: C$" << registro_costos_fijos[i].monto << "\n";
        std::cout << "   Descripción: " << registro_costos_fijos[i].descripcion << "\n";
        std::cout << "   ";

        esperar(800);
    }

    set_color(Cyan);
    std::cout << "\n   ---------------------------------------------------------------------\n";
    std::cout << "   Presione 'Enter' para continuar...";
    std::cin.ignore();
    std::cin.get();
    reset_color();
    return true;
}


bool mostrar_costos_variables() {
    limpiar_pantalla();
    if (!leer_archivos(archivo_costos_variables)) {
        return false;
    }

    if (checkear_registro(num_costos_variables)) {
        return true;
    }

    set_color(Green);
    std::cout << "\n   Mostrando costos variables registrados...";
    esperar(800);

    std::cout << "\n";
    for (int32_t i = 0; i < num_costos_variables; i++) {
        set_color(Cyan);
        std::cout << "\n                            Costo Variable #" << i + 1 << ":\n";
        std::cout << "   ---------------------------------------------------------------------\n";
        set_color(LightYellow);

        std::cout << "   ID: " << registro_costos_variables[i].id << "\n";
        std::cout << "   Monto: C$" << registro_costos_variables[i].monto << "\n";
        std::cout << "   Descripción: " << registro_costos_variables[i].descripcion << "\n";
        std::cout << "   Mes: ";
        set_color(Teal);
        std::cout << registro_costos_variables[i].mes << "\n";
        std::cout << "   ";

        esperar(800);
    }

    set_color(Cyan);
    std::cout << "\n   ---------------------------------------------------------------------\n";
    std::cout << "   Presione 'Enter' para continuar...";
    std::cin.ignore();
    std::cin.get();
    reset_color();
    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


bool editar_costo_fijo() {
    limpiar_pantalla();
    std::string id = "";
    int32_t indice = 0;
    int32_t info = 0;

    if (!leer_archivos(archivo_costos_fijos)) {
        return false;
    }

    if (checkear_registro(num_costos_fijos)) {
        return true;
    }

    std::cout << "\n";
    pedir_str("ID del costo fijo a editar", id);
    indice = buscar_costo_fijo(id);

    set_color(Teal);
    std::cout << "\n   Buscando costo fijo...";
    esperar(800);
    reset_color();

    if (indice == -1) {
        set_color(Red);
        std::cout << "\n   ERROR: ID ingresado no está registrado...";
        esperar(2250);
        reset_color();
        return true;
    }

    set_color(Green);
    std::cout << "\n   Costo fijo encontrado!";
    esperar(500);
    set_color(Cyan);

    std::cout << "\n\n                              Costo Fijo #" << indice + 1 << ":\n";
    std::cout << "   *********************************************************************";

    while (info != 1 && info != 2) {
        set_color(LightYellow);
        std::cout << "\n\n   ¿Qué información quiere editar?";
        std::cout << "\n   1. Monto\n   2. Descripción\n";
        set_color(Teal);

        std::cout << "   Ingrese su opción: ";
        std::cin >> info;

        if (std::cin.fail()) {
            menu_error();
            continue;
        }

        std::cout << "\n";
        reset_color();

        switch (info) {
            case 1:
                registro_costos_fijos[indice].monto = pedir_float("monto (en C$)");
                break;
            case 2:
                pedir_str("descripción", registro_costos_fijos[indice].descripcion);
                break;
            default:
                set_color(Red);
                std::cout << "   Opción inválida...";
                esperar(1000);
                reset_color();
                break;
        }
    }

    std::cout << "   ";
    esperar(500);

    if (escribir_archivos(archivo_costos_fijos)) {
        set_color(Green);
        std::cout << "\n   *********************************************************************";
        std::cout << "\n                           Costo fijo editado...                        ";
    } else {
        return false;
    }

    esperar(2250);
    reset_color();

    return true;
}


bool editar_costo_variable() {
    limpiar_pantalla();
    std::string id = "";
    int32_t indice = 0;
    int32_t info = 0;

    if (!leer_archivos(archivo_costos_variables)) {
        return false;
    }

    if (checkear_registro(num_costos_variables)) {
        return true;
    }

    std::cout << "\n";
    pedir_str("ID del costo variable a editar", id);
    indice = buscar_costo_variable(id);

    set_color(Teal);
    std::cout << "\n   Buscando costo variable...";
    esperar(800);
    reset_color();

    if (indice == -1) {
        set_color(Red);
        std::cout << "\n   ERROR: ID ingresado no está registrado...";
        esperar(2250);
        reset_color();
        return true;
    }

    set_color(Green);
    std::cout << "\n   Costo variable encontrado!";
    esperar(500);

    set_color(Cyan);
    std::cout << "\n\n                            Costo Variable #" << indice + 1 << ":\n";
    std::cout << "   ***********************************************************************";

    while (info < 1 || info > 3) {
        set_color(LightYellow);
        std::cout << "\n\n   ¿Qué información quiere editar?";
        std::cout << "\n   1. Monto\n   2. Descripción\n   3. Mes\n";

        set_color(Teal);
        std::cout << "   Ingrese su opción: ";
        std::cin >> info;

        if (std::cin.fail()) {
            menu_error();
            continue;
        }

        std::cout << "\n";
        reset_color();

        switch (info) {
            case 1:
                registro_costos_variables[indice].monto = pedir_float("monto (en C$)");
                break;
            case 2:
                pedir_str("descripción", registro_costos_variables[indice].descripcion);
                break;
            case 3:
                std::cout << "\n";

                // pedir mes hasta que se ingrese uno válido
                while (true) {
                    pedir_str("mes (en minúscula)", registro_costos_variables[indice].mes);

                    if (checkear_mes(registro_costos_variables[indice].mes) >= 0) {
                        break;
                    }

                    set_color(Red);
                    std::cout << "   ERROR: Mes inválido...";
                    reset_color();
                    esperar(1500);
                    std::cout << "\n";
                }

                break;
            default:
                set_color(Red);
                std::cout << "   Opción inválida...";
                reset_color();
                esperar(1000);
                break;
        }
    }

    std::cout << "   ";
    esperar(500);
    set_color(Green);

    if (escribir_archivos(archivo_costos_variables)) {
        std::cout << "\n   ***********************************************************************";
        std::cout << "\n                          Costo variable editado...";
    } else {
        return false;
    }

    esperar(2250);
    reset_color();

    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


int32_t eliminar_costo_fijo(const std::string &id) {
    int32_t indice = 0;

    if (!leer_archivos(archivo_costos_fijos)) {
        return -2;
    }

    if (checkear_registro(num_costos_fijos)) {
        return 1;
    }

    indice = buscar_costo_fijo(id);
    if (indice < 0) {
        return indice;
    }

    // mover los costos después del costo ingresado
    // a la izquierda para ocupar el espacio vacío
    for (int32_t i = indice; i < num_costos_fijos - 1; i++) {
        registro_costos_fijos[i] = registro_costos_fijos[i + 1];
    }

    num_costos_fijos--;

    if (escribir_archivos(archivo_costos_fijos)) {
        return 0;
    } else {
        return -2;
    }
}


int32_t eliminar_costo_variable(const std::string &id) {
    int32_t indice = 0;

    if (!leer_archivos(archivo_costos_variables)) {
        return -2;
    }

    if (checkear_registro(num_costos_variables)) {
        return 1;
    }

    indice = buscar_costo_variable(id);
    if (indice < 0) {
        return indice;
    }

    // mover los costos después del costo ingresado
    // a la izquierda para ocupar el espacio vacío
    for (int32_t i = indice; i < num_costos_variables - 1; i++) {
        registro_costos_variables[i] = registro_costos_variables[i + 1];
    }

    num_costos_variables--;

    if (escribir_archivos(archivo_costos_variables)) {
        return 0;
    } else {
        return -2;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
