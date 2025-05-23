/////////////////////////////////////////////////////////////////////////////////
// Implementación de la funcionalidad de cálculos: ingresos, costos y utilidad //
/////////////////////////////////////////////////////////////////////////////////

#ifndef CALCULOS_CPP
#define CALCULOS_CPP

#include <cstdint>
#include <iostream>

#include "../headers/calculos.h"
#include "../headers/colors.h"
#include "../headers/util_funcs.h"
#include "../headers/variables.h"


///////////////////////////////////////////////////////////////////////////////////////////////////


bool calcular_ingresos() {
    /////////////////////////////////////////////////////////////////
    // Esto debería tener un paramétro de mes para permitirle al   //
    // usuario escoger para cual mes desean calcular los ingresos. //
    /////////////////////////////////////////////////////////////////

    if (!leer_archivos(archivo_ventas)) {
        return false;
    }

    if (checkear_registro(num_ventas)) {
        // no hay nada que calcular
        return true;
    }

    ingresos_totales = 0.00;
    int32_t mes_actual = obtener_fecha()->tm_mon;

    for (int32_t i = 0; i < num_ventas; i++) {
        if (
            registro_ventas[i].pagada
            &&
            registro_ventas[i].fecha.mes == meses[mes_actual]
        ) {
            // solo calcular ingresos del mes actual
            ingresos_totales += registro_ventas[i].monto;
        }
    }

    return true;
}


bool calcular_costos() {
    ///////////////////////////////////////////////////////////////
    // Esto debería tener un paramétro de mes para permitirle al //
    // usuario escoger para cual mes desean calcular los costos. //
    ///////////////////////////////////////////////////////////////

    if (
        !leer_archivos(archivo_costos_fijos)
         ||
        !leer_archivos(archivo_costos_variables)
    ) {
        return false;
    }

    if (num_costos_fijos == 0 && num_costos_variables == 0) {
        set_color(Red);
        std::cout << "   \nLos registros de costos estan vacíos...";
        reset_color();
        return true;
    }

    costos_totales = 0.00;
    int32_t mes_actual = obtener_fecha()->tm_mon;

    for (int32_t i = 0; i < num_costos_fijos; i++) {
        costos_totales += registro_costos_fijos[i].monto;
    }

    for (int32_t i = 0; i < num_costos_variables; i++) {
        if (registro_costos_variables[i].mes == meses[mes_actual]) {
            // solo sumar los costos del mes actual
            costos_totales += registro_costos_variables[i].monto;
        }
    }

    return true;
}


bool calcular_utilidad() {
    limpiar_pantalla();

    if (
        !leer_archivos(archivo_ventas) ||
        !leer_archivos(archivo_costos_fijos) ||
        !leer_archivos(archivo_costos_variables)
    ) {
        return false;
    }

    if (checkear_registro(num_ventas)) {
        return true;
    }

    if (num_costos_fijos == 0 && num_costos_variables == 0) {
        set_color(Red);
        std::cout << "\n   Los registros de costos estan vacíos...";
        reset_color();
        return true;
    }


    set_color(Teal);
    std::cout << "\n   Calculando...";
    esperar(1000);

    if (calcular_ingresos() && calcular_costos()) {
        // solo calcular si ambos cálculos funcionaron
        utilidad = ingresos_totales - costos_totales;
    } else {
        return false;
    }

    set_color(LightYellow);
    std::cout << "\n   Utilidad total mensual: ";

    // colorear output dependiendo de la utilidad
    if (utilidad < 0) {
        set_color(Red);
    } else if (utilidad > 0) {
        set_color(Green);
    } else {
        reset_color();
    }

    std::cout << "C$" << utilidad;
    set_color(Gray);

    std::cout << "\n\n   Presione 'Enter' para continuar...";
    std::cin.ignore();
    std::cin.get();

    reset_color();
    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
