/**
 * Pisador
 * -----------------------------------------
 *
 * - This class extends QThread
 * - This class implements one pisador
 *      - up the level
 *      - down the level
 *
 * @author Victor Algaba
 */
#include <QDebug>
#include <QSettings>
#include "Pisador.h"

Pisador::Pisador(QObject *parent):
    QThread(parent)
{}

Pisador::Pisador(HSTREAM Stream, bool Estado, QString Prefijo, QObject *parent):
    QThread(parent)
{
    stream=Stream;
    estado=Estado;

    /*QSettings RaditIni("Radit.ini", QSettings::IniFormat);
    Transicion=RaditIni.value(Prefijo + "/Transicion").toInt();
    Locucion=RaditIni.value(Prefijo + "/Locucion").toFloat();*/

    setPrefijo(Prefijo);
}

/**
 * Overload of class QThread
 * @brief Pisador::run
 * @return void
 */
void Pisador::run()
{
    if(estado)
        BASS_ChannelSlideAttribute(stream, BASS_ATTRIB_VOL, Locucion, Transicion);
    else
        BASS_ChannelSlideAttribute(stream, BASS_ATTRIB_VOL, 1, Transicion);

    this->msleep(Transicion);
    // this->terminate();
}

void Pisador::setStream(HSTREAM Stream)
{
    stream=Stream;
}

void Pisador::setEstado(bool Estado)
{
    estado=Estado;
}

/**
 * This function get the init configuration
 * @brief Pisador::setPrefijo
 * @param Prefijo
 * @return void
 */
void Pisador::setPrefijo(QString  Prefijo)
{
    QSettings RaditIni("Radit.ini", QSettings::IniFormat);
    Transicion=RaditIni.value(Prefijo + "/Transicion").toInt();
    Locucion=RaditIni.value(Prefijo + "/Locucion").toFloat();
}
