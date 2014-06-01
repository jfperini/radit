/**
 * AcercaDe
 * -----------------------------------------
 *
 * - View of about software
 *
 * @author Victor Algaba
 */
#include "AcercaDe.h"
AcercaDe::AcercaDe(QDialog *parent )
    :QDialog(parent)
{
    setupUi(this);
    setModal(true);


    //set size views
    QSize fixedSize(this->width(),this->height());
    setMinimumSize(fixedSize);
    setMaximumSize(fixedSize);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}
