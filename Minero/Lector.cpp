#include<bits/stdc++.h>
#include <sys/stat.h>
#include "Lector.h"
using namespace std;
namespace fs = filesystem;
//Para leer el directorio y añadir a una lista los mp3 válidos
//recorre el directorio y agrega los archivos a la lista(vector)
void Lector::leeDirectorio(const string& r){
  fs::path ruta(r);
  try{
    int i = 0;
    for(const auto& f : fs::recursive_directory_iterator(ruta)){
      cout << f.path() << endl;
      string archivo = f.path().filename();
      if(filtro.ejecuta(archivo)){
      filtro.ejecuta(archivo);      
      i++;
      }else
	continue;
    }
    cout << "Se agregaron "<< i << "archivos";
  }catch(const fs::filesystem_error& e){
    cout << e.what() << endl;
  }
}


