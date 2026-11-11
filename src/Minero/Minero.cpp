#include<bits/stdc++.h>
#include "Lector.h"
using namespace std;
//solo manda a llamar al lector
int main(){
  string ruta;
  getline(cin, ruta);
  try{
    Lector lector;
    lector.leeDirectorio(ruta);
  }catch(exception& e){
    cout << e.what() << endl;
  }
}
