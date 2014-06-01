#ifndef PLAYERGRUPOS_H
#define PLAYERGRUPOS_H

#include <QMainWindow>
#include "ui_FrmGrupos.h"
#include "engine/Stream.h"
#include "engine/Hth.h"
#include "engine/Pisador.h"

class PlayerGrupos : public QMainWindow, private Ui::FrmGrupos
{
    Q_OBJECT

    private:
        Stream *w_Stream;

        void setRojo();
        void setColor();
        Hth *w_Hth;

        PlayerGrupos *w_PlayerGrupos;

        void PlayGrupos(QString Url);
        void PlayHth(int Tipo);
        bool IsGrupo; //checks if is played group

        Pisador *w_Pisador ;
        //void ClickFaderOut();
        //void ClickFaderIn();
        QString UrlPisadorIn;
        QString UrlPisadorOut;

    public:
        explicit PlayerGrupos(QString w_Prefijo, QWidget *parent = 0);

        virtual ~PlayerGrupos(){;}
        void setRotacion(QString Url);
        void setStream(Stream *stream, int Tipo);
        void Start();

        QString Prefijo;
        int w_Row;
        int Solapar;//Guarda la configuracion para enlatados
        bool IsTanda;
        QSplitter *splitter;

    public slots:
        void Final();
        void FinalGrupo();

        void ClickPisadorIn();
        void ClickPisadorOut();

        void ClickFaderOut();
        void ClickFaderIn();

    protected:

    signals:
        void Finish();//after when finished played file
};

#endif // PLAYERGRUPOS_H
