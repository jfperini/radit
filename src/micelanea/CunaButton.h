#ifndef CUNABUTTON_H
#define CUNABUTTON_H

#include <QDebug>
#include <QPushButton>
#include "engine/Pisadores.h"

class CunaButton : public QPushButton
{
    Q_OBJECT

    private:

    public:
        CunaButton(QWidget *parent = 0);
        virtual ~CunaButton(){;}
        Pisadores *SetPisadore;

    protected:

    private slots:

    signals:
        //void FinishCuna(QPushButton*);  //emite al finalizar
        void FinishCuna();  //emite al finalizar
};

#endif // CUNABUTTON_H
