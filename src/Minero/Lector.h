#ifndef LECTOR_H
#define LECTOR_H
#include<bits/stdc++.h>
#include "Filtro.h"
using namespace std;
//Para leer el directorio y añadir a la base de datos con los mp3 válidos con ayuda de Filtro
class Lector{
private:
  Filtro filtro;
  //recorre el directorio y agrega los archivos a la base de datos
public:
  Lector(){};
  void leeDirectorio(const string& ruta);
};
#endif
