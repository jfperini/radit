/**
 * Stream
 * -----------------------------------------
 *
 * - Management stream
 * - Class dedicated to the management of sound files
 *
 * @author Victor Algaba
 * @author Caio Thomas
 */
#ifndef STREAM_H
#define STREAM_H

#include <QTimer>
#include <QString>
#include <QLabel>
#include <qvumeter.h>
#include <QSlider>
#include "Fader.h"
#include "Pisador.h"
#include "bass.h"
#include "StreamMath.h"
class Stream: public QObject
{
    Q_OBJECT

    private:
        HSTREAM streamA;
        HSTREAM streamB;

        QLabel   *Label;
        QLabel   *LTitulo;
        QVUMeter *Vumeter;
        QSlider  *Slider;
        Fader    *FaderStop;
        // Fader    *FaderA;
        // Fader    *FaderB;
        Fader    *w_Fader;
        void FaderOut(HSTREAM cual);
        Pisador *w_Pisador;
       // StreamMath *w_StreamMath;
        QTimer *Timer;  //time to fader

        //void ActualizarContadores();
        //void PuestaCero();

        bool IsFinal(HSTREAM cual);//checks if is the end song
        bool IsPlay(HSTREAM cual);//checks if is play
        bool IsPause(HSTREAM cual);//checks if is pause

        void PlayA();
        void PlayB();

        void Stop(HSTREAM cual);
        void Play(HSTREAM cual);
        void Pause(HSTREAM cual);

        QString  Prefijo;

        //configurations
        int Descartar;
        int Fundir;
        bool FundirSolapar;
        bool FundirParar;
        bool Detector;
        int DetectorNivel;
        int Render; //update rate timers

        HSTREAM StreamTipo(const QString url);

    public:
        Stream(QWidget *parent = 0);
        ~Stream();
        HSTREAM streamUltimo;
        int Solapar;
        bool IsTanda;
        void Load(const QString url);
        void StopA();
        void StopB();
        void PauseA();
        void PauseB();
        void Retroceso();
        void Avance();
        void pisador();

        //Entrada de Objetos
        void SetLabel(QLabel *w_Label);
        void SetTitulo(QLabel *w_Titulo);
        void SetVumeter( QVUMeter *w_Vumeter);
        void SetSlider(QSlider *w_Slider);

        void Configuracion(const QString Player);

        bool IsValido; //check if stream is valid
        bool IsPisadorIn;//controla el pisador IN
        bool IsPisadorOut;//controla el pisador Out
        bool IsPisador;//checks if volume is down or up from pisador
        int PisadorSegundos;

        QString  IndicadorOnAir;//indica el fichero que esta reproduciendo

        void PuestaCero();

        bool IsRadioOnLine;
        float TiempoRadioOnLine;

        int Dispositivo;//Device sound

    private slots:
        void slot_Barra(int pos);
        void Update();//updates the time
        void ActualizarContadores();

    signals:
        void Finish();//when playback ends
        void PisadorIn();//Seucede cuando el pisador IN debe acrivarse
        void PisadorOut();//Seucede cuando el pisador OUT debe acrivarse
        //void FinishLista();
};
#endif // STREAM_H
