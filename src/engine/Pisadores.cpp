/**
 * Pisadores
 * -----------------------------------------
 *
 * - ??
 *
 * @author Victor Algaba
 */
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include "Pisadores.h"
#include "StreamMath.h"
#include "StreamFile.h"
#include "micelanea/Aleatorio.h"
#include "engine/Hth.h"

Pisadores::Pisadores(QString url,QWidget *parent):
    QObject(parent)
{
    Timer = new QTimer();
    connect(Timer, SIGNAL(timeout()), this, SLOT(Update())); // temporizaor horario

    QFileInfo w_FileInfo(url);

    IsHth=false;
    HthTipo=0;

    if(w_FileInfo.suffix().toLower()=="hora" || w_FileInfo.suffix().toLower()=="tempe" ||
       w_FileInfo.suffix().toLower()=="hume" )
    {
        if(w_FileInfo.suffix().toLower()=="hora")
            HthTipo=1;

        if(w_FileInfo.suffix().toLower()=="tempe")
            HthTipo=2;

        if(w_FileInfo.suffix().toLower()=="hume")
            HthTipo=3;

         IsHth=true;
    }

    w_Hth= new Hth(this);
    connect(w_Hth, SIGNAL(Finish()),  this,  SLOT(EmitFinal()));

    QLabel *Label= new QLabel("fff");

    if(w_FileInfo.isDir())
    {
        Aleatorio *w_Aleatorio = new Aleatorio(url,Label);
        url=w_Aleatorio->FicheroPath();
        delete w_Aleatorio;
    }
    //  StreamFile *w_StreamFile = new StreamFile;
     // w_StreamFile->setTitulo=false; //evita poner el currerntsound
     // w_StreamFile->SetUrl(url.toLatin1());
    //  stream =w_StreamFile->stream;
    //  delete w_StreamFile;
    Url=url;
}

/**
 * This function free in memory BASS
 * @brief Pisadores::~Pisadores
 * @return void;
 */
Pisadores::~Pisadores()
{
    BASS_StreamFree(stream);
    delete w_Hth;
}

void Pisadores::Start()
{
    BASS_SetDevice(Dispositivo);    //dispositivo

    StreamFile *w_StreamFile = new StreamFile;
    w_StreamFile->setTitulo = false; //evita poner el currerntsound
    w_StreamFile->SetUrl(Url.toLatin1());
    stream = w_StreamFile->stream;
    delete w_StreamFile;

    EmitInicio();

    if(IsHth)//if tht
    {
       w_Hth->Configuracion(Prefijo);
       if(HthTipo==1)
           w_Hth->StartHora();

       if(HthTipo==2)
           w_Hth->StartTemp();

       if(HthTipo==3)
           w_Hth->StartHumedad();
       return;
    }

    BASS_ChannelPlay(stream,false);
    Timer->start(30);
}

void Pisadores::Update()
{
    if (IsFinal(stream))
    {
        Timer->stop();
        // emit Finish();
        EmitFinal();
        emit FinishCuna(boton);
    }
}

/**
 * This function checks if is the end of STREAM
 * @brief Pisadores::IsFinal
 * @param cual
 * @return true or false
 */
bool Pisadores::IsFinal(HSTREAM cual)
{
    if(BASS_ChannelIsActive(cual) != BASS_ACTIVE_PLAYING)
        return(true);
    else
        return(false);
}

void Pisadores::EmitInicio()
{
    emit Iniciar();
}

void Pisadores::EmitFinal()
{
    emit Finish();
}

/**
 * This functions get the configuration
 * @brief Pisadores::Configuracion
 * @param Player
 * @return void
 */
void Pisadores::Configuracion(const QString Player)
{
    QSettings RaditIni("Radit.ini", QSettings::IniFormat);
    Dispositivo=RaditIni.value(Player + "/Dispositivo").toInt();
}
