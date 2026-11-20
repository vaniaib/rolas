#include "Filtro.h"
#include <bits/stdc++.h>
#include <id3/misc_support.h>
#include <id3/tag.h>
namespace fs = filesystem;
using namespace std;

string Filtro::getrutabdd(){
  const char* home = getenv("HOME");
  if(home == nullptr){
    throw runtime_error("No se encontró HOME");
  }
  fs::path directorio = fs::path(home) / ".local/share/rolas";
  fs::create_directories(directorio);
  return (directorio / "rolas.db").string();
}

bool Filtro::PrintInformation(const ID3_Tag &myTag, const string& filename){
  ID3_Tag::ConstIterator* iter = myTag.CreateIterator();
  const ID3_Frame* frame = NULL;
  while ((frame = iter->GetNext()) != NULL){
    ID3_FrameID eFrameID = frame->GetID();
    fs::path ruta(filename);
    string nombre = cancion.album;
    	if(nombre.empty()){
	  nombre = ruta.parent_path().filename().string();
	  cancion.album = nombre;	 
    //para sacar la foto y tambien asignar lo del album si es null, si no se vuelve a asignar el nombre en el switch(eso está arriba jeje)
    if(eFrameID == ID3FID_PICTURE){
      ID3_Field* pField = frame->GetField(ID3FN_DATA);
      size_t nDataSize = pField->Size();
      char *sMimeType = ID3_GetString(frame, ID3FN_MIMETYPE);
      if(nDataSize > 0){
	vector<uchar> imageData(nDataSize);
        pField->Get(imageData.data(), nDataSize);
        string extension = "";
        if(sMimeType != nullptr && string(sMimeType) == "image/png")
	  extension = ".png";       
	else
	  extension = ".jpg";
        // fs::path ruta(filename);
	// string nombre = cancion.album;
	//aqui veo que sea null el album ys si es asigno el nombre del directorio donde está
	// if(nombre.empty()){
	//   nombre = ruta.parent_path().filename().string();
	//   cancion.album = nombre;
	//   cout << "no identificó el elbum y la carpeta es:" << nombre;
	// }else{
	//   cout << "si se identificó un album y es: " << nombre;
	// }
	const char* home = getenv("HOME");
	fs::path directorio = fs::path(home) /".local/share/rolas/imagenes";
	fs::create_directories(directorio);
	fs::path portada = directorio / (nombre + extension);
	if(fs::exists(portada)){
	  delete[] sMimeType;
	  break;
	}	
        ofstream imageFile(portada, ios::binary);
        if(imageFile.is_open())
	  imageFile.write(reinterpret_cast<const char*>(imageData.data()), nDataSize);	   
        imageFile.close();
      }
      delete[] sMimeType;
      //   continue;
    }
    char* texto = ID3_GetString(frame, ID3FN_TEXT);
    if(!texto)
      continue;
    switch (eFrameID){ 
    case ID3FID_ALBUM:
      cout << "Album: ";
      cancion.album = texto;
      break;
    case ID3FID_CONTENTTYPE:
      cout << "Genero: ";
      cancion.genre = texto;
      break;
    case ID3FID_TITLE:
      cout << "Titulo: ";
      break;
    case ID3FID_LEADARTIST:
      cout << "Artista principal: ";
      cancion.performer = texto;
      break;
    case ID3FID_BAND:
      cout << "Banda: ";
      break;
    case ID3FID_COMPOSER:
      cout << "Compositor: ";
      break;
    case ID3FID_ORIGARTIST:
      cout << "Artista original: ";
      break;
    case ID3FID_TRACKNUM:
      cout << "numero de cancion";
      cancion.track = atoi(texto);
      break;
    case ID3FID_PARTINSET:
      cout << "Numero de disco: ";
      break;
    case ID3FID_YEAR:
      cout << "Año: ";
      cancion.year = atoi(texto);
      break;
    case ID3FID_COMMENT:
      {
	char
	  *sText = ID3_GetString(frame, ID3FN_TEXT),
	  *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION);	 
	if (sDesc != nullptr && sDesc[0] != '\0')
	  cout << "(" << sDesc << ") "
	       << sText << endl;
	delete [] sText;
	delete [] sDesc;
	break;
      }
    default :
      continue;
    }
    delete[] texto;
    if(eFrameID != ID3FID_COMMENT){
      char* texto = ID3_GetString(frame, ID3FN_TEXT);
      cout << (texto ? texto : "") << '\n';
      delete[] texto;
    }
  }
  delete iter;
  return true;
}
bool Filtro:: valida(const string& filename){
  //primero vefr que abra
  ifstream f(filename, ios::binary);
  if(!f.is_open()){
    cout << MAGENTA << "[ERROR]: "<< "NO SE PUDO ABRIR EL ARCHIVO" << filename  << RESET << endl;
    f.close();
    return false;
  }
  f.close();
  //luego ver que tenga metadatos por leer con la etiqueta id3 , si tiene 0 es inválido
  //ID3_Tag myTag;
  myTag.Clear();
  myTag.Link(filename.c_str(), ID3TT_ALL);//ó _ID3V2
  int i = myTag.NumFrames();
  if(i == 0){
    cout << MAGENTA << "[ERROR]: "<< "NO HAY METADATOS QUE LEER " << filename  << RESET << endl;
    return false;
  }
  return true;
}
bool Filtro::ejecuta(const string& filename, const string& nomCancion){
  cancion = campoCancion{};
  //ID3D_INIT_DOUT();
  if(valida(filename)){
    cancion.path = filename;
    const ID3_Frame* titulo = myTag.Find(ID3FID_TITLE);
    if(titulo == NULL)
      cancion.title = nomCancion;
    else{
      char* narchivo = ID3_GetString(titulo, ID3FN_TEXT);
      cancion.title = narchivo;
      delete[] narchivo;
    }
    cout << CYAN << "[AGREGADO]: "<< filename  << RESET << endl;
    PrintInformation(myTag, filename);
    BasedeDatos bdd;    
    string rutaString = getrutabdd();
    bdd.conecta(rutaString);
    bdd.agrega(cancion, filename);
    bdd.cierra();
    return true;
  }
  return false;
}
