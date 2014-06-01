/**
 * Fader
 * -----------------------------------------
 *
 * - This class is responsible for generating fade in bass.h
 * - Generate fade Out
 *
 * @author Victor Algaba
 */


#ifndef FADER_H
#define FADER_H

#include <QThread>
#include "bass.h"

class Fader: public QThread
{
    Q_OBJECT

    public:
        Fader(QObject *parent = 0);
        Fader(HSTREAM Stream,int Fundir, QObject *parent = 0);

        void setStream(HSTREAM Stream);
        void setFundir(int Fundir);

        void run();

    private:
        HSTREAM stream;
        int Tiempo;

    signals:
};
#endif // FADER_H
