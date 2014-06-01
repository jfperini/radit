/**
 * Cue
 * -----------------------------------------
 *
 * - This class Play sound in cue.
 *
 * @author Victor Algaba
 */
#ifndef CUE_H
#define CUE_H

//#include <QObject>
#include <QTimer>
#include <QString>
#include <QLabel>


#include "bass.h"

class Cue: public QObject
{
     Q_OBJECT

  private:
    HSTREAM stream;

  public:

        Cue();
        Cue(HSTREAM Stream);
        virtual ~Cue(){;}

        void setDevice(int device);
        void setStream(HSTREAM Stream);

        void Play();
        void Stop();

  private slots:

  signals:
};



#endif // CUE_H
