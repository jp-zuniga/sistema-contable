/////////////////////////////////////////////
// Implementación de los menús del sistema //
/////////////////////////////////////////////

#ifndef MENUS_CPP
#define MENUS_CPP

#include <cstdint>
#include <iostream>
#include <string>

#include "../headers/calculos.h"
#include "../headers/clientes.h"
#include "../headers/colors.h"
#include "../headers/costos.h"
#include "../headers/menus.h"
#include "../headers/pendientes.h"
#include "../headers/precio.h"
#include "../headers/titulos.h"
#include "../headers/util_funcs.h"
#include "../headers/variables.h"
#include "../headers/ventas.h"


///////////////////////////////////////////////////////////////////////////////////////////////////


int32_t inicio() {
    int32_t opcion = 0;

    while (true) {
        limpiar_pantalla();
        titulo_inicial();
        set_color(LightYellow);
        std::cout << "\n   1. Acceder a menú principal\n";
        set_color(Red);
        std::cout << "   2. Cerrar programa\n";
        set_color(Teal);
        std::cout << "\n   => Ingrese su opción: ";
        std::cin >> opcion;
        reset_color();

        if (std::cin.fail()) {
            menu_error();
            continue;
        } else {
            break;
        }
    }

    return opcion;
}

int32_t principal() {
    int32_t opcion = 0;
    int32_t submenu = 0;

    do {
        limpiar_pantalla();
        titulo_principal();
        set_color(LightYellow);
        std::cout << "   1. Acceder a Gestión de Clientes\n";
        std::cout << "   2. Acceder a Gestión de Finanzas\n";
        set_color(Cyan);
        std::cout << "\n   3. Regresar a pantalla principal\n";
        set_color(Teal);

        std::cout << "\n   => Ingrese su opción: ";
        std::cin >> opcion;
        reset_color();

        if (std::cin.fail()) {
            menu_error();
            continue;
        }

        switch (opcion) {
            case 1:
                set_color(LightYellow);
                std::cout << "      Accediendo a Gestión de Clientes...";
                reset_color();
                esperar(1000);

                submenu = clientes();
                if (submenu == 6) {
                    continue;
                }

                break;

            case 2:
                set_color(LightYellow);
                std::cout << "      Accediendo a Gestión de Finanzas...";
                reset_color();
                esperar(1000);

                submenu = finanzas();
                if (submenu == 9) {
                    continue;
                }

                break;

            case 3:
                set_color(Cyan);
                std::cout << "      Regresando a pantalla principal...";
                reset_color();
                esperar(1000);
                break;

            default:
                set_color(Red);
                std::cout << "      Opción inválida...";
                reset_color();
                esperar(1000);
                break;
        }
    } while (opcion != 3);

    reset_color();
    return opcion;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


int32_t clientes() {
    int32_t opcion = 0;
    int32_t indice = 0;
    std::string input_id = "";

    do {
        limpiar_pantalla();
        titulo_clientes();
        set_color(LightYellow);
        std::cout << "\n   1. Registrar clientes\n";
        std::cout << "   2. Mostrar clientes\n";
        std::cout << "   3. Buscar cliente\n";
        std::cout << "   4. Modificar datos de cliente\n";
        std::cout << "   5. Eliminar cliente\n";

        set_color(Cyan);
        std::cout << "\n   6. Regresar a menú principal\n";
        set_color(Teal);

        std::cout << "\n   => Ingrese su opción: ";
        std::cin >> opcion;
        reset_color();

        if (std::cin.fail()) {
            menu_error();
            continue;
        }

        switch (opcion) {
            case 1:
                if (!registrar_clientes()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo abrir, leer y/o escribir al archivo 'registro_clientes.txt'...";
                } else {
                    set_color(Green);
                    std::cout << "\n   ***********************************************************************";
                    std::cout << "\n                          Cliente(s) registrado(s)...";
                }

                reset_color();
                esperar(2250);
                break;

            case 2:
                if (!mostrar_clientes()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_clientes.txt'...";
                    reset_color();
                    esperar(2250);
                }

                break;

            case 3:
                limpiar_pantalla();
                std::cout << "\n";
                pedir_str("ID o nombre del cliente a buscar", input_id);

                if (leer_archivos(archivo_clientes)) {
                    // solo buscar el id si se pudo leer el archivo
                    indice = buscar_cliente(input_id);
                } else {
                    indice = -2;
                }

                set_color(Teal);
                std::cout << "\n   Buscando cliente...";
                reset_color();
                esperar(800);

                if (indice == -1) {
                    set_color(Red);
                    std::cout << "\n   ERROR: el dato '" << input_id << "' no se encontró en el registro...";
                    reset_color();
                    esperar(2250);
                    break;
                }

                else if (indice == -2) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_clientes.txt'...";
                    reset_color();
                    esperar(2250);
                    break;
                }

                set_color(Green);
                std::cout << "\n   Cliente encontrado!";
                esperar(800);

                set_color(Cyan);
                std::cout << "\n\n                                  Cliente #" << indice + 1 << ":" << "\n";
                std::cout << "   ---------------------------------------------------------------------\n";

                set_color(LightYellow);
                std::cout << "   ID: " << registro_clientes[indice].id << "\n";
                std::cout << "   Nombre: " << registro_clientes[indice].nombre << "\n";
                std::cout << "   Contacto: " << registro_clientes[indice].contacto << "\n";
                set_color(Cyan);

                std::cout << "\n   ---------------------------------------------------------------------\n";
                std::cout << "   Presione 'Enter' para continuar...";
                std::cin.get();
                limpiar_buffer();

                reset_color();
                break;

            case 4:
                if (!editar_cliente()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_clientes.txt'...";
                    reset_color();
                    esperar(2250);
                }

                break;

            case 5:
                limpiar_pantalla();
                std::cout << "\n";
                pedir_str("ID del cliente a eliminar", input_id);
                indice = eliminar_cliente(input_id);

                set_color(Teal);
                std::cout << "\n   Buscando cliente...";
                reset_color();
                esperar(800);

                if (indice == -1) {
                    set_color(Red);
                    std::cout << "\n   ERROR: el ID '" << input_id << "' no se encontró en el registro...";
                    reset_color();
                    esperar(1000);
                    break;
                } else if (indice == -2) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_clientes.txt'...";
                    reset_color();
                    esperar(1000);
                    break;
                }

                set_color(Green);
                std::cout << "\n   Cliente eliminado!";

                reset_color();
                esperar(1500);
                break;

            case 6:
                set_color(Cyan);
                std::cout << "      Regresando a menú principal...";
                reset_color();
                esperar(1000);
                break;

            default:
                set_color(Red);
                std::cout << "      Opción inválida...";
                reset_color();
                esperar(1000);
                break;
        }
    } while (opcion != 6);

    reset_color();
    return opcion;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


