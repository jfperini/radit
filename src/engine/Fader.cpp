/**
 * Fader
 * -----------------------------------------
 *
 * - This class is responsible for generating fade in bass.h
 * - Generate fade Out
 *
 * @author Victor Algaba
 */
#include <QDebug>
#include "Fader.h"

Fader::Fader(QObject *parent): QThread(parent){}

/**
 * This a builder class
 * @brief Fader::Fader
 * @param Stream Hstream
 * @param Fundir value int
 * @param parent
 */
Fader::Fader(HSTREAM Stream, int Fundir, QObject *parent):
    QThread(parent)
{
    stream=Stream;
    Tiempo=Fundir*1000;
}

/**
 * This functions implements QThread
 * @brief Fader::run
 * @return void
 */
void Fader::run()
{
    BASS_ChannelSlideAttribute(stream, BASS_ATTRIB_VOL, -1, Tiempo);
    this->msleep(Tiempo);
    BASS_ChannelStop(stream);
    BASS_StreamFree(stream); //lo liberamos
    // this->terminate();
}

/**
 * This function set Stream
 * @param Stream
 * @return void
 */
void Fader::setStream(HSTREAM Stream)
{
    stream=Stream;
}

/**
 * Set the tiempo
 * @param int Fundir value
 * @return void
 */
void Fader::setFundir(int Fundir)
{
    Tiempo=Fundir*1000;
}
