/**
 * AddEventos
 * -----------------------------------------
 *
 * - View of about software
 * - This class represents a Dialog of new Event
 * - Has control of event add
 *
 * @author Victor Algaba
 */
#ifndef ADDEVENTOS_H
#define ADDEVENTOS_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include "ui_FrmAddEvento.h"
#include "AddHora.h"

class AddEventos : public QDialog, public Ui::FrmAddEvento
{
    Q_OBJECT

    private:
        // QSqlQuery query;
        QSqlDatabase db;
        AddHora * w_AddHora;

        void NuevoEvento();
        void UpdateEvento();

        void HoraTotabla(QTime hora, int cod, bool nuevo=true);
        void DiaTotabla(int cod, bool nuevo=true);
        void AddUrl(bool fichero);

        QString HorasToString();
        QString DiasToString();
        QString Url;

        bool ModoEdit; /// esta en modo de edicion
        int Cod;        // codigo del evento a modificar en modo edit

    public:
        explicit AddEventos(QSqlDatabase w_db , QWidget *parent = 0);
        virtual ~AddEventos(){;}
        QString Pestana;
        void PoderDatos(int cod);

    public slots:

    private slots:
        void OtrasHoras();
        void ClickAceptar();
        void ClickTipo(int);
        void ClickUrl();
        void HoratoCheckBox();
        void AddRadio();

    signals:
};
#endif // ADDEVENTOS_H
