#ifndef EDITOREVENTOS_H
#define EDITOREVENTOS_H

#include <QSqlDatabase>
#include "ui_FrmEventosEdit.h"


class EditorEventos : public QDialog, private Ui::FrmEventosEdit
{
    Q_OBJECT

    private:
        QSqlDatabase db;
        void CrearPestanas();
        void ListarEventos(QString pestana);
        QString HoraToString();

    public:
        explicit EditorEventos(QSqlDatabase w_db,QWidget *parent = 0);
        virtual ~EditorEventos();

    public slots:

    private slots:
        void AddPestana();
        void DelPestana();

        void AddEvento();
        void EditEvento();
        void DelEvento();
        void DelExpirados();

        void ClickPestana(int);
        void ChecCambio(int,int);

    signals:
};
#endif // EDITOREVENTOS_H
