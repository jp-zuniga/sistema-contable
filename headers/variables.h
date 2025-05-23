////////////////////////////////////
// Variables globales del sistema //
////////////////////////////////////

#ifndef VARIABLES_H
#define VARIABLES_H

#define MAX_REGISTRO 1000

#include <cstdint>
#include <string>


///////////////////////////////////////////////////////////////////////////////////////////////////


typedef struct Fecha {
    int32_t dia = 0;
    std::string mes = "";
    int32_t year = 0;
} Fecha;


typedef struct Cliente {
    std::string id = "";
    std::string nombre = "";
    std::string contacto = "";
} Cliente;


typedef struct Venta {
    std::string id = "";
    Fecha fecha;
    std::string nombre_cliente = "";
    int32_t cantidad_producto = 0;
    float monto = 0.00;
    bool pagada = false;
} Venta;


typedef struct Pendiente {
    std::string id_venta = "";
    Fecha fecha;
    std::string nombre_cliente = "";
    float monto = 0.00;
} Pendiente;


typedef struct Costo {
    std::string id = "";
    float monto = 0.00;
    std::string descripcion = "";
    std::string mes = "";
} Costo;


///////////////////////////////////////////////////////////////////////////////////////////////////


inline float precio = 0.00;
inline float ingresos_totales = 0.00;
inline float costos_totales = 0.00;
inline float utilidad = 0.00;
inline float produccion_total = 0.00;


inline int32_t num_clientes = 0;
inline int32_t num_ventas = 0;
inline int32_t num_pendientes = 0;
inline int32_t num_costos_fijos = 0;
inline int32_t num_costos_variables = 0;


inline Cliente registro_clientes[MAX_REGISTRO];
inline Venta registro_ventas[MAX_REGISTRO];
inline Pendiente registro_pendientes[MAX_REGISTRO];
inline Costo registro_costos_fijos[MAX_REGISTRO];
inline Costo registro_costos_variables[MAX_REGISTRO];


///////////////////////////////////////////////////////////////////////////////////////////////////


const std::string archivo_precio = "datos/registro_precio.txt";
const std::string archivo_clientes = "datos/registro_clientes.txt";
const std::string archivo_ventas = "datos/registro_ventas.txt";
const std::string archivo_pendientes = "datos/registro_pendientes.txt";
const std::string archivo_costos_fijos= "datos/registro_costos_fijos.txt";
const std::string archivo_costos_variables= "datos/registro_costos_variables.txt";


const std::string meses[] = {
    "enero", "febrero", "marzo",
    "abril", "mayo", "junio",
    "julio", "agosto", "septiembre",
    "octubre", "noviembre", "diciembre"
};


///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
