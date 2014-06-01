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
#ifndef TAGINFO_H
#define TAGINFO_H

#include <QString>
#include "fileref.h"
#include "tag.h"

//#include "tbytevector.h"
//#include "taglib_export.h"

class TagInfo
{
    private:
        TagLib::FileRef tagFile;
        QString TagTiempo(int duracion );

    protected:

    public:
        TagInfo(const QString file);
        TagInfo();
        void setFile(const QString file);
        QString getArtista();
        QString getTitulo();
        QString getAlbum();
        QString getDuracion();
        QString getGenero();
};
#endif // TAGINFO_H
