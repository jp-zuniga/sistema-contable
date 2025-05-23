/////////////////////////////////////////////////////////////////
// Declaraciones de las funciones del módulo Gestión de Costos //
/////////////////////////////////////////////////////////////////

#ifndef COSTOS_H
#define COSTOS_H

#include <cstdint>
#include <string>


///////////////////////////////////////////////////////////////////////////////////////////////////


int32_t buscar_costo_fijo(const std::string &id);
int32_t buscar_costo_variable(const std::string &id);

bool registrar_costos_fijos();
bool registrar_costos_variables();

bool mostrar_costos_fijos();
bool mostrar_costos_variables();

bool editar_costo_fijo();
bool editar_costo_variable();

int32_t eliminar_costo_fijo(const std::string &id);
int32_t eliminar_costo_variable(const std::string &id);


///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
