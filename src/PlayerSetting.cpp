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
#include <QDebug>
#include "PlayerSetting.h"
#include "bass.h"

PlayerSetting::PlayerSetting(QString Modulo,QDialog *parent )
    :QDialog(parent)
{
    setupUi(this);
    setModal(true);

    //define size of form
    QSize fixedSize(660,485);
    setMinimumSize(fixedSize);
    setMaximumSize(fixedSize);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    listWidget->setCurrentRow(0);
    stackedWidget->setCurrentIndex(0);

    this->setWindowTitle(this->windowTitle() + " [ " + Modulo.toLatin1() + " ]");

    connect(BtnSalir,SIGNAL(clicked()),this, SLOT(ClickSalir()));
    w_Modulo=Modulo;

    AddDispositivo();

    Establecer();
}

/**
 * ????
 * @brief PlayerSetting::SetPos
 * @param scr
 */
void PlayerSetting::SetPos(QRect scr)
{
    move( scr.center() - rect().center() );
}

/**
 * Define device sound
 * @brief PlayerSetting::AddDispositivo
 * @return void
 */
void PlayerSetting::AddDispositivo()
{
    BASS_DEVICEINFO i;
    QString nombre;

    QString nulo(tr("Sin Sonido")); //añadimos nulo
    this->Dispositivo->addItem(nulo);

    #ifdef Q_OS_UNIX
      //  QString def(tr("Defecto")); //en linux añadimos otro el default
      //  this->Dispositivo->addItem(def);
    #endif


 //****************** añadimos las tarjetas de audio *****************************
    for (int c=1;BASS_GetDeviceInfo(c,&i);c++)// device 1 = el primer dispositivo
    {
        if (i.flags&BASS_DEVICE_ENABLED)// enabled, lo añadimos...
                this->Dispositivo->addItem(nombre.fromAscii(i.name));
    }
}

/**
 * Set Settings
 * @brief PlayerSetting::Establecer
 * @return void
 */
void PlayerSetting::Establecer()
{
    QSettings RaditIni("Radit.ini", QSettings::IniFormat);

    Dispositivo->setCurrentIndex(RaditIni.value(w_Modulo + "/Dispositivo").toInt());
    Solapar->setValue(RaditIni.value(w_Modulo + "/Solapar").toInt());
    Descartar->setValue(RaditIni.value(w_Modulo + "/Descartar").toInt());
    Fundir->setValue(RaditIni.value(w_Modulo + "/Fundir").toInt());

    Qt::CheckState checkState;


    checkState =  static_cast<Qt::CheckState>(RaditIni.value(w_Modulo + "/FundirSolapar", Qt::Unchecked).toInt());
    FundirSolapar->setCheckState(checkState) ;

    checkState =  static_cast<Qt::CheckState>(RaditIni.value(w_Modulo + "/FundirParar", Qt::Unchecked).toInt());
    FundirParar->setCheckState(checkState) ;

    checkState =  static_cast<Qt::CheckState>(RaditIni.value(w_Modulo + "/Detector", Qt::Unchecked).toInt());
    Detector->setCheckState(checkState) ;

    DetectorNivel->setValue(RaditIni.value(w_Modulo + "/DetectorNivel").toInt());

    //Pisador
    Transicion->setValue(RaditIni.value(w_Modulo + "/Transicion").toInt());
    Locucion->setValue(RaditIni.value(w_Modulo + "/Locucion").toFloat());
}

/**
 * Exit and save settings
 * @brief PlayerSetting::ClickSalir
 * @return void
 */
void PlayerSetting::ClickSalir()
{
    QSettings RaditIni("Radit.ini", QSettings::IniFormat);

    RaditIni.setValue(w_Modulo + "/Dispositivo",this->Dispositivo->currentIndex());
    RaditIni.setValue(w_Modulo + "/Solapar",this->Solapar->value());
    RaditIni.setValue(w_Modulo + "/Descartar",this->Descartar->value());
    RaditIni.setValue(w_Modulo + "/Fundir",this->Fundir->value());

    RaditIni.setValue(w_Modulo + "/FundirSolapar",this->FundirSolapar->checkState());
    RaditIni.setValue(w_Modulo + "/FundirParar",this->FundirParar->checkState());
    RaditIni.setValue(w_Modulo + "/Detector",this->Detector->checkState());
    RaditIni.setValue(w_Modulo + "/DetectorNivel",this->DetectorNivel->value());

    //Pisador
    RaditIni.setValue(w_Modulo + "/Transicion",this->Transicion->value());
    RaditIni.setValue(w_Modulo + "/Locucion",this->Locucion->value());

    this->close();
}
