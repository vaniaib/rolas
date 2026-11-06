#include "Filtro.h"
#include <bits/stdc++.h>
#include <id3/misc_support.h>
#include <id3/tag.h>
using namespace std;

bool Filtro::PrintInformation(const ID3_Tag &myTag){
  const ID3_Frame* titulo = myTag.Find(ID3FID_TITLE);
  cout << titulo << endl;
  // if(titulo != NULL){
  //   ID3_Field* texTitulo = titulo -> GetField(ID3FN_TEXT);
  //   if(texTitulo != NULL){
  //     char bufferTitulo[1024];
  //     texTitulo -> Get(bufferTitulo, sizeof(bufferTitulo));
  //     cout << "Titulo: " << bufferTitulo << endl;
  //   }
  // }else
  //   return false;
  ID3_Tag::ConstIterator* iter = myTag.CreateIterator();
  const ID3_Frame* frame = NULL;
  while ((frame = iter->GetNext()) != NULL){
    // const char* desc = frame->GetDescription();
    ID3_FrameID eFrameID = frame->GetID();
    switch (eFrameID){	
    case ID3FID_ALBUM:
      cout << "Album";
      break;
    case ID3FID_CONTENTTYPE:
      cout << "Genero: ";
      break;
    case ID3FID_TITLE:
      cout << "Titulo: ";
      break;
    case ID3FID_LEADARTIST:
      cout << "Artista principal: ";
      break;
    case ID3FID_BAND:
      cout << "Banda";
      break;
    case ID3FID_COMPOSER:
      cout << "Compositor: ";
      break;
    case ID3FID_ORIGARTIST:
      cout << "Artista original: ";
      break;
    case ID3FID_TRACKNUM:
      cout << "numero de cancion";
      break;
    case ID3FID_PARTINSET:
      cout << "Numero de disco: ";
      break;
    case ID3FID_YEAR:
      cout << "año";
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
    if(eFrameID != ID3FID_COMMENT){
      char* texto = ID3_GetString(frame, ID3FN_TEXT);
      cout << (texto ? texto : "") << '\n';
      delete[] texto;
    }
  }
  delete iter;
  return true;
}

bool Filtro::ejecuta(const string& filename){
  //ID3D_INIT_DOUT();
  ID3_Tag myTag;
  ifstream f(filename, ios::binary);

  cout << "Archivo abierto: " << f.is_open() << endl;
  cout << "Ruta: " << filename << endl;

  f.close();
  myTag.Link(filename.c_str(), ID3TT_ALL);//ó _ID3V2
  int i = myTag.NumFrames();
  cout << "Frames encontrados: "
       << i << endl;
  if(i == 0){
    return false;
  }
  // const Mp3_Headerinfo* mp3info;
  // mp3info = myTag.GetMp3HeaderInfo();
  cout << endl << "*** Tag information for " << filename << endl;
  PrintInformation(myTag);
  return true;
}
