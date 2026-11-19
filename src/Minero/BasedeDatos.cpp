#include "BasedeDatos.h"
#include <bits/stdc++.h>
#include <id3/misc_support.h>
#include <id3/tag.h>
using namespace std;

BasedeDatos::BasedeDatos(): bdd(nullptr){}
BasedeDatos::~BasedeDatos(){
  cierra();
}
void BasedeDatos:: cierra(){
  if(bdd != nullptr){
    sqlite3_close(bdd);
    bdd = nullptr;
  }
}

bool BasedeDatos:: conecta(const string& ruta){
  if(sqlite3_open(ruta.c_str(), &bdd) != SQLITE_OK){
    cout << "Algo salió mal al conectar: " << sqlite3_errmsg(bdd) << endl;
    cierra();
    return false;
  }
  sqlite3_exec(bdd, "PRAGMA foreign_keys = ON;", nullptr,nullptr, nullptr);
  return true;
}

bool BasedeDatos::getPerformer(const string& nombre,int id_type,int& id_performer){
  sqlite3_stmt* stmt = nullptr;
  //busca si existe ya 
  const char* sql = "SELECT id_performer "
    "FROM performers "
    "WHERE name = ? LIMIT 1;";
  if (sqlite3_prepare_v2(bdd, sql, -1, &stmt, nullptr)!= SQLITE_OK){
    cerr << sqlite3_errmsg(bdd) << endl;
    return false;
  }
  sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_TRANSIENT);
  int r = sqlite3_step(stmt);
  //por si si existe
  if (r == SQLITE_ROW) {
    id_performer = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return true;
  }
  sqlite3_finalize(stmt);
  if(r != SQLITE_DONE) {
    cerr << sqlite3_errmsg(bdd) << endl;
    return false;
  }
  //agregar si no existe
  const char* sqlAgregar= "INSERT INTO performers (id_type, name) "
    "VALUES (?, ?);";
  if(sqlite3_prepare_v2(bdd, sqlAgregar, -1, &stmt, nullptr)!= SQLITE_OK){
    cerr << sqlite3_errmsg(bdd) << endl;
    return false;
  }   
  sqlite3_bind_int(stmt, 1, id_type);
  sqlite3_bind_text(stmt, 2, nombre.c_str(), -1, SQLITE_TRANSIENT);
  bool correcto = sqlite3_step(stmt) == SQLITE_DONE;
  if(correcto){
    id_performer = static_cast<int>(sqlite3_last_insert_rowid(bdd));
  }    
  if(!correcto){
    cerr << sqlite3_errmsg(bdd) << endl;
  }
  sqlite3_finalize(stmt);
  return correcto;
}

bool BasedeDatos::getAlbum(const string& nombre,int year,int& id_album){
  sqlite3_stmt* stmt = nullptr;  
  if(nombre.empty()){
    id_album = 0;
    return true;
  }
  //busca con el nombre y año
  const char* sqlBuscar = "SELECT id_album "
    "FROM albums "
    "WHERE name = ? AND year = ? "
    "LIMIT 1;";
  if (sqlite3_prepare_v2(bdd, sqlBuscar, -1, &stmt, nullptr) != SQLITE_OK){
    cerr << sqlite3_errmsg(bdd) << endl;
    return false;
  }    
  sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 2, year);   
  int r = sqlite3_step(stmt);
  //si existe recuperar el id
  if(r == SQLITE_ROW){
    id_album = sqlite3_column_int(stmt, 0);      
    sqlite3_finalize(stmt);
    return true;
  }
  sqlite3_finalize(stmt);
  if(r != SQLITE_DONE) {      
    cerr << sqlite3_errmsg(bdd) << endl;
    return false;
  }
  // agrega si no existe
  const char* sqlInsertar =
    "INSERT INTO albums (path, name, year) "
    "VALUES (NULL, ?, ?);";    
  if(sqlite3_prepare_v2(bdd, sqlInsertar, -1, &stmt, nullptr) != SQLITE_OK) {
    cerr << sqlite3_errmsg(bdd) << endl;
    return false;
  }
  sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 2, year);
  bool correcto = sqlite3_step(stmt) == SQLITE_DONE;
  if(correcto){
    id_album = static_cast<int>(sqlite3_last_insert_rowid(bdd));
  }
  else{
    cerr << sqlite3_errmsg(bdd) << endl;
  }
  sqlite3_finalize(stmt);
  return correcto;
}


