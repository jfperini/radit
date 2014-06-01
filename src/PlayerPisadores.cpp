/**
 * PlayerPisadores
 * -----------------------------------------
 *
 * - View of PlayerPisadores
 *
 * @author Victor Algaba
 **/
#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include "PlayerPisadores.h"

PlayerPisadores::PlayerPisadores(QWidget *parent )
    :QDialog(parent)
{
    setupUi(this);
    setModal(true);

    //setWindowFlags( Qt::SubWindow);

    //evitamos que cambie el formulario
    QSize fixedSize(458,488);
    setMinimumSize(fixedSize);
    setMaximumSize(fixedSize);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    //ficheros botones
    connect(BtnInFichero,SIGNAL(clicked()),this, SLOT(ClickFichero()));
    connect(BtnOutFichero,SIGNAL(clicked()),this, SLOT(ClickFichero()));

    connect(BtnInCarpeta,SIGNAL(clicked()),this, SLOT(ClickFichero()));
    connect(BtnOutCarpeta,SIGNAL(clicked()),this, SLOT(ClickFichero()));

    //radios botones
    connect(RInFichero,SIGNAL(clicked()),this, SLOT(EstablecerIn()));
    connect(RInCarpeta,SIGNAL(clicked()),this, SLOT(EstablecerIn()));
    connect(RInHora,SIGNAL(clicked()),this, SLOT(EstablecerIn()));
    connect(RInTemperatura,SIGNAL(clicked()),this, SLOT(EstablecerIn()));
    connect(RInHumedad,SIGNAL(clicked()),this, SLOT(EstablecerIn()));

    connect(ROutFichero,SIGNAL(clicked()),this, SLOT(EstablecerOut()));
    connect(ROutCarpeta,SIGNAL(clicked()),this, SLOT(EstablecerOut()));
    connect(ROutHora,SIGNAL(clicked()),this, SLOT(EstablecerOut()));
    connect(ROutTemperatura,SIGNAL(clicked()),this, SLOT(EstablecerOut()));
    connect(ROutHumedad,SIGNAL(clicked()),this, SLOT(EstablecerOut()));

    connect(BtnSalir,SIGNAL(clicked()),this, SLOT(ClickSalir()));
}

void PlayerPisadores::setLista(QTableWidget *w_Lista)
{
    Lista=w_Lista;

    TTitulo->setText(Lista->item(Lista->currentRow(),1)->text());

    setPidadorIn();
    setPidadorOut();
}

void PlayerPisadores::ClickFichero()
{
    QObject* obj = QObject::sender();
    QPushButton *Boton = qobject_cast<QPushButton *>(obj);

    if(Boton == this->BtnInFichero)
    {
        LInFichero->setText(Fichero());
    }

    if(Boton == this->BtnInCarpeta)
    {
        LInCarpeta->setText(Carpeta());
    }

    if(Boton == this->BtnOutFichero)
    {
        LOutFichero->setText(Fichero());
    }

    if(Boton == this->BtnOutCarpeta)
    {
        LOutCarpeta->setText(Carpeta());
    }
}

QString PlayerPisadores::Fichero()
{
    QSettings RaditIni("Radit.ini", QSettings::IniFormat);
    QString   Dir=QDir::toNativeSeparators(RaditIni.value("General/Path").toString());

    QString fileName = QFileDialog::getOpenFileName(0,
                                                    QObject::tr("Abrir PlayList"),
                                                    Dir,
                                                    QObject::tr("Sound file (*.wav *.mp2 *.mp3 *.ogg *.flac *.wma);;"
                                                    ));
    if (fileName.isNull())
        return("");

    QFileInfo File(fileName);

    return(File.absoluteFilePath());
}

