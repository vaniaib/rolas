#ifndef FILTRO_H
#define FILTRO_H
#include "BasedeDatos.h"
#include<bits/stdc++.h>
#include <id3/misc_support.h>
#include <id3/tag.h>
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
#define RESET "\033[0m"
using namespace std;
//Para validar los mp3 y guardarlos
class Filtro{
public:
  campoCancion cancion;
  ID3_Tag myTag;
  Filtro(){};
  //imprime la info y ve q al menos tenga el titulo, si no no lo considera válido
  bool PrintInformation(const ID3_Tag &myTag);
  //valida que el archivo abra e imprime en rojo si no, si es válido continua
  bool valida(const string& filename);
  //lee y obtiene los datos
  bool ejecuta(const string& a, const string& b);
};
#endif
