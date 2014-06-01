/****************************************************************************
 **
 ** Copyright (C) 2010 Victor Algaba .
 ** All rights reserved.
 ** Contact: (www.radit.org)
 **
 ** *************************************************************************
 **Crea un fichero indice.dat
 **
 **
 ****************************************************************************/
#include <QDirIterator>
#include <QList>
#include <QDebug>

#include "AleatorioIndice.h"



AleatorioIndice::AleatorioIndice(const QString url){

    urlx=url;


    QFile fexiste( url + "/index.dat" );
      if(fexiste.exists())
        return;
      fexiste.close();






    QList<QString> list;

    QStringList filters;
    filters << "*.mp3" << "*.mp2" << "*.mp1" << "*.ogg" << "*.wav" << "*.aif" <<"*.wma"  <<"*.flac";
    QDirIterator it(url,filters, QDir::Files|QDir::NoSymLinks, QDirIterator::Subdirectories);


     while (it.hasNext()) {
          QString qstr = it.next();
          list << qstr ;

         }


     QFile file( url + "/index.dat" );
       if( !file.open( QIODevice::WriteOnly ) )
         return;

       QDataStream stream( &file );
       stream.setVersion( QDataStream::Qt_4_2 );

       stream << list;

       file.close();









}

//*******************************************************************************

AleatorioIndice::~AleatorioIndice(){



}

//*****************************************************************************
QDataStream AleatorioIndice::Load(){

    QList<QString> list;

    QFile file( urlx + "/test.dat");
    file.open( QIODevice::ReadOnly );

      QDataStream stream( &file );
      stream.setVersion( QDataStream::Qt_4_2 );

      stream >> list;

      file.close();


      //qDebug() << list[0];
      //qDebug() << list[1];







}




