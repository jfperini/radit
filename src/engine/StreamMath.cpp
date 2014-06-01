/**
 * StreamMath
 * -----------------------------------------
 *
 * - This class supports the mathematical system of the audio file.
 *
 * @author Victor Algaba
 */
#include <math.h>
#include <QDebug>
#include "StreamMath.h"

StreamMath::StreamMath(){
    playTime= new QTime();

}

StreamMath::StreamMath(HSTREAM stream)
{
     playTime= new QTime();
    this->stream=stream;
}

StreamMath::~StreamMath(){

    delete playTime;
}




/**
 * Set Hstream
 * @brief StreamMath::SetStream
 * @param stream
 * @return void
 */
void StreamMath::SetStream(HSTREAM stream)
{
    this->stream=stream;
}




/**
 * Get Duration
 * @brief StreamMath::Duracion
 * @return double
 */
double StreamMath::Duracion()
{
    return(BASS_ChannelBytes2Seconds(stream, BASS_ChannelGetLength(stream, BASS_POS_BYTE)));
}

/**
 * Get time in progress
 * @brief StreamMath::Trascurridos
 * @return double
 */
double StreamMath::Trascurridos()
{
    return(BASS_ChannelBytes2Seconds(stream, BASS_ChannelGetPosition(stream, BASS_POS_BYTE)));
}

/**
 * Returns the duration in seconds elapsed minus
 * (Duration less Transcurridos)
 * @brief StreamMath::Contador
 * @return double
 */
double StreamMath::Contador()
{
    return(Duracion()-Trascurridos());
}

/**
 * Get level of DB of channel right
 * @brief StreamMath::VumetroIz
 * @return double
 */
double StreamMath::VumetroDe()
{
    //double level;
    int level;
    level=BASS_ChannelGetLevel(stream);
    return(ToDb(LOWORD(level)));
}

/**
 * Get level of DB of channel left
 * @brief StreamMath::VumetroIz
 * @return double
 */
double StreamMath::VumetroIz()
{
    //double level;
    int level;
    level=BASS_ChannelGetLevel(stream);
    return(ToDb(HIWORD(level)));
}

/**
 * Returns the decibel
 * @brief StreamMath::ToDb
 * @param valor
 * @return dobule
 */
double StreamMath::ToDb(double valor)
{
    double db=20*log(valor/100); //log esta en <math.h>
    return(db);
}

/**
 * Convert formato (string) to secunds
 * @brief StreamMath::FormatoToSegundos
 * @param Formato
 * @return double
 */
double StreamMath::FormatoToSegundos(QString Formato)
{
    QTime time = QTime::fromString(Formato, "HH:mm:ss"); // invalid
    double Minutos, Segundos;

    Minutos=time.hour()*60;
    Minutos= Minutos + time.minute();
    Segundos=Minutos *60 + time.second();
    //qDebug() <<time.hour() <<time.minute() <<time.second();

    return(Segundos);
}


/**
 * This function convert secunds (float) to formato (string)
 * @brief StreamMath::SegundoToFormato
 * @param Segundos
 * @param Formato
 * @return string
 */
QString StreamMath::SegundoToFormato(float Segundos,QString Formato)
{
   long pos=Segundos*1000;
   long sec = pos/1000;
   long min = sec/60;
   long hour = min/60;
   long msec = pos;

   playTime->setHMS(hour%60, min%60,  sec%60, msec%1000);

    return(playTime->toString("hh:mm:ss.zzz").left(10));

}

/**
 * This function convert secunds to string
 * @brief StreamMath::SegundoToFormato
 * @return string
 */
QString StreamMath::SegundoToFormato()
{
    float Segundos= Duracion();
    long pos=Segundos*1000;
    long sec = pos/1000;
    long min = sec/60;
    long hour = min/60;
    long msec = pos;
    //QTime *playTime = new QTime((int) hour%60, (int) min%60, (int) sec%60, (int) msec%1000);
   playTime->setHMS((int) hour%60, (int) min%60, (int) sec%60, (int) msec%1000);
    return(playTime->toString("hh:mm:ss:z"));
}

/**
 * @brief StreamMath::SegundoToFormato
 * @param Formato
 * @return string
 */
QString StreamMath::SegundoToFormato(QString Formato)
{
    double Segundos=Duracion();

    long pos=Segundos*1000;
    long sec = pos/1000;
    long min = sec/60;
    long hour = min/60;
    long msec = pos;

   // QTime *playTime = new QTime((int) hour%60, (int) min%60, (int) sec%60, (int) msec%1000);

    playTime->setHMS((int) hour%60, (int) min%60, (int) sec%60, (int) msec%1000);
    return(playTime->toString(Formato));
}

