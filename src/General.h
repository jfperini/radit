/**
 * General Setting
 * -----------------------------------------
 *
 * - Define view
 * - Set/get General Settings
 * - Define device of cue
 *
 * @author Victor Algaba
 */





#ifndef GENERAL_H
#define GENERAL_H


#include "ui_FrmGeneral.h"

class General : public QDialog, private Ui::FrmGeneral
{
    Q_OBJECT

    private:


    public:
        explicit General(QDialog *parent = 0);
        virtual ~General(){;}
        void SetPos(QRect scr);

    private slots:
        //void ClickSalir();
};




#endif // GENERAL_H