int32_t finanzas() {
    int32_t opcion = 0;
    int32_t submenu = 0;

    do {
        limpiar_pantalla();
        titulo_finanzas();
        set_color(LightYellow);
        std::cout << "\n   1. Acceder a menú de ventas\n";
        std::cout << "   2. Acceder a menú de pagos pendientes\n";
        std::cout << "   3. Acceder a menú de costos fijos\n";
        std::cout << "   4. Acceder a menú de costos variables\n";

        set_color(Green);
        std::cout << "\n   5. Calcular ingresos totales\n";
        set_color(Red);
        std::cout << "   6. Calcular costos totales\n";
        reset_color();
        std::cout << "   7. Calcular utilidad\n";
        set_color(LightYellow);
        std::cout << "\n   8. Mostrar precio\n";
        std::cout << "   9. Cambiar precio\n";

        set_color(Cyan);
        std::cout << "\n   10. Regresar a menú principal\n";
        set_color(Teal);

        std::cout << "\n   => Ingrese su opción: ";
        std::cin >> opcion;
        reset_color();

        if (std::cin.fail()) {
            menu_error();
            continue;
        }

        // esto es por si se escogió entrar a los menús de costos fijos/variables y se
        // quiere mover directamente entre los dos sin tener que regresar al menú de finanzas
        while (opcion == 3 || opcion == 4) {
            if (opcion == 3) {
                set_color(LightYellow);
                std::cout << "      Accediendo a submenú de costos fijos...";
                reset_color();
                esperar(1000);

                submenu = costos_fijos();
                if (submenu == 6) {
                    // ir al menú de costos variables
                    opcion = 4;
                }
            }

            else if (opcion == 4) {
                set_color(LightYellow);
                std::cout << "      Accediendo a submenú de costos variables...";
                reset_color();
                esperar(1000);

                submenu = costos_variables();
                if (submenu == 6) {
                    // ir al menú de costos fijos
                    opcion = 3;
                }
            }

            // si se escogió regresar al menú de finanzas, salir del
            // loop, y entrar al caso correspondiente del switch
            if (submenu == 7) {
                set_color(Cyan);
                std::cout << "      Regresando a menú de finanzas...";
                reset_color();
                esperar(1000);
                break;
            }

            // si se escogió regresar al menú principal directamente desde
            // los menús de costos fijos/variables, opcion toma el valor de este menú
            // que nos regresa a al menú principal y nos saca de este while y el do while
            else if (submenu == 8) {
                opcion = 10;
            }
        }

        // resolver el resto de los casos del menu con el switch
        switch (opcion) {
            case 1:
                set_color(LightYellow);
                std::cout << "      Accediendo a menú de ventas...";
                reset_color();
                esperar(1000);

                submenu = ventas();
                if (submenu == 6) {
                    // si el submenú retorna la opción de regresar al
                    // menú de finanzas, ir a la próxima iteración
                    continue;
                } else if (submenu == 7) {
                    // retorna la opción que hace que termine el
                    // do while y se regrese al menú principal
                    return 10;
                }

                break;

            case 2:
                set_color(LightYellow);
                std::cout << "      Accediendo a menú de pagos pendientes...";
                reset_color();
                esperar(1000);

                submenu = pendientes();
                if (submenu == 5) {
                    continue;
                } else if (submenu == 6) {
                    return 10;
                }

                break;

            // para que el switch no se vaya al caso default si opcion es 3 o 4
            case 3: break;
            case 4: break;

            case 5:
                limpiar_pantalla();

                if (!calcular_ingresos()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer uno o más de los siguientes archivos: 'registro_ventas.txt', 'registro_pendientes.txt'...";
                    reset_color();
                }

                // calcular_ingresos() se encarga de informarle al usuario si hubo un error
                // aqui solo hay que encargarse de mostrar el output si se pudo hacer el cálculo
                if (ingresos_totales != 0.00) {
                    set_color(Teal);
                    std::cout << "\n   Calculando...";
                    esperar(1000);

                    set_color(LightYellow);
                    std::cout << "\n   Ingresos totales mensuales: ";
                    set_color(Green);

                    std::cout << "C$" << ingresos_totales;
                    set_color(Gray);
                    std::cout << "\n\n   Presione 'Enter' para continuar...";
                    std::cin.get();

                    reset_color();
                    limpiar_buffer();
                }

                break;

            case 6:
                limpiar_pantalla();

                if (!calcular_costos()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer uno o más de los siguientes archivos: 'costos_fijos.txt', 'costos_variables.txt'...";
                    reset_color();
                }

                // lo mismo que con los ingresos
                if (costos_totales != 0.00) {
                    set_color(Teal);
                    std::cout << "\n   Calculando...";
                    esperar(1000);

                    set_color(LightYellow);
                    std::cout << "\n   Costos totales mensuales: ";
                    set_color(Red);

                    std::cout << "C$" << costos_totales;
                    set_color(Gray);
                    std::cout << "\n\n   Presione 'Enter' para continuar...";
                    std::cin.get();

                    reset_color();
                    limpiar_buffer();
                }

                break;

            case 7:
                // la misma función valida si no se pudo calcular
                if (!calcular_utilidad()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer uno o más de los siguientes archivos: 'registro_ventas.txt', 'registro_pendientes.txt', 'costos_fijos.txt', 'costos_variables.txt'...";
                    reset_color();
                }

                break;

            case 8:
                if (!mostrar_precio()) {
                   esperar(500);
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_precio.txt'...";
                    reset_color();
                }

                esperar(2250);
                break;

            case 9:
                if (!registrar_precio()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo escribir el precio ingresado al archivo 'registro_precio.txt'...";
                    reset_color();
                }

                esperar(2250);
                break;

            case 10:
                set_color(Cyan);
                std::cout << "      Regresando a menú principal...";
                reset_color();
                esperar(1000);
                break;

            default:
                set_color(Red);
                std::cout << "      Opción inválida...";
                reset_color();
                esperar(1000);
                break;
        }
    } while (opcion != 10);

    reset_color();
    return opcion;
}


