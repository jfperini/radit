#ifndef EVENTOS_H
#define EVENTOS_H

#include <QMainWindow>
#include <QSqlDatabase>
#include "ui_FrmEventos.h"
#include "engine/Hth.h"

class Eventos : public QMainWindow, private Ui::FrmEventos
{
    Q_OBJECT

    public:
        Eventos( QMainWindow *parent = 0);
        virtual ~Eventos();

    private:
        QString Path;
        Hth *w_Hth;

        QSqlDatabase db;
        void setBase();
        void AddTablas();
        void ProximoEvento();
        void FechaHoyToBase(); ////pone la fecha de hoy en las horas
        void DeleteEnpuerta(); // borra los eventos en puerta
        void HoraEvento(); //es la hora de reproducir el evento
        void FaltaCinco(); // faltan cinco mintos para la reproduccion del evento
        void EsperaMaxima();
        bool IsEditor;  // si esta editando eventos paramos la actulizacion


    private slots:
        void UpdateHora();   //Procesa la hora
        void ClickHtm();
        void Final();
        void ClickEditor(); //activa el editor de eventos
        void ClickPlay();
        void setUpdateEventos();

    public slots:
        void BorrarEvento();

    protected:

    signals:
        //void startInmediato(QString);
        // void startRetardado(QString);
        void startEvento(QString,int,QString);
        void descartarEvento();

        //void startEvent();
        void startHth();   //se inicia la locucion hth
        void finalHth();   //finaliza la locucion hth
};
#endif // EVENTOS_H
