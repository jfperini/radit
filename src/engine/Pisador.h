/**
 * Pisador
 * -----------------------------------------
 *
 * - This class extends QThread
 * - This class implements one pisador
 *      - up the level
 *      - down the level
 *
 * @author Victor Algaba
 */
#ifndef PISADOR_H
#define PISADOR_H

#include <QThread>
#include "bass.h"

class Pisador: public QThread
{
    public:
        Pisador(QObject *parent = 0);
        Pisador(HSTREAM Stream, bool Estado, QString  Prefijo, QObject *parent = 0);
        void run();
        bool estado;

        void setStream(HSTREAM Stream);
        void setEstado(bool Estado);
        void setPrefijo(QString  Prefijo);

    private:
        HSTREAM stream;
        int Transicion;
        float Locucion;
};

#endif // PISADOR_H
