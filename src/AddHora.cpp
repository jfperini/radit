/**
 * AddHora
 * -----------------------------------------
 *
 * - View hour of event
 * - Control with views
 * - Set check true/false
 *
 * @author Victor Algaba
 */
#include <QDebug>
#include "AddHora.h"

AddHora::AddHora( QWidget*parent )
    :QDialog(parent)
{
    setupUi(this);
    setModal(true);


    //set form view
    QSize fixedSize(this->width(),this->height());
    setMinimumSize(fixedSize);
    setMaximumSize(fixedSize);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    connect(BtnAceptar, SIGNAL(clicked()), this, SLOT(close()));//ok
    connect(BtnTodos, SIGNAL(clicked()), this, SLOT(Todos()));//all
    connect(BtnNinguna, SIGNAL(clicked()), this, SLOT(Ninguna()));//none
}

/**
 * This function set checked all hours TRUE on view
 * @brief AddHora::Ninguna
 */
void AddHora::Todos()
{
    this->H0->setChecked(true);
    this->H1->setChecked(true);
    this->H2->setChecked(true);
    this->H3->setChecked(true);
    this->H4->setChecked(true);
    this->H5->setChecked(true);
    this->H6->setChecked(true);
    this->H7->setChecked(true);
    this->H8->setChecked(true);
    this->H9->setChecked(true);
    this->H10->setChecked(true);
    this->H11->setChecked(true);
    this->H12->setChecked(true);
    this->H13->setChecked(true);
    this->H14->setChecked(true);
    this->H15->setChecked(true);
    this->H16->setChecked(true);
    this->H17->setChecked(true);
    this->H18->setChecked(true);
    this->H19->setChecked(true);
    this->H20->setChecked(true);
    this->H21->setChecked(true);
    this->H22->setChecked(true);
    this->H23->setChecked(true);
}

/**
 * This function set checked all hours FALSE on view
 * @brief AddHora::Ninguna
 */
void AddHora::Ninguna()
{
    this->H0->setChecked(false);
    this->H1->setChecked(false);
    this->H2->setChecked(false);
    this->H3->setChecked(false);
    this->H4->setChecked(false);
    this->H5->setChecked(false);
    this->H6->setChecked(false);
    this->H7->setChecked(false);
    this->H8->setChecked(false);
    this->H9->setChecked(false);
    this->H10->setChecked(false);
    this->H11->setChecked(false);
    this->H12->setChecked(false);
    this->H13->setChecked(false);
    this->H14->setChecked(false);
    this->H15->setChecked(false);
    this->H16->setChecked(false);
    this->H17->setChecked(false);
    this->H18->setChecked(false);
    this->H19->setChecked(false);
    this->H20->setChecked(false);
    this->H21->setChecked(false);
    this->H22->setChecked(false);
    this->H23->setChecked(false);
}
