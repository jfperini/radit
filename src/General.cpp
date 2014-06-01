
/**
 * General Setting
 * -----------------------------------------
 *
 * - Define view
 * - Set/get General Settings
 * - Define device of cue etc
 *
 * @author Victor Algaba
 */




#include <QDebug>
#include "General.h"
#include "bass.h"

General::General(QDialog *parent )
    :QDialog(parent)
{
    setupUi(this);
    setModal(true);



    //define size of form
        QSize fixedSize(this->width(),this->height());
        setMinimumSize(fixedSize);
        setMaximumSize(fixedSize);
        setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

        listWidget->setCurrentRow(0);
       // stackedWidget->setCurrentIndex(0);


   // connect(BtnSalir,SIGNAL(clicked()),this, SLOT(ClickSalir()));

}
