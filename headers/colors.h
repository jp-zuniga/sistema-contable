/////////////////////////////////////////////////////////////////////////
// Basado en el trabajo de: Dani Meza (https://github.com/sea2horses). //
// Fuente: https://github.com/sea2horses/LemonLib/.                    //
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// El contenido de este archivo esta basado en código escrito por Dani Meza para su        //
// librería, LemonLib. Específicamente, el código de LemonLibCSB.h fue modificado          //
// para crear este header file. Su funcionalidad fue simplificada para los propósitos      //
// de este sistema, el cual solamente necesitaba poder darle color al texto de un terminal //
// (en este caso, con códigos ANSI). El namespace LLC, y las funcionalidades adicionales   //
// para formatear texto en negritas, itálicas, y más, no fueron incluidas.                 //
//     -- Joaquín Zúñiga, mayo 2024.                                                       //
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef COLORS_H
#define COLORS_H

#include <stdlib.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////////////////////////


enum Color
{
    Black = 0,
    Blue = 4,
    Gray = 8,
    White = 15,
    DarkBlue = 18,
    DarkGreen = 28,
    DarkAqua = 29,
    Teal = 33,
    Green = 46,
    Aqua = 47,
    Cyan = 51,
    DarkPurple = 55,
    DarkRed = 88,
    DarkPink = 125,
    Purple = 129,
    DarkYellow = 142,
    Red = 160,
    Pink = 197,
    DarkOrange = 202,
    Orange = 214,
    Yellow = 226,
    LightYellow = 228,
};


///////////////////////////////////////////////////////////////////////////////////////////////////


inline void set_color(Color c) {
    printf("\033[38;5;%dm", c);
}

inline void reset_color() {
    set_color(White);
}


///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
