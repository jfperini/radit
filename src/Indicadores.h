#ifndef INDICADORES_H
#define INDICADORES_H

#include "ui_FrmIndicadores.h"
#include <QTableWidget>
#include <QString>

class Indicadores : public QDialog, public Ui::FrmIndicadores
{
    Q_OBJECT

    private:

    public:
        explicit Indicadores(QWidget *parent = 0);
        virtual ~Indicadores(){;}
        void setAdd(const QString line);
        void UpdateLog(QString file);

    private slots:

};

#endif // INDICADORES_H
