/**
 * Hth
 * -----------------------------------------
 *
 * - This class uses to play time, temperature and humidity.
 * - Check folder Hth
 *
 * @author Victor Algaba
 */



#include <QSettings>
#include <QDebug>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>
#include <stdlib.h>
#include "Hth.h"
#include "StreamMath.h"

Hth::Hth(QObject *parent):
    QObject(parent)
{
    Timer = new QTimer();
    connect(Timer, SIGNAL(timeout()), this, SLOT(Update())); // temporizaor horario
    IsHFinal=false;//checks if has played the hour
    IsMFinal=false;

    IsHora=false;
    IsTemperatura=false;
    IsHumedad=false;
    Path=QCoreApplication::applicationDirPath().toLatin1();

    LeerClima();
}

Hth::~Hth()
{
    BASS_StreamFree(stream); //free sream
    delete Timer;
}

/**
 * This function
 * @brief Hth::StartHora
 */
void Hth::StartHora()
{
    BASS_SetDevice(Dispositivo);//device

    IsHora=true;
    emit Iniciar();

    QDateTime dateTime = QDateTime::currentDateTime();//get the time
    QTime playTime=dateTime.time();//decompose the time

    QString Hora , Minutos; //Path;
    //Path=QCoreApplication::applicationDirPath().toLatin1();

    Hora= "HRS" + playTime.toString("hh")+ QString(".mp3").toLower();  //obtenemos fichero de hora
    Minutos ="MIN" + playTime.toString("mm")+ QString(".mp3").toLower();  //obtenemos fichero de minutos

    //time point
    if (playTime.minute() == 0)
    {
        Hora= "HRS" + playTime.toString("hh")+ "_O" + QString(".mp3").toLower(); //obtenemos fichero hora en punto añadiendo _O
        IsMFinal=true; //evita decir minutos
    }

    if(!IsHFinal)//si no se ha reprodcido la hora
    {
        IsHFinal = true;
        stream = BASS_StreamCreateFile(FALSE, Path.toLatin1() + "/Hth/Time/"+ Hora.toLatin1(), 0, 0,BASS_STREAM_AUTOFREE);
        BASS_ChannelPlay(stream,false);
        Timer->start(30);
        return;
    }

    if(!IsMFinal)//checks if played the seconds
    {
        IsMFinal = true;
        BASS_StreamFree(stream); //free stream of memory
        stream = BASS_StreamCreateFile(FALSE, Path.toLatin1() + "/Hth/Time/" + Minutos.toLatin1(), 0, 0, BASS_STREAM_AUTOFREE);
        BASS_ChannelPlay(stream,false);
        Timer->start(30);
        return;
    }

    emit Finish();//si es hora en punto
}

/**
 * Locution temperature
 * @brief Hth::StartTemp
 * @return void
 */
void Hth::StartTemp()
{
    BASS_SetDevice(Dispositivo);//device
    LeerClima();//data update

    QFileInfo File(Path.toLatin1() + "/Hth/Temp/TMP" + Temperatura.toLatin1() + ".mp3");

    IsTemperatura=true;
    BASS_StreamFree(stream);//free
    stream=BASS_StreamCreateFile(FALSE, File.absoluteFilePath().toLatin1(), 0, 0, BASS_STREAM_AUTOFREE);
    BASS_ChannelPlay(stream,false);
    Timer->start(30);
}

/**
 * Locution Humidity
 * @brief Hth::StartHumedad
 * @return void
 */
void Hth::StartHumedad()
{
    BASS_SetDevice(Dispositivo);//device
    LeerClima();

    QFileInfo File(Path.toLatin1() + "/Hth/Hume/HUM" + Humedad.toLatin1() + ".mp3");

    IsTemperatura=true;
    BASS_StreamFree(stream); //free memory
    stream=BASS_StreamCreateFile(FALSE, File.absoluteFilePath().toLatin1(), 0, 0, BASS_STREAM_AUTOFREE);
    BASS_ChannelPlay(stream,false);
    Timer->start(30);
}

/**
 * This function processes the time
 * @brief Hth::Update
 * @return void
 */
void Hth::Update()
{
    if (IsFinal(stream))//checks if hstream is the end
    {
        Timer->stop();

        if(IsHora)// es hora
        {
            if(IsMFinal)// primero comprobamos los minutos, si es el final de los minutos
            {
                emit Finish();         // Se reproducieros las hora y mandamos la señal de final
                IsHora=false;
                return;
            }

            if(IsHFinal)
            {
                StartHora();
                return;
            }

          }

        emit Finish();         // es temperatura/humedad
     }
}

/**
 * This function checks if is the end of pre listening
 * @brief Hth::IsFinal
 * @param cual Hstream
 * @return true or false
 */
bool Hth::IsFinal(HSTREAM cual)
{
    if(BASS_ChannelIsActive(cual)!=BASS_ACTIVE_PLAYING){
        BASS_StreamFree(stream); //free sream
        return(true);
    }
    else{
        return(false);
            }
}

/**
 * This fucntion get the configuration of radit
 * @brief Hth::Configuracion
 * @param Player
 * @return void
 */
void Hth::Configuracion(const QString Player)
{
    QSettings RaditIni("Radit.ini", QSettings::IniFormat);
    Dispositivo=RaditIni.value(Player + "/Dispositivo").toInt();
    IsHFinal=false;//Ha reproducido la hora
    IsMFinal=false;
}

/**
 * This function read temperature and humidity
 * @brief Hth::LeerClima
 * @return void
 */
void Hth:: LeerClima()
{
    QFile file(Path.toLatin1() + "/Currenweather/currenweather.txt");//select file

    if (!file.open( QIODevice::ReadOnly| QIODevice::Text))
        return;

    int tempo;
    bool MenosCero=false;

    QTextStream out(&file);
    QString line=out.readLine();

    Temperatura=line.left(2);//read file

    tempo=Temperatura.toInt();//convert to int

    if(tempo<0)//if the time is less then zero
    {
        tempo=abs(tempo);
        MenosCero=true;
    }

    Temperatura = Temperatura.setNum(tempo);

    if(tempo<10)
        Temperatura = "0" + Temperatura;

    Temperatura = "0" + Temperatura;

    if(MenosCero)
        Temperatura = "N" + Temperatura;

    Humedad = line.right(3);
    // qDebug() <<Humedad;
    Humedad=Humedad.left(3);
    tempo= Humedad.toInt();
    Humedad=Humedad.setNum(tempo);

    if(tempo<10)
        Humedad = "0" + Humedad;

    if(tempo !=100)
        Humedad = "0" +Humedad;

    file.close();
}
