#include <QDebug>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QUrl>
#include "IconoItem.h"

IconoItem::IconoItem(QTableWidgetItem *Cual,const QString url)
{
    QIcon icono;

    QFileInfo *Extension= new QFileInfo(url);
    QFileIconProvider *fIcon = new QFileIconProvider();

    //icono=fIcon->icon(*Extension);

    //qDebug() <<Extension->suffix().toLower();

    /// ********* FICHEROS AUDIO *******************************

    if(Extension->suffix().toLower()=="mp3") //toLower es miniscula
    icono.addFile(":/ficheros/iconos/ficheros/mp3.png");

    if(Extension->suffix().toLower()=="ogg") //toLower es miniscula
    icono.addFile(":/ficheros/iconos/ficheros/ogg.png");

    if(Extension->suffix().toLower()=="wav") //toLower es miniscula
    icono.addFile(":/ficheros/iconos/ficheros/wav.png");

    if(Extension->suffix().toLower()=="wma") //toLower es miniscula
    icono.addFile(":/ficheros/iconos/ficheros/wma.png");

    if(Extension->suffix().toLower()=="flac") //toLower es miniscula
    icono.addFile(":/ficheros/iconos/ficheros/flac.png");

    if(Extension->suffix().toLower()=="seq") //toLower es miniscula
    icono.addFile(":/ficheros/iconos/ficheros/rotacion.png");

    /// ***************** hth **********************************************

    if(Extension->suffix().toLower()=="hora") //toLower es miniscula
    icono.addFile(":/general/iconos/menu/relog.png");

    if(Extension->suffix().toLower()=="tempe") //toLower es miniscula
    icono.addFile(":/general/iconos/menu/sol.png");

    if(Extension->suffix().toLower()=="hume") //toLower es miniscula
    icono.addFile(":/general/iconos/menu/nube.png");

    /// ****************grupos*********************************************
    if(Extension->suffix().toLower()=="rot") //toLower es miniscula
    icono.addFile(":/general/iconos/ficheros/rot.png");

    if(Extension->suffix().toLower()=="seq") //toLower es miniscula
    icono.addFile(":/general/iconos/ficheros/rot.png");

    if(Extension->suffix().toLower()=="tad") //toLower es miniscula
    icono.addFile(":/general/iconos/ficheros/tanda.png");

    /// ***************lista********************************************

    if(Extension->suffix().toLower()=="lst") //toLower es miniscula
    icono.addFile(":/general/iconos/ficheros/tanda.png");


    /// *****************dir********************************************

    if(Extension->isDir())
       icono.addFile(":/ficheros/iconos/ficheros/carpeta.png");


    /// radio on-line****************************************************
    QUrl RadioLine(url);

     if(RadioLine.scheme()=="http")
        icono.addFile(":/ficheros/iconos/ficheros/online.png");

    Cual->setIcon(icono);

    delete Extension;
    delete fIcon;

    return;
}

IconoItem::~IconoItem(){}
