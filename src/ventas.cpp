/////////////////////////////////////////////////////////////////////
// Implementación de la funcionalidad del módulo Gestión de Ventas //
/////////////////////////////////////////////////////////////////////

#ifndef VENTAS_CPP
#define VENTAS_CPP

#include <cstdint>
#include <cmath>
#include <iostream>
#include <string>

#include "../headers/clientes.h"
#include "../headers/colors.h"
#include "../headers/pendientes.h"
#include "../headers/ventas.h"
#include "../headers/util_funcs.h"
#include "../headers/variables.h"


///////////////////////////////////////////////////////////////////////////////////////////////////


int32_t buscar_venta(const std::string &id) {
    for (int32_t i = 0; i < num_ventas; i++) {
        if (registro_ventas[i].id == id) {
            return i;
        }
    }

    return -1;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


bool registrar_ventas() {
    limpiar_pantalla();
    std::string temp_ID = "";
    std::string temp_nombre = "";
    int32_t respuesta = 0;  // para almacenar los returns de si_no()

    // leer los clientes para ver si se ingresó un cliente registrado
    // leer el precio para calcular el monto de la venta
    if (
        !leer_archivos(archivo_ventas) ||
        !leer_archivos(archivo_pendientes) ||
        !leer_archivos(archivo_clientes) ||
        !leer_archivos(archivo_precio)
    ) {
        // si cualquiera de las lecturas falla, retornar false
        return false;
    }

    // si el archivo 'registro_precio.txt' no existe, se va a crear,
    // pero va a estar vacio, entonces hay que pedir un precio para
    // poder calcular el monto de las ventas
    if (precio == 0.00) {
        set_color(Red);
        std::cout << "\n   Archivo 'registro_precio.txt' vacío...\n\n";
        precio = pedir_float("precio");

        if (escribir_archivos(archivo_precio)) {
            set_color(Green);
            std::cout << "   Precio guardado...";
        } else {
            return false;
        }

        esperar(500);
        std::cout << "\n";
        reset_color();
    }

    // pedir los datos de la venta a registrar
    do {
        set_color(Cyan);
        std::cout << "\n                                  Venta #" << num_ventas + 1 << ":\n";
        std::cout << "   ---------------------------------------------------------------------\n";
        std::cin.ignore();
        reset_color();

        // pedir y validar ID
        while (true) {
            pedir_str("ID", temp_ID);

            // si el ID ingresado ya existe, informar al usuario y continuar el loop
            if (buscar_venta(temp_ID) >= 0) {
                set_color(Red);
                std::cout << "   ERROR: ID ya registrado...";
                reset_color();

                esperar(1500);
                std::cout << "\n";
                continue;
            }

            // si no se encontro, se guarda en el registro y termina el loop
            registro_ventas[num_ventas].id = temp_ID;
            break;
        }

        // pedir y validar cliente
        pedir_str("nombre del cliente", temp_nombre);

        // el cliente ingresado no existe, entonces no se puede registrar la venta
        if (buscar_cliente(temp_nombre) == -1) {
            set_color(Red);
            std::cout << "   ERROR: Cliente debe estar registrado antes de registrar la venta...";
            reset_color();

            esperar(1500);
            std::cout << "\n";

            // si el cliente no esta registrado, se borra el id de la venta actual
            // y se escriben los cambios que habia hecho el usuario
            registro_ventas[num_ventas].id.clear();
            return (escribir_archivos(archivo_ventas) && escribir_archivos(archivo_pendientes));
        }

        registro_ventas[num_ventas].nombre_cliente = temp_nombre;
        respuesta = si_no("\n   ¿Desea ingresar la fecha manualmente? (sí/no) ");

        if (respuesta == 1) {
            // pedir manualmente
            std::cout << "\n";
            registro_ventas[num_ventas].fecha = pedir_fecha();
        } else {
            // se quiere usar la fecha actual, o se ingresó un input inválido
            if (respuesta == 0) {
                set_color(Teal);
                std::cout << "\n   De acuerdo, se usará la fecha actual...\n";
            } else {
                set_color(Red);
                std::cout << "\n   ERROR: Input inválido, se usará la fecha actual...\n";
            }

            reset_color();
            tm *time = obtener_fecha();
            registro_ventas[num_ventas].fecha.dia = time->tm_mday;
            registro_ventas[num_ventas].fecha.mes = meses[time->tm_mon];
            registro_ventas[num_ventas].fecha.year = time->tm_year + 1900;
        }

        registro_ventas[num_ventas].cantidad_producto = pedir_int("cantidad de unidades compradas");

        set_color(Teal);
        std::cout << "\n   Monto calculado utilizando cantidad y precio previamente ingresados...\n";
        reset_color();

        registro_ventas[num_ventas].monto = precio * registro_ventas[num_ventas].cantidad_producto;
        registro_ventas[num_ventas].pagada = pedir_pagada();

        // si se ingresa que no se ha pagado la compra, agregar los
        // datos del cliente y la compra al registro de pagos pendientes
        if (!registro_ventas[num_ventas].pagada) {
            registro_pendientes[num_pendientes].id_venta = registro_ventas[num_ventas].id;
            registro_pendientes[num_pendientes].fecha = registro_ventas[num_ventas].fecha;
            registro_pendientes[num_pendientes].nombre_cliente = registro_ventas[num_ventas].nombre_cliente;
            registro_pendientes[num_pendientes].monto = registro_ventas[num_ventas].monto;

            set_color(Green);
            std::cout << "   Pago pendiente registrado...";
            esperar(500);
            reset_color();

            std::cout << "\n\n";
            num_pendientes++;
        }

        num_ventas++;

    } while (si_no("   ¿Desea registrar más ventas? (sí/no) ") == 1);

    if (
        !escribir_archivos(archivo_ventas)
         ||
        !escribir_archivos(archivo_pendientes)
    ) {
        return false;
    }

    std::cout << "   ";
    esperar(500);
    reset_color();
    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


bool mostrar_ventas() {
    limpiar_pantalla();
    std::string pagada = "";

    if (!leer_archivos(archivo_ventas)) {
        return false;
    }

    if (checkear_registro(num_ventas)) {
        return true;
    }

    set_color(Green);
    std::cout << "\n   Mostrando ventas registradas...";
    esperar(800);

    std::cout << "\n";
    for (int32_t i = 0; i < num_ventas; i++) {
        set_color(Cyan);
        std::cout << "\n                                  Venta #" << i + 1 << ":\n";
        std::cout << "   ---------------------------------------------------------------------\n";

        set_color(LightYellow);
        std::cout << "   ID: " << registro_ventas[i].id << "\n";
        std::cout << "   Fecha: " << registro_ventas[i].fecha.dia << " de " << registro_ventas[i].fecha.mes << ", " << registro_ventas[i].fecha.year << "\n";
        std::cout << "   Nombre de cliente: " << registro_ventas[i].nombre_cliente << "\n";
        std::cout << "   Cantidad comprada: " << registro_ventas[i].cantidad_producto << " unidad(es)\n";
        std::cout << "   Monto: C$" << registro_ventas[i].monto << "\n";
        std::cout << "   ¿Está pagada? ";
        set_color(Teal);

        // convertir el bool a un string para imprimir correctamente
        std::cout << (registro_ventas[i].pagada ? "Sí" : "No") << "\n";
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


bool editar_venta() {
    limpiar_pantalla();
    int32_t indice = 0;
    int32_t indice_pendiente = 0;
    int32_t info = 0;

    std::string id = "";
    std::string temp_nombre = "";

    if (
        !leer_archivos(archivo_pendientes) ||
        !leer_archivos(archivo_ventas) ||
        !leer_archivos(archivo_clientes) ||
        !leer_archivos(archivo_precio)
    ) {
        return false;
    }

    if (checkear_registro(num_ventas)) {
        return true;
    }

    std::cout << "\n";
    pedir_str("ID de la venta a editar", id);

    indice = buscar_venta(id);
    indice_pendiente = buscar_pendiente(id);

    set_color(Teal);
    std::cout << "\n   Buscando venta...";
    esperar(800);
    reset_color();

    if (indice == -1) {
        set_color(Red);
        std::cout << "\n   ERROR: ID ingresado no esta registrado...";
        esperar(2250);
        reset_color();
        return true;
    }

    set_color(Green);
    std::cout << "\n   Venta encontrada!";
    esperar(500);

    // mostrar datos de la venta a editar:
    set_color(Cyan);
    std::cout << "\n\n                                  Venta #" << indice + 1 << ":\n";
    std::cout << "   ***********************************************************************";
    set_color(LightYellow);

    std::cout << "   ID: " << registro_ventas[indice].id << "\n";
    std::cout << "   Fecha: " << registro_ventas[indice].fecha.dia << " de " << registro_ventas[indice].fecha.mes << ", " << registro_ventas[indice].fecha.year << "\n";
    std::cout << "   Nombre de cliente: " << registro_ventas[indice].nombre_cliente << "\n";
    std::cout << "   Cantidad comprada: " << registro_ventas[indice].cantidad_producto << " unidades\n";
    std::cout << "   Monto: C$" << registro_ventas[indice].monto << "\n";
    std::cout << "   ¿Está pagada? ";
    set_color(Teal);
    std::cout << (registro_ventas[indice].pagada ? "Sí" : "No") << "\n";

    do {
        set_color(LightYellow);
        std::cout << "\n\n   ¿Qué información quiere editar?";
        std::cout << "\n   1. Fecha";
        std::cout << "\n   2. Nombre del cliente";
        std::cout << "\n   3. Cantidad de leche comprada";
        std::cout << "\n   4. Monto";
        std::cout << "\n   5. Estado de pago";
        set_color(Teal);

        std::cout << "   Ingrese su opción: ";
        std::cin >> info;

        if (std::cin.fail()) {
            menu_error();
            continue;
        }

        std::cout << "\n";
        reset_color();

        // procesar input:
        switch (info) {
            case 1:
                std::cout << "\n";
                registro_ventas[indice].fecha = pedir_fecha();

                if (!registro_ventas[indice].pagada && indice_pendiente >= 0) {
                    // si la venta no está pagada, también actualizar
                    // la fecha en el registro de pendientes
                    registro_pendientes[indice_pendiente].fecha = registro_ventas[indice].fecha;
                }

                break;

            case 2:
                while (true) {
                    pedir_str("nombre del cliente", temp_nombre);

                    if (buscar_cliente(temp_nombre) >= 0) {
                        set_color(Red);
                        std::cout << "   ERROR: Cliente debe estar registrado para editar la venta...";
                        reset_color();

                        esperar(1500);
                        return true;
                    }

                    registro_ventas[indice].nombre_cliente = temp_nombre;
                    break;
                }

                if (!registro_ventas[indice].pagada){
                    // si la venta estaba registrada como pendiente,
                    // actualizar el nombre en el registro de pendientes
                    registro_pendientes[indice_pendiente].nombre_cliente = registro_ventas[indice].nombre_cliente;
                }

                break;

            case 3:
                registro_ventas[indice].cantidad_producto = pedir_int("cantidad de unidades compradas");
                if (precio == 0.00) {
                    set_color(Red);
                    std::cout << "   ERROR: Precio por unidad no registrado...";
                    esperar(2250);

                    reset_color();
                    return true;
                }

                // si cambió la cantidad de unidades, hay que actualizar el monto
                registro_ventas[indice].monto = precio * registro_ventas[indice].cantidad_producto;

                if (!registro_ventas[indice].pagada && indice_pendiente >= 0) {
                    // si la venta no está pagada, actualizar el monto en el registro de pendientes
                    registro_pendientes[indice_pendiente].monto = registro_ventas[indice].monto;
                }

                break;

            case 4:
                registro_ventas[indice].monto = pedir_float("monto (en C$)");
                if (precio == 0.00) {
                    set_color(Red);
                    std::cout << "   ERROR: Precio por unidad no registrado...";
                    esperar(2250);

                    reset_color();
                    return true;
                }

                // si cambió el monto, hay que actualizar la cantidad de unidades
                registro_ventas[indice].cantidad_producto = static_cast<int32_t>(registro_ventas[indice].monto / precio);

                if (!registro_ventas[indice].pagada && indice_pendiente >= 0) {
                    // si la venta no está pagada, actualizar el monto en el registro de pendientes
                    registro_pendientes[indice_pendiente].monto = registro_ventas[indice].monto;
                }

                break;

            case 5:
                registro_ventas[indice].pagada = pedir_pagada();

                if (registro_ventas[indice].pagada) {
                    eliminar_pendiente(registro_pendientes[indice_pendiente].id_venta);
                } else {
                    registro_pendientes[num_pendientes].id_venta = registro_ventas[indice].id;
                    registro_pendientes[num_pendientes].fecha = registro_ventas[indice].fecha;
                    registro_pendientes[num_pendientes].nombre_cliente = registro_ventas[indice].nombre_cliente;
                    registro_pendientes[num_pendientes].monto = registro_ventas[indice].monto;
                    num_pendientes++;
                }

                break;

            default:
                set_color(Red);
                std::cout << "   Opción inválida...";
                esperar(1000);

                reset_color();
                break;
        }
    } while (info < 1 || info > 5);

    std::cout << "   ";
    esperar(500);
    set_color(Green);

    if (escribir_archivos(archivo_ventas) && escribir_archivos(archivo_pendientes)) {
        set_color(Green);
        std::cout << "\n   ***********************************************************************";
        std::cout << "\n                              Venta editada...";
    }

    else {
        return false;
    }

    esperar(2250);
    reset_color();
    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


int32_t eliminar_venta(const std::string &id) {
    int32_t indice = 0;

    if (!leer_archivos(archivo_ventas) || !leer_archivos(archivo_pendientes)) {
        return -2;
    }

    if (checkear_registro(num_ventas)) {
        return 1;
    }

    indice = buscar_venta(id);
    if (indice < 0) {
        return indice;
    }

    // si la venta no se ha pagado, significa que también estaba
    // registrada como pendiente, entonces se elimina de ese registro igual
    if (!registro_ventas[indice].pagada) {
        eliminar_pendiente(id);
    }

    for (int32_t i = indice; i < num_ventas - 1; i++) {
        registro_ventas[i] = registro_ventas[i + 1];
    }

    num_ventas--;

    if (escribir_archivos(archivo_ventas) && escribir_archivos(archivo_pendientes)) {
        return 0;
    }

    else {
        return -2;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