int32_t ventas() {
    int32_t opcion = 0;
    int32_t indice = 0;
    std::string input_id = "";

    do {
        limpiar_pantalla();
        titulo_ventas();
        set_color(LightYellow);
        std::cout << "\n   1. Registrar ventas\n";
        std::cout << "   2. Mostrar ventas\n";
        std::cout << "   3. Buscar venta\n";
        std::cout << "   4. Modificar venta\n";
        std::cout << "   5. Eliminar venta\n";

        set_color(Cyan);
        std::cout << "\n   6. Regresar a menú de finanzas\n";
        std::cout << "   7. Regresar a menú principal\n";
        set_color(Teal);

        std::cout << "\n   => Ingrese su opción: ";
        std::cin >> opcion;
        reset_color();

        if (std::cin.fail()) {
            menu_error();
            continue;
        }

        switch (opcion) {
            case 1:
                if (!registrar_ventas()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo abrir, leer y/o escribir a uno o más los siguientes archivos: 'registro_ventas.txt', 'registro_pendientes.txt', 'registro_clientes.txt', 'registro_precio.txt'...";
                } else {
                    set_color(Green);
                    std::cout << "\n   ***********************************************************************";
                    std::cout << "\n                           Venta(s) registrada(s)...";
                }

                reset_color();
                esperar(2250);
                break;

            case 2:
                if (!mostrar_ventas()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_ventas.txt'...";
                    reset_color();
                    esperar(2250);
                }

                break;

            case 3:
                limpiar_pantalla();
                std::cout << "\n";
                pedir_str("ID de la venta a buscar", input_id);

                if (leer_archivos(archivo_ventas)) {
                    indice = buscar_venta(input_id);
                } else {
                    indice = -2;
                }

                set_color(Teal);
                std::cout << "\n   Buscando venta...";
                reset_color();
                esperar(800);

                if (indice == -1) {
                    set_color(Red);
                    std::cout << "\n   ERROR: el ID '" << input_id << "' no se encontró en el registro...";
                    reset_color();
                    esperar(2250);
                    break;
                }

                else if (indice == -2) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_ventas.txt'...";
                    reset_color();
                    esperar(2250);
                    break;
                }

                set_color(Green);
                std::cout << "\n   Venta encontrada!";
                esperar(800);

                set_color(Cyan);
                std::cout << "\n\n                                  Venta #" << indice + 1 << ":\n";
                std::cout << "   ---------------------------------------------------------------------\n";

                set_color(LightYellow);
                std::cout << "   ID: " << registro_ventas[indice].id << "\n";
                std::cout << "   Fecha: " << registro_ventas[indice].fecha.dia << " de " << registro_ventas[indice].fecha.mes << ", " << registro_ventas[indice].fecha.year << "\n";
                std::cout << "   Nombre del cliente: " << registro_ventas[indice].nombre_cliente << "\n";
                std::cout << "   Cantidad comprada: " << registro_ventas[indice].cantidad_producto << " unidades\n";
                std::cout << "   Monto: C$" << registro_ventas[indice].monto << "\n";

                std::cout << "   ¿Está pagada? ";
                set_color(Teal);
                std::cout << (registro_ventas[indice].pagada ? "Sí" : "No") << "\n";
                set_color(Cyan);

                std::cout << "\n   ---------------------------------------------------------------------\n";
                std::cout << "   Presione 'Enter' para continuar...";
                std::cin.get();
                limpiar_buffer();

                reset_color();
                break;

            case 4:
                if (!editar_venta()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo abrir, leer y/o escribir a uno o más de los siguientes archivos: 'registro_ventas.txt', 'registro_pendientes.txt'...";
                    reset_color();
                    esperar(2250);
                }

                break;

            case 5:
                limpiar_pantalla();
                std::cout << "\n";
                pedir_str("ID de la venta a eliminar", input_id);
                indice = eliminar_venta(input_id);

                set_color(Teal);
                std::cout << "\n   Buscando venta...";
                reset_color();
                esperar(800);

                if (indice == -1) {
                    set_color(Red);
                    std::cout << "\n   ERROR: el ID '" << input_id << "' no se encontró en el registro...";
                    reset_color();
                    break;
                } else if (indice == -2) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo abrir, leer y/o escribir a uno o más los siguientes archivos: 'registro_ventas.txt', 'registro_pendientes.txt', 'registro_precio.txt...";
                    reset_color();
                    break;
                }

                set_color(Green);
                std::cout << "\n   Venta eliminada!";

                reset_color();
                esperar(1500);
                break;

            case 6:
                set_color(Cyan);
                std::cout << "      Regresando a menú de finanzas...";
                reset_color();
                esperar(1000);
                break;

            case 7:
                set_color(Cyan);
                std::cout << "      Regresando a menú principal...";
                reset_color();
                esperar(1000);
                break;

            default:
                set_color(Red);
                std::cout << "      Opción inválida...";
                reset_color();
                esperar(1000);
                break;
        }
    // mientras no se quiera regresar a otro menu, permanecer en el loop
    } while (opcion != 6 && opcion != 7);

    reset_color();
    return opcion;
}


