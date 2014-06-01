/**
 * Pisadores
 * -----------------------------------------
 *
 * - ??
 *
 * @author Victor Algaba
 */
#ifndef PISADORES_H
#define PISADORES_H

//#include <QObject>
#include <QTime>
#include <QTimer>
#include <QPushButton>
#include "bass.h"
#include "engine/Hth.h"
//#include "micelanea/CunaButton.h"

class Pisadores: public QObject
{

    Q_OBJECT //attention!!!!! con esto habeces hace falta recompilar todo no vale con compilar

    private:
        QTimer *Timer;//timer count
        HSTREAM stream;
        Hth *w_Hth;
        bool IsHth;
        int HthTipo;
        int Dispositivo;
        QString Url;

    public:
        explicit Pisadores(QString url,QWidget *parent = 0);
        ~Pisadores();//free memory
        QString  Prefijo;
        void Configuracion(const QString Player);
        bool IsFinal(HSTREAM cual);  //checks if the end song
        void Start();//locuta el pisador
        QPushButton *boton;

    private slots:
        void Update();//process the time
        void EmitInicio();//process the time
        void EmitFinal();//process the time

    signals:
        void Finish();  //emite al finalizar
        void Iniciar();  //emite al iniciar
        void FinishCuna(QPushButton*);  //emite al finalizar
};
#endif // PISADORES_H
