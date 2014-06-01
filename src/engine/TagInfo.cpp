/**
 * TagInfo
 * -----------------------------------------
 *
 * - This class creates information tag
 * - Read music filename, and get tags like
 *      - artist
 *      - song
 *      - duration
 *      - album
 *      - duration
 *
 * @author Victor Algaba
 */
#include <QByteArray>
#include <QFile>
#include <QTime>
#include <QDir>
#include <math.h>
#include "TagInfo.h"

TagInfo::TagInfo(){}

TagInfo::TagInfo(const QString file)
{
    this->setFile(file);
}

void TagInfo::setFile(const QString file)
{
    QByteArray fileName = QFile::encodeName( QDir::toNativeSeparators(file));
    const char * encodedName = fileName.constData();
    tagFile = TagLib::FileRef(encodedName);
}

QString TagInfo:: getArtista()
{
    if(!tagFile.isNull())
        return(TStringToQString(tagFile.tag()->artist()));
    return("---");
}

QString TagInfo:: getTitulo()
{
    if(!tagFile.isNull())
        return(TStringToQString(tagFile.tag()->title()));
    return("----");
}

QString TagInfo:: getAlbum()
{
    if(!tagFile.isNull())
        return(TStringToQString(tagFile.tag()->album()));

    return("---");
}

QString TagInfo:: getGenero()
{
    if(!tagFile.isNull())
        return(TStringToQString(tagFile.tag()->genre()));

    return("---");
}

QString TagInfo:: getDuracion()
{
    if(!tagFile.isNull() && tagFile.audioProperties())
    {
        TagLib::AudioProperties *properties = tagFile.audioProperties();

        int seconds = properties->length();
        return(TagTiempo(seconds));
   }
    return("---");
}

QString TagInfo::TagTiempo(int duracion)
{
    long pos=duracion*1000;
    long sec = pos/1000;
    long min = sec/60;
    long hour = min/60;
    long msec = pos;

    QTime *playTime = new QTime((int) hour%60, (int) min%60, (int) sec%60, (int) msec%1000);

    return(playTime->toString("hh:mm:ss"));
}
