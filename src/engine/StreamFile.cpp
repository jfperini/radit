/**
 * StreamFile
 * -----------------------------------------
 *
 * - This class determines the type of file that will play
 *      - WAV
 *      - MP3
 *      - OGG
 *      - FLAC
 *      - WMA
 *
 * @author Victor Algaba
 */
#include <QDebug>
#include <QDir>
#include <QUrl>
#include <QCoreApplication>
#include "StreamFile.h"
#include "CurrentSound.h"

StreamFile::StreamFile()
{
    setTitulo = true;
}

StreamFile::StreamFile(const QString url)
{
    setTitulo = true;
    EstableceStream(QDir::toNativeSeparators(url));
}

/**
 * Set Url
 * @brief StreamFile::SetUrl
 * @param url of file
 * @return void
 */
void StreamFile::SetUrl(const QString url)
{
    EstableceStream(QDir::toNativeSeparators(url));
}

void StreamFile::EstableceStream(const QString url)
{
    QString w_url;
    w_url=QDir::toNativeSeparators(url);


    #ifdef Q_WS_X11   //linux
       w_url=QDir::toNativeSeparators(url.toUtf8());
    #endif

    QUrl RadioLine(w_url);

    if(RadioLine.scheme()=="http") //it is Radio on-line
    {
        OnLine(url);
        return;
    }


    stream=BASS_StreamCreateFile(FALSE, w_url.toLatin1(), 0, 0, BASS_STREAM_AUTOFREE); //checks if is a mp3,ogg,wav

    if(setTitulo)
    {
        //establecemos el título
        QFileInfo *w_File = new QFileInfo(url);
        //w_File->setFile(w_url);
        Titulo=w_File->completeBaseName(); // establece el titulo para sus diferentes usos

        CurrentSound *w_CurrentSound = new CurrentSound(w_url); //create a CurrentSound
        delete w_CurrentSound ;

        delete w_File;
    }

    IsRadioOnLine=false;
}

//////////////////////////////////////////////////////
void StreamFile::OnLine(const QString url)
{
    QString w_url=QDir::fromNativeSeparators(url);
    stream=BASS_StreamCreateURL(w_url.toLatin1(),0,BASS_STREAM_BLOCK|BASS_STREAM_STATUS|BASS_STREAM_AUTOFREE,NULL,0); // open URL

    const char *meta=BASS_ChannelGetTags(stream,BASS_TAG_META);
    TAG_ID3 *id3= (TAG_ID3*) BASS_ChannelGetTags(stream, BASS_TAG_META); //get the ID3 tags

    IsRadioOnLine=true;
    //char *p=strstr(meta," ");
    Titulo=Titulo.fromLatin1(meta);

    Titulo = Titulo.mid(13,25);
    QString y=Titulo.mid(13,25);

    //qDebug()  <<meta;
}
