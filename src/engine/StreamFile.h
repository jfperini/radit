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
#ifndef STREAMFILE_H
#define STREAMFILE_H
#include <QFileInfo>
#include <QString>
#include "bass.h"
//#include "bassflac.h"
//#include "bass_aac.h"



class StreamFile
{
    private:
        void EstableceStream(const QString url);
        void OnLine(const QString url);

    public:
        StreamFile();
        StreamFile(const QString url);
        void SetUrl(const QString url);
        HSTREAM stream;
        QString Titulo;//name song

        bool setTitulo;              //decide si establece el titulo
        bool IsRadioOnLine;
        virtual ~StreamFile(){;}
};
#endif // STREAMFILE_H
