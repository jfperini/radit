/**
 * CurrentSound
 * -----------------------------------------
 *
 * - This class creates new file with information of the current song.
 * - file: CurrentSound.txt  folder: CurrentSound
 * - Out: title, artist, album,gender
 * - if don't have tag information , a out filename
 *
 * @author Victor Algaba
 */
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include "CurrentSound.h"
#include "TagInfo.h"

CurrentSound::CurrentSound(){}

CurrentSound::CurrentSound(const QString url)
{
    CrearFichero(url);
}

void CurrentSound::SetCurrent(const QString url)
{
    CrearFichero(url);
}

/**
 * This function creates new file with current song
 * @param QString url of file
 * @return void
 */
void CurrentSound::CrearFichero(const QString url)
{


   QString Path=QCoreApplication::applicationDirPath().toLatin1();
   QFile file(Path.toLatin1() + "/CurrentSound/CurrentSound.txt");
   file.open(QIODevice::WriteOnly| QIODevice::Text);
   QTextStream out(&file);

   TagInfo *w_TagInfo = new TagInfo(QDir::toNativeSeparators(url));
   QFileInfo File(url);


  /* if(w_TagInfo->getTitulo()!=""){
        out << w_TagInfo->getArtista();
        out << "  ";
        out << w_TagInfo->getTitulo();
   }else*/
        out << File.completeBaseName();

  // out << "\n";
 //  out << "  ";
   //out << w_TagInfo->getArtista();
 //  out << "\n";
 //  out << w_TagInfo->getAlbum();
 //  out << "\n";
 //  out << w_TagInfo->getGenero();
  // out << "\n";

   delete w_TagInfo;
   file.close();
}
