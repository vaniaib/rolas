#include<bits/stdc++.h>
#include <sys/stat.h>
#include "Lector.h"
using namespace std;
namespace fs = filesystem;
//Para leer el directorio
//recorre el directorio y agrega los archivos a la base de datos usando el filtro
void Lector::leeDirectorio(const string& r){
  fs::path ruta(r);
  try{
    int i = 0;
    for(const auto& f : fs::recursive_directory_iterator(ruta)){
      // cout << f.path() << endl;
      string archivo = fs::absolute(f.path().string());
      string nomCancion = f.path().filename();
      if(filtro.ejecuta(archivo, nomCancion)){  
      i++;
      }else
	continue;
    }
    cout << "Se agregaron "<< i << " archivos";
  }catch(const fs::filesystem_error& e){
    cout << e.what() << endl;
  }
}


