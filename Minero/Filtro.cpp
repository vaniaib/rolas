#include "Filtro.h"
#include <bits/stdc++.h>
#include <id3/misc_support.h>
#include <id3/tag.h>
using namespace std;

bool Filtro::PrintInformation(const ID3_Tag &myTag){
  ID3_Tag::ConstIterator* iter = myTag.CreateIterator();
  const ID3_Frame* frame = NULL;
  int i = 0;
  while ((frame = iter->GetNext()) != NULL){
    i++;
    // const char* desc = frame->GetDescription();
    ID3_FrameID eFrameID = frame->GetID();
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
    case ID3FID_PICTURE:
      {
	char
	  *sMimeType = ID3_GetString(frame, ID3FN_MIMETYPE),
	  *sDesc     = ID3_GetString(frame, ID3FN_DESCRIPTION),
	  *sFormat   = ID3_GetString(frame, ID3FN_IMAGEFORMAT);
	size_t
	  nPicType   = frame->GetField(ID3FN_PICTURETYPE)->Get(),
	  nDataSize  = frame->GetField(ID3FN_DATA)->Size();
	cout << "(" << sDesc << ")[" << sFormat << ", "
	     << nPicType << "]: " << sMimeType << ", " << nDataSize
	     << " bytes" << endl;
	delete [] sMimeType;
	delete [] sDesc;
	delete [] sFormat;
	break;
      }
    default :
      continue;
      // case ID3FID_UNIQUEFILEID:
      //   {
      // 	char *sOwner = ID3_GetString(frame, ID3FN_OWNER);
      // 	size_t nDataSize = frame->GetField(ID3FN_DATA)->Size();
      // 	cout << sOwner << ", " << nDataSize
      // 	     << " bytes" << endl;
      // 	delete [] sOwner;
      // 	break;
      //   }
    }
    delete[] texto;
    if(eFrameID != ID3FID_COMMENT){
      char* texto = ID3_GetString(frame, ID3FN_TEXT);
      cout << (texto ? texto : "") << '\n';
      delete[] texto;
    }
    cout << "entra al while :"<< i << endl;
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
  //ID3D_INIT_DOUT();
  if(valida(filename)){
    cancion.path = filename;
    //cout << "Frames encontrados: " << i << endl;
    const ID3_Frame* titulo = myTag.Find(ID3FID_TITLE);
    if(titulo == NULL)
      cancion.title = nomCancion;
    else{
      char* narchivo = ID3_GetString(titulo, ID3FN_TEXT);
      cancion.title = narchivo;
      delete[] narchivo;
    }
    //   ID3_Field* texTitulo = titulo -> GetField(ID3FN_TEXT);
    //   if(texTitulo != NULL){
    //     char bufferTitulo[1024];
    //     texTitulo -> Get(bufferTitulo, sizeof(bufferTitulo));
    //     cout << "Titulo: " << bufferTitulo << endl;
    //   }
    // }else
    //   return false;
    //    cout << endl << "*** Tag information for " << filename << endl;
    cout << CYAN << "[AGREGADO]: "<< filename  << RESET << endl;
    // if(i == 0){
    //   return false;
    // }
    // }
    // const Mp3_Headerinfo* mp3info;
    // mp3info = myTag.GetMp3HeaderInfo();
    PrintInformation(myTag);
    BasedeDatos bdd;
    bdd.conecta("musica.db");
    bdd.agrega(cancion, filename);
    bdd.cierra();
    return true;
  }
  return false;
}