int32_t pendientes() {
    int32_t opcion = 0;
    int32_t indice = 0;
    std::string input_id = "";

    do {
        limpiar_pantalla();
        titulo_pendientes();
        set_color(LightYellow);
        std::cout << "\n   1. Mostrar pagos pendientes\n";
        std::cout << "   2. Buscar pago pendiente\n";
        std::cout << "   3. Modificar pago pendiente\n";
        std::cout << "   4. Eliminar pago pendiente\n";

        set_color(Cyan);
        std::cout << "\n   5. Regresar a menú de finanzas\n";
        std::cout << "   6. Regresar a menú principal\n";
        set_color(Teal);

        std::cout << "\n   => Ingrese su opción: ";
        std::cin >> opcion;
        reset_color();

        if (std::cin.fail()) {
            menu_error();
            continue;
        }

        switch (opcion) {
            case 1:
                if (!mostrar_pendientes()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_pendientes.txt'...";
                    reset_color();
                    esperar(2250);
                }

                break;

            case 2:
                limpiar_pantalla();
                std::cout << "\n";
                pedir_str("ID del pago pendiente a buscar", input_id);

                if (leer_archivos(archivo_pendientes)) {
                    indice = buscar_pendiente(input_id);
                } else {
                    indice = -2;
                }

                set_color(Teal);
                std::cout << "\n   Buscando pago pendiente...";
                reset_color();
                esperar(800);

                if (indice == -1) {
                    set_color(Red);
                    std::cout << "\n   ERROR: el ID '" << input_id << "' no se encontró en el registro...";
                    reset_color();
                    esperar(2250);
                    break;
                } else if (indice == -2) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_pendientes.txt'...";
                    reset_color();
                    esperar(2250);
                    break;
                }

                set_color(Green);
                std::cout << "\n   Pago pendiente encontrado!";
                esperar(800);

                set_color(Cyan);
                std::cout << "\n\n                                  Pago pendiente #" << indice + 1 << ":\n";
                std::cout << "   ---------------------------------------------------------------------\n";

                set_color(LightYellow);
                std::cout << "   ID de la venta: " << registro_pendientes[indice].id_venta << "\n";
                std::cout << "   Fecha: " << registro_pendientes[indice].fecha.dia << " de " << registro_pendientes[indice].fecha.mes << ", " << registro_pendientes[indice].fecha.year << "\n";
                std::cout << "   Nombre del cliente: " << registro_pendientes[indice].nombre_cliente << "\n";
                std::cout << "   Monto: C$" << registro_pendientes[indice].monto << "\n";
                set_color(Cyan);

                std::cout << "\n   ---------------------------------------------------------------------\n";
                std::cout << "   Presione 'Enter' para continuar...";
                std::cin.get();
                limpiar_buffer();

                reset_color();
                break;

            case 3:
                if (!editar_pendiente()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer uno o más de los siguientes archivos: 'registro_pendientes.txt', 'registro_ventas.txt...";
                    reset_color();
                    esperar(2250);
                }

                break;

            case 4:
                limpiar_pantalla();
                std::cout << "\n";
                pedir_str("ID del pago pendiente a eliminar", input_id);
                indice = eliminar_pendiente(input_id);

                set_color(Teal);
                std::cout << "\n   Buscando pago pendiente...";
                reset_color();
                esperar(800);

                if (indice == -1) {
                    set_color(Red);
                    std::cout << "\n   ERROR: el ID '" << input_id << "' no se encontró en el registro...";
                    reset_color();
                    break;
                } else if (indice == -2) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo abrir/leer uno o más de los siguientes archivos: 'registro_pendientes.txt', 'registro_ventas.txt', 'registro_precio.txt'...";
                    reset_color();
                    break;
                }

                set_color(Green);
                std::cout << "\n   Pago pendiente eliminado!";

                reset_color();
                esperar(1500);
                break;

            case 5:
                set_color(Cyan);
                std::cout << "      Regresando a menú de finanzas...";
                reset_color();
                esperar(1000);
                break;

            case 6:
                set_color(Cyan);
                std::cout << "      Regresando a menú principal...";
                reset_color();
                esperar(1000);
                break;

            default:
                set_color(Red);
                std::cout << "      Opción inválida...";
                reset_color();
                esperar(1000);
                break;
        }
    } while ((opcion != 5) && (opcion != 6));

    reset_color();
    return opcion;
}


