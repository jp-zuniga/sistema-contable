/////////////////////////////////////////////////////////////
// Implementación de las funciones de utilidad del sistema //
/////////////////////////////////////////////////////////////

#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "../headers/colors.h"
#include "../headers/util_funcs.h"
#include "../headers/variables.h"


///////////////////////////////////////////////////////////////////////////////////////////////////


tm* obtener_fecha() {
    // retorna la cantidad de segundos desde 1970
    time_t now = time(0);

    // convierte la cantidad de segundos a una fecha y hora legible
    tm* time = localtime(&now);

    return time;
}


void limpiar_buffer() {
    std::cin.clear();
    while (std::cin.get() != '\n' && !std::cin.eof()) {
        // limpia el buffer hasta que se encuentre un salto de linea o el final del stream
    }
}


void limpiar_pantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


void menu_error() {
    limpiar_buffer();

    set_color(Red);
    std::cout << "\n   ERROR: tipo de dato inesperado; debe ingresar una opción válida...";
    reset_color();

    esperar(2250);
}


void esperar(const int32_t &tiempo) {
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(tiempo));
}


int32_t si_no(const std::string &pregunta) {
    static std::string input = "";

    set_color(Pink);
    std::cout << pregunta;

    if (std::cin.peek() == '\n') {
        std::cin.ignore();
    }

    std::getline(std::cin, input);

    if (
        input == "s" || input == "S" ||
        input == "si" || input == "Si" || input == "sí" || input == "Sí" ||
        input == "sI" || input == "SI" || input == "sÍ" || input == "SÍ"
    ) {
        return 1;
    }

    else if (
        input == "n" || input == "N" ||
        input == "no" || input == "No" ||
        input == "nO" || input == "NO"
    ) {
        return 0;
    }

    set_color(Red);
    std::cout << "\n   ERROR: respuesta inválida...\n\n";
    reset_color();
    return -1;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


bool pedir_pagada() {
    const static std::string pregunta = "   ¿Ha pagado el cliente? (sí/no) ";

    while (true) {
        // solamente salir si el usuario responde sí o no
        // (una respuesta inválida retorna -1)
        if (si_no(pregunta) == 1) {
            return true;
        } else if (si_no(pregunta) == 0) {
            return false;
        }
    }
}


int32_t pedir_int(const std::string &dato) {
    int32_t num = 0;

    while (true) {
        set_color(LightYellow);
        std::cout << "   Ingresar " << dato << ": ";
        std::cin >> num;

        if (std::cin.fail()) {
            limpiar_buffer();
            set_color(Red);
            std::cout << "\n   ERROR: tipo de dato inespereado y/o valor ingresado es demasiado grande...\n\n";
        } else if (num <= 0) {
            set_color(Red);
            std::cout << "\n   ERROR: debe puede ingresar números enteros positivos...\n\n";
        } else {
            reset_color();
            break;
        }
    }

    return num;
}


float pedir_float(const std::string &dato) {
    float num = 0.00;

    while (true) {
        set_color(LightYellow);
        std::cout << "   Ingresar " << dato << ": ";
        std::cin >> num;

        if (std::cin.fail()) {
            limpiar_buffer();
            set_color(Red);
            std::cout << "\n   ERROR: tipo de dato inespereado y/o valor ingresado es demasiado grande...\n\n";
        } else if (num <= 0) {
            set_color(Red);
            std::cout << "\n   ERROR: debe puede ingresar números enteros positivos...\n\n";
        } else {
            reset_color();
            break;
        }
    }

    return num;
}


void pedir_str(const std::string &dato, std::string &input) {
    set_color(LightYellow);
    std::cout << "   Ingresar " << dato << ": ";

    if (std::cin.peek() == '\n') {
        // std::getline() termina de leer cuando encuentra un salto de línea,
        // entonces si ese el próximo caracter, hay que quitarlo del buffer
        std::cin.ignore();
    }

    std::getline(std::cin, input);
    reset_color();
}


Fecha pedir_fecha() {
    Fecha fecha;

    static constexpr int32_t dias_meses[] = {
        31, 28, 31,
        30, 31, 30,
        31, 31, 30,
        31, 30, 31
    };

    // procesar input para el mes
    while (true) {
        pedir_str("mes (en minúscula)", fecha.mes);

        if (checkear_mes(fecha.mes) != -1) {
            break;
        }

        set_color(Red);
        std::cout << "   ERROR: Mes inválido...";
        reset_color();

        esperar(1500);
        std::cout << "\n";
    }

    // procesar input para el día
    while (true) {
        fecha.dia = pedir_int("día");
        int32_t num_mes = checkear_mes(fecha.mes);

        // si el día ingresado está en el rango válido
        // para el mes ingresado, terminar el loop
        if (fecha.dia <= dias_meses[num_mes]) {
            break;
        }

        // si se ingresó un día inválido, informar al usuario y continuar el loop
        set_color(Red);
        std::cout << "   ERROR: Día inválido para el mes ingresado...";
        reset_color();

        esperar(1500);
        std::cout << "\n";
    }

    while (true) {
        fecha.year = pedir_int("año");
        if (fecha.year >= 2000 && fecha.year <= (obtener_fecha()->tm_year + 1900)) {
            break;
        }

        // si se ingresó un año inválido, informar al usuario y continuar el loop
        set_color(Red);
        std::cout << "   ERROR: El año ingresado debe ser menor que el año actual y mayor que el año 2000...";
        reset_color();

        esperar(1500);
        std::cout << "\n";
    }

    reset_color();
    return fecha;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


int32_t checkear_mes(const std::string &mes) {
    for (int32_t i = 0; i < 12; i++) {
        if (mes == meses[i]) {
            return i;
        }
    }

    return -1;  // el mes es inválido
}


bool checkear_registro(const int32_t &num_registro) {
    if (num_registro == 0) {
        set_color(Red);
        std::cout << "\n   El registro esta vacío...";
        reset_color();

        esperar(2250);
        return true;
    }

    return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


bool leer_archivos(const std::string &nombre_archivo) {
    int32_t i = 0;  // contador de elementos

    std::fstream file;
    file.open(nombre_archivo, std::ios::in);

    if (!file) {
        // si el archivo no existe, crearlo con std::ios::out
        file.open(nombre_archivo, std::ios::out);
        if (!file) {
            // si no se pudo crear/abrir, salir
            return false;
        }

        // cerrar el archivo y volver a abrirlo en std::ios::in
        file.close();
        file.open(nombre_archivo, std::ios::in);

        if (!file) {
            // si todavía da error, ni modo
            return false;
        }

        set_color(Red);
        std::cout << "\n   " << nombre_archivo << " no encontrado...";
        esperar(250);

        set_color(Green);
        std::cout << "\n   Creando " << nombre_archivo << "...";
        esperar(750);

        reset_color();
        std::cout << "\n";
    }

    // leer precio
    if (nombre_archivo == archivo_precio) {
        file >> precio;
    }

    // leer clientes
    else if (nombre_archivo == archivo_clientes) {
        while (!file.eof()) {
            std::getline(file, registro_clientes[i].id);
            if (file.peek() == '\n') {
                file.ignore();
            }

            std::getline(file, registro_clientes[i].nombre);
            if (file.peek() == '\n') {
                file.ignore();
            }

            std::getline(file, registro_clientes[i].contacto);

            if (file.fail()) {
                return false;
            } else if (file.peek() == '\n') {
                file.ignore();
            }

            i++;
        }

        num_clientes = i;
    }

    // leer ventas
    else if (nombre_archivo == archivo_ventas) {
        while (!file.eof()) {
            std::getline(file, registro_ventas[i].id);
            if (file.peek() == '\n') {
                file.ignore();
            }

            std::getline(file, registro_ventas[i].nombre_cliente);
            file >> registro_ventas[i].fecha.dia;

            if (file.peek() == '\n') {
                file.ignore();
            }

            std::getline(file, registro_ventas[i].fecha.mes);
            file >> registro_ventas[i].fecha.year;
            file >> registro_ventas[i].cantidad_producto;
            file >> registro_ventas[i].monto;
            file >> registro_ventas[i].pagada;

            if (file.fail()) {
                return false;
            } else if (file.peek() == '\n') {
                file.ignore();
            }

            i++;
        }

        num_ventas = i;
    }

    // leer pagos pendientes
    else if (nombre_archivo == archivo_pendientes) {
        while (!file.eof()) {
            std::getline(file, registro_pendientes[i].id_venta);
            file >> registro_pendientes[i].fecha.dia;

            if (file.peek() == '\n') {
                file.ignore();
            }

            std::getline(file, registro_pendientes[i].fecha.mes);
            file >> registro_pendientes[i].fecha.year;

            if (file.peek() == '\n') {
                file.ignore();
            }

            std::getline(file, registro_pendientes[i].nombre_cliente);
            file >> registro_pendientes[i].monto;

            if (file.fail()) {
                return false;
            } else if (file.peek() == '\n') {
                file.ignore();
            }

            i++;
        }

        num_pendientes = i;
    }

    // leer costos fijos
    else if (nombre_archivo == archivo_costos_fijos) {
        while (!file.eof()) {
            std::getline(file, registro_costos_fijos[i].id);
            file >> registro_costos_fijos[i].monto;

            if (file.peek() == '\n') {
                file.ignore();
            }

            std::getline(file, registro_costos_fijos[i].descripcion);

            if (file.fail()) {
                return false;
            } else if (file.peek() == '\n') {
                file.ignore();
            }

            i++;
        }

        num_costos_fijos = i;
    }

    // leer costos variables
    else if (nombre_archivo == archivo_costos_variables) {
        while (!file.eof()) {
            std::getline(file, registro_costos_variables[i].id);
            file >> registro_costos_variables[i].monto;

            if (file.peek() == '\n') {
                file.ignore();
            }

            std::getline(file, registro_costos_variables[i].descripcion);
            if (file.peek() == '\n') {
                file.ignore();
            }

            std::getline(file, registro_costos_variables[i].mes);

            if (file.fail()) {
                return false;
            } else if (file.peek() == '\n') {
                file.ignore();
            }

            i++;
        }

        num_costos_variables = i;
    }

    file.close();
    return true;
}


bool escribir_archivos(const std::string &nombre_archivo) {
    int32_t x = 0;  // contador de elementos

    std::ofstream file;
    file.open(nombre_archivo, std::ios::trunc | std::ios::out);

    if (!file) {
        return false;
    }

    // escribir precio
    if (nombre_archivo == archivo_precio) {
        file << precio;
    }

    // escribir clientes
    else if (nombre_archivo == archivo_clientes) {
        for (int32_t i = 0; i < num_clientes; i++) {
            file << registro_clientes[i].id << "\n";
            file << registro_clientes[i].nombre << "\n";
            file << registro_clientes[i].contacto << "\n";
            x++;
        }

        num_clientes = x;
    }

    // escribir ventas
    else if (nombre_archivo == archivo_ventas) {
        for (int32_t i = 0; i < num_ventas; i++) {
            file << registro_ventas[i].id << "\n";
            file << registro_ventas[i].nombre_cliente << "\n";
            file << registro_ventas[i].fecha.dia << "\n";
            file << registro_ventas[i].fecha.mes << "\n";
            file << registro_ventas[i].fecha.year << "\n";
            file << registro_ventas[i].cantidad_producto << "\n";
            file << registro_ventas[i].monto << "\n";
            file << registro_ventas[i].pagada << "\n";
            x++;
        }

        num_ventas = x;
    }

    // escribir pagos pendientes
    else if (nombre_archivo == archivo_pendientes) {
        for (int32_t i = 0; i < num_pendientes; i++) {
            file << registro_pendientes[i].id_venta << "\n";
            file << registro_pendientes[i].fecha.dia << "\n";
            file << registro_pendientes[i].fecha.mes << "\n";
            file << registro_pendientes[i].fecha.year << "\n";
            file << registro_pendientes[i].nombre_cliente << "\n";
            file << registro_pendientes[i].monto << "\n";
            x++;
        }

        num_pendientes = x;
    }

    // escribir costos fijos
    else if (nombre_archivo == archivo_costos_fijos) {
        for (int32_t i = 0; i < num_costos_fijos; i++) {
            file << registro_costos_fijos[i].id << "\n";
            file << registro_costos_fijos[i].monto << "\n";
            file << registro_costos_fijos[i].descripcion << "\n";
            x++;
        }

        num_costos_fijos = x;
    }

    // escribir costos variables
    else if (nombre_archivo == archivo_costos_variables) {
        for (int32_t i = 0; i < num_costos_variables; i++) {
            file << registro_costos_variables[i].id << "\n";
            file << registro_costos_variables[i].monto << "\n";
            file << registro_costos_variables[i].descripcion << "\n";
            file << registro_costos_variables[i].mes << "\n";
            x++;
        }

        num_costos_variables = x;
    }

    file.close();
    return true;
}
