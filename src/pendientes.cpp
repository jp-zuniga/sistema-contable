/////////////////////////////////////////////////////////////////////
// Implementación de la funcionalidad del módulo Gestión de Ventas //
/////////////////////////////////////////////////////////////////////

#ifndef PENDIENTES_CPP
#define PENDIENTES_CPP

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


int32_t buscar_pendiente(const std::string &id) {
    for (int32_t i = 0; i < num_pendientes; i++) {
        if (registro_pendientes[i].id_venta == id) {
            return i;
        }
    }

    return -1;  // no se encontró el pendiente
}


///////////////////////////////////////////////////////////////////////////////////////////////////


bool mostrar_pendientes() {
    limpiar_pantalla();

    if (!leer_archivos(archivo_pendientes)) {
        return false;
    }

    if (checkear_registro(num_pendientes)) {
        return true;
    }

    set_color(Green);
    std::cout << "\n   Mostrando pagos pendientes...";
    esperar(800);

    std::cout << "\n";
    for (int32_t i = 0; i < num_pendientes; i++) {
        set_color(Cyan);
        std::cout << "\n                              Pago Pendiente #" << i + 1 << ":\n";
        std::cout << "   ---------------------------------------------------------------------\n";
        set_color(LightYellow);

        std::cout << "   ID de venta: " << registro_pendientes[i].id_venta << "\n";
        std::cout << "   Fecha: " << registro_pendientes[i].fecha.dia << " de " << registro_pendientes[i].fecha.mes << ", " << registro_pendientes[i].fecha.year << "\n";

        std::cout << "   Nombre de cliente: " << registro_pendientes[i].nombre_cliente << "\n";
        std::cout << "   Monto a pagar: C$" << registro_pendientes[i].monto << "\n";

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


bool editar_pendiente() {
    limpiar_pantalla();
    int32_t indice = 0;
    int32_t indice_venta = 0;
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

    if (checkear_registro(num_pendientes)) {
        return true;
    }

    std::cout << "\n";
    pedir_str("ID del pago pendiente a editar", id);

    indice = buscar_pendiente(id);
    indice_venta = buscar_venta(id);

    set_color(Teal);
    std::cout << "\n   Buscando pago pendiente...";
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
    std::cout << "\n   Pago pendiente encontrado!";
    esperar(500);

    // mostrar datos del pendiente a editar:

    set_color(Cyan);
    std::cout << "\n\n                              Pago Pendiente #" << indice + 1 << ":\n";
    std::cout << "   ***********************************************************************";
    set_color(LightYellow);

    std::cout << "   ID: " << registro_pendientes[indice].id_venta << "\n";
    std::cout << "   Fecha: " << registro_pendientes[indice].fecha.dia << " de " << registro_pendientes[indice].fecha.mes << ", " << registro_pendientes[indice].fecha.year << "\n";
    std::cout << "   Nombre de cliente: " << registro_pendientes[indice].nombre_cliente << "\n";
    std::cout << "   Monto pendiente: C$" << registro_pendientes[indice].monto << "\n";

    do {
        set_color(LightYellow);
        std::cout << "\n\n   ¿Qué información quiere editar?\n";
        std::cout << "   1. Fecha\n";
        std::cout << "   2. Nombre del cliente\n";
        std::cout << "   3. Monto de la compra\n";
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
            case 1: {
                std::cout << "\n";
                registro_pendientes[indice].fecha = pedir_fecha();

                // también actualizar en ventas si existe
                if (indice_venta >= 0) {
                    registro_ventas[indice_venta].fecha = registro_pendientes[indice].fecha;
                }

                break;
            }

            case 2:
                while (true) {
                    pedir_str("nombre del cliente", temp_nombre);

                    if (buscar_cliente(temp_nombre) >= 0) {
                        set_color(Red);
                        std::cout << "   ERROR: Cliente debe estar registrado para editar el pago pendiente...";
                        reset_color();
                        esperar(1500);
                        return true;
                    }

                    registro_pendientes[indice].nombre_cliente = temp_nombre;

                    // también actualizar en ventas si existe
                    if (indice_venta >= 0) {
                        registro_ventas[indice_venta].nombre_cliente = registro_pendientes[indice].nombre_cliente;
                    }

                    break;
                }

                break;

            case 3:
                registro_pendientes[indice].monto = pedir_float("monto a pagar (en C$)");

                if (indice_venta >= 0) {
                    // actualizar venta correspondiente
                    registro_ventas[indice_venta].monto = registro_pendientes[indice].monto;

                    if (precio == 0.00) {
                        set_color(Red);
                        std::cout << "   ERROR: Precio por unidad no registrado...";
                        esperar(2250);
                        reset_color();
                        return true;
                    }

                    registro_ventas[indice_venta].cantidad_producto = registro_pendientes[indice].monto / precio;
                }

                break;

            default:
                set_color(Red);
                std::cout << "   Opción inválida...";
                esperar(1000);
                reset_color();
                break;
        }
    } while (info < 1 || info > 3);

    std::cout << "   ";
    esperar(500);
    set_color(Green);

    if (escribir_archivos(archivo_pendientes) && escribir_archivos(archivo_ventas)) {
        set_color(Green);
        std::cout << "\n   ***********************************************************************";
        std::cout << "\n                          Pago pendiente editado...";
    } else {
        return false;
    }

    esperar(2250);
    reset_color();
    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


int32_t eliminar_pendiente(const std::string &id) {
    int32_t indice = 0;
    int32_t indice_venta = 0;

    if (!leer_archivos(archivo_pendientes) || !leer_archivos(archivo_ventas)) {
        return -2;
    }

    if (checkear_registro(num_pendientes)) {
        return 1;
    }

    indice = buscar_pendiente(id);
    if (indice < 0) {
        return indice;
    }

    // si se va a eliminar un pago pendiente, es porque ya se pagó,
    // entonces hay que actualizar el registro de la venta correspondiente
    indice_venta = buscar_venta(registro_pendientes[indice].id_venta);

    if (indice_venta >= 0) {
        registro_ventas[indice_venta].pagada = true;
    } else {
        return indice_venta;
    }

    for (int32_t i = indice; i < num_pendientes - 1; i++) {
        registro_pendientes[i] = registro_pendientes[i + 1];
    }

    num_pendientes--;

    if (escribir_archivos(archivo_pendientes) && escribir_archivos(archivo_ventas)) {
        return 0;
    } else {
        return -2;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
