#ifndef MINERO_H
#define MINERO_H
#include<bits/stdc++.h>
#include "Lector.h"
using namespace std;
//Para leer el directorio y añadir a la base de datos con los mp3 válidos con ayuda de Filtro
class Minero{
 private:
  Lector lector;
  /* había puesto una string variable de clase ruta para hacer el minero con ese parámetro pero como puedo llamar varias veces al minero con diferengtes rutas, para no hacer uno nuevo cada vez que pase el parámetro en el método mina mejor*/
  //recorre el directorio y agrega los archivos a la base de datos
 public:
  Minero(){
  };
  void mina(const string& ruta);
};
#endif