int32_t costos_fijos() {
    int32_t opcion = 0;
    int32_t indice = 0;
    std::string input_id = "";

    do {
        limpiar_pantalla();
        titulo_costos_fijos();
        set_color(LightYellow);
        std::cout << "\n   1. Registrar costos\n";
        std::cout << "   2. Mostrar costos\n";
        std::cout << "   3. Buscar costo\n";
        std::cout << "   4. Modificar costo\n";
        std::cout << "   5. Eliminar costo\n";
        set_color(Cyan);
        std::cout << "\n   6. Ir a menú de costos variables\n";
        std::cout << "   7. Regresar a menú de finanzas\n";
        std::cout << "   8. Regresar a menú principal\n";
        set_color(Teal);
        std::cout << "\n   => Ingrese su opción: ";
        std::cin >> opcion;
        reset_color();

        if (std::cin.fail()) {
            menu_error();
            continue;
        }

        switch (opcion) {
            case 1:
                if (!registrar_costos_fijos()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo abrir, leer y/o escribir al archivo 'registro_costos_fijos.txt'...";
                } else {
                    set_color(Green);
                    std::cout << "\n   ***********************************************************************";
                    std::cout << "\n                          Costo(s) registrado(s)...";
                }

                reset_color();
                esperar(2250);
                break;

            case 2:
                if (!mostrar_costos_fijos()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_costos_fijos.txt'...";
                    reset_color();
                    esperar(2250);
                }

                break;

            case 3:
                limpiar_pantalla();
                std::cout << "\n";
                pedir_str("ID del costo fijo a buscar", input_id);

                if (leer_archivos(archivo_costos_fijos)) {
                    indice = buscar_costo_fijo(input_id);
                } else {
                    indice = -2;
                }

                set_color(Teal);
                std::cout << "\n   Buscando costo fijo...";
                reset_color();
                esperar(800);

                if (indice == -1) {
                    set_color(Red);
                    std::cout << "\n   ERROR: el ID '" << input_id << "' no se encontró en el registro...";
                    reset_color();
                    esperar(2250);
                    break;
                } else if (indice == -2) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_costos_fijos.txt'...";
                    reset_color();
                    esperar(2250);
                    break;
                }

                set_color(Green);
                std::cout << "\n   Costo fijo encontrado!";
                esperar(800);

                set_color(Cyan);
                std::cout << "\n\n                                  Costo Fijo #" << indice + 1 << ":\n";
                std::cout << "   ---------------------------------------------------------------------\n";

                set_color(LightYellow);
                std::cout << "   ID: " << registro_costos_fijos[indice].id << "\n";
                std::cout << "   Monto: C$" << registro_costos_fijos[indice].monto << "\n";
                std::cout << "   Descripción: " << registro_costos_fijos[indice].descripcion << "\n";
                set_color(Cyan);

                std::cout << "\n   ---------------------------------------------------------------------\n";
                std::cout << "   Presione 'Enter' para continuar...";
                std::cin.get();
                limpiar_buffer();

                reset_color();
                break;

            case 4:
                if (!editar_costo_fijo()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_costos_fijos.txt'...";
                    reset_color();
                    esperar(2250);
                }

                break;

            case 5:
                limpiar_pantalla();
                std::cout << "\n";
                pedir_str("ID del costo fijo a eliminar", input_id);
                indice = eliminar_costo_fijo(input_id);

                set_color(Teal);
                std::cout << "\n   Buscando costo fijo...";
                reset_color();
                esperar(800);

                if (indice == -1) {
                    set_color(Red);
                    std::cout << "\n   ERROR: el ID '" << input_id << "' no se encontró en el registro...";
                    reset_color();
                    break;
                } else if (indice == -2) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_costos_fijos.txt'...";
                    reset_color();
                    break;
                }

                set_color(Green);
                std::cout << "\n   Costo fijo eliminado!";

                reset_color();
                esperar(1500);
                break;

            // los mensajes correspondientes se hacen en finanzas()
            case 6: break;
            case 7: break;
            case 8: break;

            default:
                set_color(Red);
                std::cout << "      Opción inválida...";
                reset_color();
                esperar(1000);
                break;
        }
    // mientras no se quiera salir del submenú, quedarse en el do while
    } while ((opcion < 6) || (opcion > 8));

    reset_color();
    return opcion;
}