bool BasedeDatos:: agrega(const campoCancion& campo, const string& ruta){
  if(bdd == nullptr){
    cout << "Algo salió mal al intentar agregar" << endl;
    return false;
  }
  if(campo.path.empty()){
    cout << "Ruta vacía" << endl;
    return false;
  }
  
  if(sqlite3_exec(bdd, "BEGIN IMMEDIATE;",nullptr, nullptr, nullptr) != SQLITE_OK){
    cerr << sqlite3_errmsg(bdd) << endl;
    return false;
  }
  int id_performer = 0;
  int id_album = 0;
  //la tabla perfomer
  //que sea unknown pero creo que el 2 me lo puedo saltar
  if(!getPerformer(campo.performer, 2, id_performer)){
    sqlite3_exec(bdd, "ROLLBACK;", nullptr, nullptr, nullptr);
    return false;
  }
  //la tabla album
  if(!getAlbum(campo.album,campo.year, id_album)){
    sqlite3_exec(bdd, "ROLLBACK;", nullptr, nullptr, nullptr);
    return false;
  }
  //para agregar ahora si en rolas
  //ver si ya está usando path
  sqlite3_stmt* stmt = nullptr;
  const char* sqlBuscar = "SELECT id_rola FROM rolas WHERE path = ?;";
  //ver que no haya error
  if(sqlite3_prepare_v2(bdd, sqlBuscar, -1, &stmt, nullptr) != SQLITE_OK) {
    cerr << sqlite3_errmsg(bdd) << endl;
    sqlite3_exec(bdd, "ROLLBACK;", nullptr, nullptr, nullptr);
    return false;
  }
  sqlite3_bind_text(stmt, 1, campo.path.c_str(), -1, SQLITE_TRANSIENT);
  //ver que no exista ya la canción, si no existe termina ya
  int r = sqlite3_step(stmt);
  if(r == SQLITE_ROW){
    sqlite3_finalize(stmt);
    sqlite3_exec(bdd, "COMMIT;", nullptr, nullptr, nullptr);
    cout << "[Canción ya existente en la base de datos]" << endl;
    return true;
  }
  //buscar errores
  sqlite3_finalize(stmt);    
  if(r != SQLITE_DONE) {
    cerr << sqlite3_errmsg(bdd) << endl;

    sqlite3_exec(bdd, "ROLLBACK;", nullptr, nullptr, nullptr);
    return false;
  }
  const char* sql = "INSERT INTO rolas "
    "(id_performer, id_album, path, title, track, year, genre) "
    "VALUES (?, ?, ?, ?, ?, ?, ?);";
  
  if(sqlite3_prepare_v2(bdd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    cerr << sqlite3_errmsg(bdd) << endl;
    sqlite3_exec(bdd, "ROLLBACK;", nullptr, nullptr, nullptr);
    return false;
  }
  sqlite3_bind_int(stmt, 1, id_performer);
  sqlite3_bind_int(stmt, 2, id_album);

  sqlite3_bind_text(stmt, 3, campo.path.c_str(),-1, SQLITE_TRANSIENT); 
  sqlite3_bind_text(stmt, 4, campo.title.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 5, campo.track);
  sqlite3_bind_int(stmt, 6, campo.year);
  sqlite3_bind_text(stmt, 7, campo.genre.c_str(),-1, SQLITE_TRANSIENT);
  bool correcto = sqlite3_step(stmt) == SQLITE_DONE;
  sqlite3_finalize(stmt);
  if(!correcto) {
    cerr << sqlite3_errmsg(bdd) << endl;
    sqlite3_exec(bdd, "ROLLBACK;", nullptr, nullptr, nullptr);
    return false;
  }
  if(sqlite3_exec(bdd, "COMMIT;", nullptr, nullptr, nullptr)!= SQLITE_OK){
    cerr << sqlite3_errmsg(bdd) << endl;
    sqlite3_exec(bdd, "ROLLBACK;", nullptr, nullptr, nullptr);
    return false;
  }
  cout << "[se agregó a la base de datos] " << endl;  
  return true;
}

