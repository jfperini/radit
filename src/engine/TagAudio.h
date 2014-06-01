#ifndef TAGAUDIO_H
#define TAGAUDIO_H

#include <QString>

#include "fileref.h"
#include "tag.h"
#include "tbytevector.h"
#include "taglib_export.h"


class TagAudio
{
    private:
QString TagTiempo(int duracion );


    protected:

    public:


    TagAudio();
    QString TagAlbum(QString file );
    QString TagArtista(QString file );
    QString TagTitulo(QString file );
    QString TagDuracion(QString file );



};



#endif // TAGAUDIO_H
