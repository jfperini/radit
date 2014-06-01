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
#ifndef CURRENTSOUND_H
#define CURRENTSOUND_H

#include <QString>

class CurrentSound
{
    private:
        void CrearFichero(const QString url);

    public:
        CurrentSound();
        CurrentSound(const QString url);
        void SetCurrent(const QString url);
        virtual ~CurrentSound(){;}
};

#endif // CURRENTSOUND_H
