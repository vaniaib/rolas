#ifndef LECTOR_H
#define LECTOR_H
#include<bits/stdc++.h>
using namespace std;
//Para leer el directorio y añadir a una lista los mp3 válidos
class Lector{
  //vector para agregar los mp3 válidos
 private:
  vector<string> archivosValidos;
  //recorre el directorio y agrega los archivos a la lista(vector)
 public:
  Lector(){};
  void leeDirectorio(const string& ruta);
  };
#endif
