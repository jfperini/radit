/**
 * AcercaDe
 * -----------------------------------------
 *
 * - View of about software
 *
 * @author Victor Algaba
 */
#ifndef ACERCADE_H
#define ACERCADE_H

#include "ui_FrmAcercaDe.h"

class AcercaDe : public QDialog, private Ui::FrmAcercaDe
{
    Q_OBJECT

    private:

    public:
        explicit AcercaDe (QDialog *parent = 0);
        virtual ~AcercaDe (){;}

    private slots:
};
#endif // ACERCADE_H
