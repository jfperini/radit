/**
 * AddRadioInternet
 * -----------------------------------------
 *
 * - Control with views
 * - Work with radio online
 *
 * @author Victor Algaba
 */
#include <QDebug>
#include "AddRadioInternet.h"

AddRadioInternet::AddRadioInternet( QWidget*parent )
    :QDialog(parent)
{
    setupUi(this);
    setModal(true);



    //set form size
    QSize fixedSize(this->width(),this->height());
    setMinimumSize(fixedSize);
    setMaximumSize(fixedSize);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    connect(BtnCancelar,SIGNAL(clicked()),this, SLOT(ClickCancelar())); //cancel
    connect(BtnAceptar,SIGNAL(clicked()),this, SLOT(ClickAceptar())); //ok
    connect(BtnPlay,SIGNAL(clicked()),this, SLOT(ClickPlay())); //play
    connect(BtnStop,SIGNAL(clicked()),this, SLOT(ClickStop())); //stop
}

/**
 * Free in memory
 * @brief AddRadioInternet::~AddRadioInternet
 */
AddRadioInternet::~AddRadioInternet()
{
    BASS_StreamFree(stream);
}

void AddRadioInternet::ClickAceptar()
{
    BASS_StreamFree(stream);
    Url=EditUrl->text().toLatin1();
    Duracion = TimeDuracion->text().toLatin1();
    Nombre=EditNombre->text().toLatin1();

    if(Url.isEmpty() || Nombre.isEmpty())
        return;

    this->close();
}

/**
 * Go play radio ???? this a preview?
 * @brief AddRadioInternet::ClickPlay
 */
void AddRadioInternet::ClickPlay()
{
    BASS_SetDevice(Dispositivo);//dispositivo
    BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST,1);// enable playlist processing
    BASS_StreamFree(stream);
    StreamFile *w_StreamFile= new StreamFile(EditUrl->text().toLatin1());

    stream=w_StreamFile->stream;
    delete w_StreamFile;

    BASS_ChannelPlay(stream, false);
}

void AddRadioInternet::ClickStop()
{
    BASS_ChannelStop(stream);
}

void AddRadioInternet::ClickCancelar()
{
    BASS_ChannelStop(stream);
    BASS_StreamFree(stream);
    this->close();
}