QString PlayerPisadores::Carpeta()
{
    QSettings RaditIni("Radit.ini", QSettings::IniFormat);
    QString   Dir=QDir::toNativeSeparators(RaditIni.value("General/Path").toString());

    QString dir = QFileDialog::getExistingDirectory(this, tr("Abrir Carpeta"),
                                                    Dir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if (dir.isNull())
        return("");

    QFileInfo File(dir);

    return(File.absoluteFilePath());
}

void PlayerPisadores::EstablecerIn()
{
    QObject* obj = QObject::sender();
    QRadioButton *w_Radio = qobject_cast<QRadioButton *>(obj);

    if(w_Radio == this->RInFichero)//que bonito comprobando dos punteros, (no si son iguales) si es el mismo
    {
        LInFichero->setEnabled(true);
        BtnInFichero->setEnabled(true);
        LInCarpeta->setEnabled(false);
        BtnInCarpeta->setEnabled(false);
        LInCarpeta->setText("");
    }

    if(w_Radio == this->RInCarpeta)//que bonito comprobando dos punteros, (no si son iguales) si es el mismo
    {
        LInFichero->setEnabled(false);
        BtnInFichero->setEnabled(false);
        LInCarpeta->setEnabled(true);
        BtnInCarpeta->setEnabled(true);
        LInFichero->setText("");
    }

    //que bonito comprobando dos punteros, (no si son iguales) si es el mismo
    if(w_Radio == this->RInHora || w_Radio == this->RInHumedad ||w_Radio == this->RInTemperatura)
    {
        LInFichero->setEnabled(false);
        BtnInFichero->setEnabled(false);
        LInCarpeta->setEnabled(false);
        BtnInCarpeta->setEnabled(false);

        LInFichero->setText("");
        LInCarpeta->setText("");
    }
}

void PlayerPisadores::EstablecerOut()
{
    QObject* obj = QObject::sender();
    QRadioButton *w_Radio = qobject_cast<QRadioButton *>(obj);

    //que bonito comprobando dos punteros, (no si son iguales) si es el mismo
    if(w_Radio == this->ROutFichero)
    {
        LOutFichero->setEnabled(true);
        BtnOutFichero->setEnabled(true);
        LOutCarpeta->setEnabled(false);
        BtnOutCarpeta->setEnabled(false);
        LOutCarpeta->setText("");
    }

    //que bonito comprobando dos punteros, (no si son iguales) si es el mismo
    if(w_Radio == this->ROutCarpeta)
    {
        LOutFichero->setEnabled(false);
        BtnOutFichero->setEnabled(false);
        LOutCarpeta->setEnabled(true);
        BtnOutCarpeta->setEnabled(true);
        LOutFichero->setText("");
    }

    //que bonito comprobando dos punteros, (no si son iguales) si es el mismo
    if(w_Radio == this->ROutHora || w_Radio == this->ROutHumedad ||w_Radio == this->ROutTemperatura)
    {
        LOutFichero->setEnabled(false);
        BtnOutFichero->setEnabled(false);
        LOutCarpeta->setEnabled(false);
        BtnOutCarpeta->setEnabled(false);

        LOutFichero->setText("");
        LOutCarpeta->setText("");
    }
}

/**
 * Set data of Pisador
 * @brief PlayerPisadores::setPidadorIn
 */
void PlayerPisadores::setPidadorIn()
{
    int Tiempo =Lista->item(Lista->currentRow(), 5)->text().toInt();
    this->Segundos->setValue(Tiempo);

    QString PisadorTipo;
    PisadorTipo = Lista->item(Lista->currentRow(), 6)->text(); //

    QFileInfo File(PisadorTipo.toLatin1());

    if(File.suffix().toLower()=="hora")
    {
        RInHora->click();
        return;
    }

    if(File.suffix().toLower()=="tempe")
    {
        RInTemperatura->click();
        return;
    }

    if(File.suffix().toLower()=="hume")
    {
        RInHumedad->click();
        return;
    }

    if(File.isDir())
    {
        RInCarpeta->click();
        LInCarpeta->setText(PisadorTipo.toLatin1());
        return;
    }

    if(File.isFile())
    {
        RInFichero->click();
        LInFichero->setText(PisadorTipo.toLatin1());
        return;
    }
}

void PlayerPisadores::setPidadorOut()
{
    QString PisadorTipo;
    PisadorTipo = Lista->item(Lista->currentRow(), 7)->text(); //

    QFileInfo File(PisadorTipo.toLatin1());

    if(File.suffix().toLower()=="hora")
    {
        ROutHora->click();
        return;
    }

    if(File.suffix().toLower()=="tempe")
    {
        ROutTemperatura->click();
        return;
    }

    if(File.suffix().toLower()=="hume")
    {
        ROutHumedad->click();
        return;
    }

    if(File.isDir())
    {
        ROutCarpeta->click();
        LOutCarpeta->setText(PisadorTipo.toLatin1());
        return;
    }

    if(File.isFile())
    {
        ROutFichero->click();
        LOutFichero->setText(PisadorTipo.toLatin1());
        return;
    }
}

/**
 * Exit of view and save data
 * @brief PlayerPisadores::ClickSalir
 * @return void
 */
void PlayerPisadores::ClickSalir()
{
    if(RInHora->isChecked())
    {
        Lista->setItem(Lista->currentRow(), 5, new  QTableWidgetItem (Segundos->text())); //Segundos locucion
        Lista->setItem(Lista->currentRow(), 6, new  QTableWidgetItem ("Locucion Hora.hora")); //urlIN
    }

    if(RInTemperatura->isChecked())
    {
        Lista->setItem(Lista->currentRow(), 5, new  QTableWidgetItem (Segundos->text())); //Segundos locucion
        Lista->setItem(Lista->currentRow(), 6, new  QTableWidgetItem ("Locucion Temperatura.tempe")); //urlIN
    }

    if(RInHumedad->isChecked())
    {
        Lista->setItem(Lista->currentRow(), 5, new  QTableWidgetItem (Segundos->text())); //Segundos locucion
        Lista->setItem(Lista->currentRow(), 6, new  QTableWidgetItem ("Locucion Humedad.hume")); //urlIN
    }

    if(RInFichero->isChecked())
    {
        Lista->setItem(Lista->currentRow(), 5, new  QTableWidgetItem (Segundos->text())); //Segundos locucion
        Lista->setItem(Lista->currentRow(), 6, new  QTableWidgetItem (LInFichero->text())); //urlIN
    }

    if(RInCarpeta->isChecked())
    {
        Lista->setItem(Lista->currentRow(), 5, new  QTableWidgetItem (Segundos->text())); //Segundos locucion
        Lista->setItem(Lista->currentRow(), 6, new  QTableWidgetItem (LInCarpeta->text())); //urlIN
    }

    ///out
    if(ROutHora->isChecked())
    {
         Lista->setItem(Lista->currentRow(), 7, new  QTableWidgetItem ("Locucion Hora.hora")); //urlIN
    }

    if(ROutTemperatura->isChecked())
    {
        Lista->setItem(Lista->currentRow(), 7, new  QTableWidgetItem ("Locucion Temperatura.tempe")); //urlIN
    }

    if(ROutHumedad->isChecked())
    {
         Lista->setItem(Lista->currentRow(), 7, new  QTableWidgetItem ("Locucion Humedad.hume")); //urlIN
    }

    if(ROutFichero->isChecked())
    {
        Lista->setItem(Lista->currentRow(), 7, new  QTableWidgetItem (LOutFichero->text())); //urlIN
    }

    if(ROutCarpeta->isChecked())
    {
        Lista->setItem(Lista->currentRow(), 7, new  QTableWidgetItem (LOutCarpeta->text())); //urlIN
    }

    this->close();
}
