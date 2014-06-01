/**
 * PlayerSetting
 * -----------------------------------------
 *
 * - Define view
 * - Set/get Settings
 * - Define device of sound
 *
 * @author Victor Algaba
 */
#ifndef PLAYERSETTING_H
#define PLAYERSETTING_H

#include "ui_FrmSetting.h"

class PlayerSetting : public QDialog, private Ui::FrmSetting
{
    Q_OBJECT

    private:
        void AddDispositivo();
        void Establecer();
        QString w_Modulo;

    public:
        explicit PlayerSetting(QString Modulo,QDialog *parent = 0);
        virtual ~PlayerSetting(){;}
        void SetPos(QRect scr);

    private slots:
        void ClickSalir();
};
#endif // PLAYERSETTING_H