int32_t costos_variables() {
    int32_t opcion = 0;
    int32_t indice = 0;
    std::string input_id = "";

    do {
        limpiar_pantalla();
        titulo_costos_variables();
        set_color(LightYellow);
        std::cout << "\n   1. Registrar costos\n";
        std::cout << "   2. Mostrar costos\n";
        std::cout << "   3. Buscar costo\n";
        std::cout << "   4. Modificar costo\n";
        std::cout << "   5. Eliminar costo\n";
        set_color(Cyan);
        std::cout << "\n   6. Ir a menú de costos fijos\n";
        std::cout << "   7. Regresar a menú de finanzas\n";
        std::cout << "   8. Regresar a menú principal\n";
        set_color(Teal);
        std::cout << "\n   => Ingrese su opción: ";
        std::cin >> opcion;
        reset_color();

        if (std::cin.fail()) {
            menu_error();
            continue;
        }

        switch (opcion) {
            case 1:
                if (!registrar_costos_variables()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo abrir, leer y/o escribir al archivo 'registro_costos_variables.txt'...";
                } else {
                    set_color(Green);
                    std::cout << "\n   ***********************************************************************";
                    std::cout << "\n                           Costo(s) registrado(s)...";
                }

                reset_color();
                esperar(2250);
                break;

            case 2:
                if (!mostrar_costos_variables()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_costos_variables.txt'...";
                    reset_color();
                    esperar(2250);
                }

                break;

            case 3:
                limpiar_pantalla();
                std::cout << "\n";
                pedir_str("ID del costo variable a buscar", input_id);

                if (leer_archivos(archivo_costos_variables)) {
                    indice = buscar_costo_variable(input_id);
                } else {
                    indice = -2;
                }

                set_color(Teal);
                std::cout << "\n   Buscando costo variable...";
                reset_color();
                esperar(800);

                if (indice == -1) {
                    set_color(Red);
                    std::cout << "\n   ERROR: el ID '" << input_id << "' no se encontró en el registro...";
                    reset_color();
                    esperar(2250);
                    break;
                } else if (indice == -2) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_costos_variables.txt'...";
                    reset_color();
                    esperar(2250);
                    break;
                }

                set_color(Green);
                std::cout << "\n   Costo variable encontrado!";
                esperar(800);

                set_color(Cyan);
                std::cout << "\n\n                               Costo Variable #" << indice + 1 << ":\n";
                std::cout << "   ---------------------------------------------------------------------\n";

                set_color(LightYellow);
                std::cout << "   ID: " << registro_costos_variables[indice].id << "\n";
                std::cout << "   Monto: C$" << registro_costos_variables[indice].monto << "\n";
                std::cout << "   Descripción: " << registro_costos_variables[indice].descripcion << "\n";
                std::cout << "   Mes: " << registro_costos_variables[indice].mes << "\n";
                set_color(Cyan);

                std::cout << "\n   ---------------------------------------------------------------------\n";
                std::cout << "   Presione 'Enter' para continuar...";
                std::cin.get();
                limpiar_buffer();

                reset_color();
                break;

            case 4:
                if (!editar_costo_variable()) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_costos_variables.txt'...";
                    reset_color();
                    esperar(2250);
                }

                break;

            case 5:
                limpiar_pantalla();
                std::cout << "\n";
                pedir_str("ID del costo variable a eliminar", input_id);
                indice = eliminar_costo_variable(input_id);

                set_color(Teal);
                std::cout << "\n   Buscando costo variable...";
                reset_color();
                esperar(800);

                if (indice == -1) {
                    set_color(Red);
                    std::cout << "\n   ERROR: el ID '" << input_id << "' no se encontró en el registro...";
                    reset_color();
                    break;
                } else if (indice == -2) {
                    set_color(Red);
                    std::cout << "\n   ERROR DE ARCHIVO: no se pudo leer el archivo 'registro_costos_variables.txt'...";
                    reset_color();
                    break;
                }

                set_color(Green);
                std::cout << "\n   Costo variable eliminado!";

                reset_color();
                esperar(1500);
                break;

            // los mensajes correspondientes se hacen en finanzas()
            case 6: break;
            case 7: break;
            case 8: break;

            default:
                set_color(Red);
                std::cout << "      Opción inválida...";
                reset_color();
                esperar(1000);
                break;
        }
    } while ((opcion < 6) || (opcion > 8));

    reset_color();
    return opcion;
}


///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
