#ifndef PLAYER_H
#define PLAYER_H

//#include <QMainWindow>
#include <QMainWindow>
#include <QByteArray>
#include "ui_FrmPlayer.h"

#include "engine/Stream.h"
#include "engine/Pisador.h"
#include "engine/Hth.h"
#include "Indicadores.h"
#include "PlayerGrupos.h"

class Player : public QMainWindow, private Ui::FrmPlayer
{
    Q_OBJECT

    public:
        Player(QString w_Prefijo, QMainWindow *parent = 0);
        ~Player();
        //identifica al reproductor para las configuraciones
        QDockWidget *Dock;
        Indicadores *w_Indicadores;
        QMainWindow *MainWindow;  //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

    public slots:
        //void Start(QString Url, bool IsEvento);
        void Evento(QString Url, int tipo, QString Duracion);
        void DescartarEvento();//descartamos el evento

    private slots:
        void ShowToolBar();

        //botonera
        void ClickPlay();
        void ClickStop();
        void ClickPause();
        void ClickAtras();
        void ClickAlante();
        void ClickParar();
        void ClickBtnStopPlay();
        void ClickLista();

        void ClickPisador();

        //comobo
        void ClickModo();

        //finales
        void Final();
        void FinalGrupo();

        void ClickBorrar();

        //menu Herramientas
        void ClickPreferencias();

        //menu file
        void ClickNuevo();
        void ClickAbrir();
        void ClickGuardarComo();
        void ClickGuardar();

        //pisadores
        void ClickPisadorIn();
        void ClickPisadorOut();

        void ClickFaderOut();
        void ClickFaderIn();

        //Menulista
        void AddRadio();

    private:
        void Start(QString Url, bool IsEvento);

        Stream *w_Stream;
        bool IsEventoEspera;//hay un evento pendiente de emitir
        bool IsFinal;  //es el final de la cancion nos sirve para activar eventos pendientes

        void PonerVerde();
        bool IsPause;
        QLabel *LTotal;
        QString Prefijo;        // prefijo para las configuraciones
        QString NombreLista;  // el nombre del fichero la lista cargada
        QString UrlPisadorIn;
        QString UrlPisadorOut;

        void setIndicadores(QString cual, bool OnAir );

        void PlayGrupos(QString Url);
        bool IsGrupo;                //se esta reproduciendo un grupo
        void PlayHth(int Tipo);

        QDockWidget *w_Dock ;
        PlayerGrupos *w_PlayerGrupos;
        Pisador *w_Pisador ;
        Hth *w_Hth;

        int TmpAncho;  //dejamos el mismo ancho despues del grupo

        QString UrlEvento;
        QString EventoConexion;

        void QuitarRojo();

    protected:
        virtual void closeEvent(QCloseEvent *e);
        void resizeEvent( QResizeEvent *event) ;
        void contextMenuEvent(QContextMenuEvent *event);

    signals:
        void borrarEvento();
};

#endif // PLAYER_H
