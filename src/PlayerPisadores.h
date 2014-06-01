/**
 * PlayerPisadores
 * -----------------------------------------
 *
 * - View of PlayerPisadores
 *
 * @author Victor Algaba
 **/
#ifndef PLAYERPISADORES_H
#define PLAYERPISADORES_H

#include "ui_FrmPisadores.h"
#include <QTableWidget>
#include <QString>

class PlayerPisadores : public QDialog, private Ui::FrmPisadores
{
    Q_OBJECT

    private:
        QTableWidget *Lista;
        QString Fichero();
        QString Carpeta();

        void setPidadorIn();
        void setPidadorOut();
        QRadioButton *Radio;

    public:
        explicit PlayerPisadores(QWidget *parent = 0);
        virtual ~PlayerPisadores(){;}
        void setLista(QTableWidget *w_Lista);

    private slots:
        void ClickFichero();
        void EstablecerIn();
        void EstablecerOut();
        void ClickSalir();
};
#endif // PLAYERPISADORES_H
