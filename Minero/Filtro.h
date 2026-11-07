#ifndef FILTRO_H
#define FILTRO_H
#include<bits/stdc++.h>
#include <id3/misc_support.h>
#include <id3/tag.h>
#define AZUL    "\033[34m"
#define ROJO    "\033[31m"
#define RESET   "\033[0m"
using namespace std;
//Para validar los mp3 y guardarlos
class Filtro{
 public:
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
