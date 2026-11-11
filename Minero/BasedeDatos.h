#ifndef BASEDEDATOS_H
#define BASEDEDATOS_H
#include<bits/stdc++.h>
#include <sqlite3.h>
using namespace std;

//los diferentes datos que puede tener la etiquetea(solo puse los de la primera tabla)
struct campoCancion {
  string performer;
  string album;
  string path;
  string title;
  int track;
  int year;
  string genre;
};

//para agregar usando los datos de filtro
class BasedeDatos{
  
private:
  
  sqlite3* bdd;
  bool getPerformer(const string& nombre,int id_type, int& id_performer);
  bool getAlbum(const string& nombrealbum ,int year,int& id_album);
  
public:
  
  BasedeDatos();
  ~BasedeDatos();
  bool conecta(const string& ruta);
  //agrega los datos que puede tener la struct campoCancion
  bool agrega(const campoCancion& campo,const string& ruta);
  void cierra();
};
#endif
