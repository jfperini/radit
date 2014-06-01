#include <QDebug>

#include "PlayerGrupos.h"
#include "micelanea/Aleatorio.h"
#include "iolista/IoLista.h"
#include "engine/Pisadores.h"
#include "engine/StreamMath.h"


PlayerGrupos::PlayerGrupos(QString w_Prefijo, QWidget*parent )
    :QMainWindow(parent)
{
    setupUi(this);

    ///Hth
    w_Hth= new Hth(this);
    connect(w_Hth, SIGNAL(Finish()), this, SLOT(Final())); // temporizaor horario

    Prefijo=w_Prefijo;//////////////////esto puede ser inutil revisar

    w_Row =0;
    Lista-> setRowCount(0);
    w_Pisador = new Pisador(this);
}

void PlayerGrupos::setRotacion(QString Url)
{
    Lista-> setRowCount(0);

    QFile File;
    File.setFileName(Url);
    // qDebug() <<Url;

    if(File.open(QIODevice::ReadOnly))
    {
        IoLista *w_IoLista = new IoLista(this->Lista);
        w_IoLista->loadModel(&File, this->Lista);
        delete w_IoLista;
    }

    File.close();

    Lista->setCurrentCell(0,0);//set first cell
}

void PlayerGrupos::setStream(Stream *stream, int Tipo)
{
    w_Stream=stream;
    Solapar=w_Stream->Solapar;

    connect(w_Stream,SIGNAL(Finish()),this, SLOT(Final()));

    //Pisadores
    connect(w_Stream,SIGNAL(PisadorIn()),this, SLOT(ClickPisadorIn()));
    connect(w_Stream,SIGNAL(PisadorOut()),this, SLOT(ClickPisadorOut()));
}


////////////////////////////////////////////////////////////////////////////
void PlayerGrupos::Start()
{


     // w_Stream->Load("C:/discoteca/NOVEDADES DEL MES/KIKE GABANA - Dímelo Bajito.mp3");
     // return;

    QString url=Lista->item(w_Row , 4)->text(); //leemos el iten
    QFileInfo w_FileInfo(url);

    ///HTH
    if(w_FileInfo.suffix().toLower()=="hora" ||
       w_FileInfo.suffix().toLower()=="tempe" ||
       w_FileInfo.suffix().toLower()=="hume" )
    {
        setRojo();
        w_Stream->PuestaCero();//ponemos el vumetro a 0

        if(w_FileInfo.suffix().toLower()=="hora")
            PlayHth(1);

        if(w_FileInfo.suffix().toLower()=="tempe")
            PlayHth(2);

        if(w_FileInfo.suffix().toLower()=="hume")
            PlayHth(3);
        return;
    }

    if(w_FileInfo.suffix().toLower()=="seq" ||
       w_FileInfo.suffix().toLower()=="tad" )//grupos
    {
        setRojo();
        PlayGrupos(url);
        return;
    }


    //===========================================================

    w_Stream->IsPisadorIn=false;  //falsos de inicio
    w_Stream->IsPisadorOut=false;

    //===========================================================

    if(w_FileInfo.isDir())//its file song?
    {
        QLabel w_Label;
        Aleatorio *w_Aleatorio= new Aleatorio(url,&w_Label);
        url=w_Aleatorio->FicheroPath();
        delete w_Aleatorio;
    }

    w_Stream->IsTanda=IsTanda;
    w_Stream->Load(url);


    setRojo();

    //radio on-line
    if(w_Stream->IsRadioOnLine)
    {
        StreamMath *w_StreamMath = new StreamMath;
        w_Stream->TiempoRadioOnLine=w_StreamMath->FormatoToSegundos(Lista->item(w_Row, 2)->text());

        delete w_StreamMath;
        return;
    }

    ///pisadores
    if(IsTanda)
        return;//si es tanda no soparta pisadores

    if(Lista->item(w_Row, 6)->text()!="")//si hay algo en el url-in del pisador
    {
        w_Stream->IsPisadorIn=true;
        w_Stream->PisadorSegundos=Lista->item(w_Row, 5)->text().toInt();//tiempo de inicio del pisador
        UrlPisadorIn=Lista->item(w_Row, 6)->text();
    }

    if(Lista->item(w_Row, 7)->text()!="")//si hay algo en el urlin del pisador
    {
        w_Stream->IsPisadorOut=true;
        UrlPisadorOut=Lista->item(w_Row, 7)->text();
    }
}

void PlayerGrupos::Final()
{

//Start();
//return;
    if(Lista->rowCount()==w_Row +1)// estan todos
    {
        disconnect(w_Stream,SIGNAL(Finish()),this, SLOT(Final()));
        disconnect(w_Stream,SIGNAL(PisadorIn()),this, SLOT(ClickPisadorIn()));
        disconnect(w_Stream,SIGNAL(PisadorOut()),this, SLOT(ClickPisadorOut()));

        emit  this->Finish();
        this->close();

        return;
    }



    setColor();

    w_Row ++;

    Start();
}

void PlayerGrupos::PlayGrupos(QString Url)
{
    QFileInfo w_FileInfo(Url);

    disconnect(w_Stream,SIGNAL(Finish()),this, SLOT(Final())); //desconectamos el padre
    disconnect(w_Stream,SIGNAL(PisadorIn()),this, SLOT(ClickPisadorIn()));
    disconnect(w_Stream,SIGNAL(PisadorOut()),this, SLOT(ClickPisadorOut()));


    w_PlayerGrupos = new PlayerGrupos(this->Prefijo,this);
    splitter->addWidget(w_PlayerGrupos);

    QString Titulo;
    QFileInfo File(Url);
    Titulo=File.baseName().toLatin1();

    w_PlayerGrupos->setRotacion(Url);

    w_PlayerGrupos->setStream(w_Stream,0);
    w_PlayerGrupos->splitter=splitter;

    if(w_FileInfo.suffix().toLower()=="tad")//checks if is tanda (Commercial Break)
        w_PlayerGrupos->IsTanda=true;
    else
        w_PlayerGrupos->IsTanda=false;

    w_PlayerGrupos->w_Row=0; //lo fijamos al principio
    w_PlayerGrupos->Start();
    IsGrupo=true;

    connect(w_PlayerGrupos,SIGNAL(Finish()),this, SLOT(FinalGrupo())); //final del grupo
}

void PlayerGrupos::PlayHth(int Tipo)
{
    w_Hth->Configuracion(Prefijo);

    if (Tipo==1)
        w_Hth->StartHora();

    if (Tipo==2)
        w_Hth->StartTemp();

    if (Tipo==3)
        w_Hth->StartHumedad();
}

void PlayerGrupos::FinalGrupo()
{
    connect(w_Stream,SIGNAL(Finish()),this, SLOT(Final()));  //conectamos el padre
    IsGrupo=false;

    Final();
}

////////////Señal que viene del streamer///////////////////////

void PlayerGrupos::ClickPisadorIn()
{
    qDebug() <<"pisador";

    Pisadores *w_Pisadores = new Pisadores(UrlPisadorIn.toLatin1(),this);
    w_Pisadores->Configuracion(Prefijo);
    w_Pisadores->Prefijo=this->Prefijo;

    connect(w_Pisadores , SIGNAL(Iniciar()), this, SLOT(ClickFaderOut()));
    connect(w_Pisadores , SIGNAL(Finish()), this, SLOT(ClickFaderIn()));

    w_Pisadores->Start();
}

void PlayerGrupos::ClickPisadorOut()
{
    Pisadores *w_Pisadores = new Pisadores(UrlPisadorOut.toLatin1(),this);
    w_Pisadores->Configuracion(Prefijo);
    w_Pisadores->Prefijo=this->Prefijo;

    connect(w_Pisadores , SIGNAL(Iniciar()), this, SLOT(ClickFaderOut()));
    connect(w_Pisadores , SIGNAL(Finish()), this, SLOT(Final()));

    w_Pisadores->Start();
}

void PlayerGrupos:: ClickFaderOut()
{
    //Pisador *w_Pisador = new Pisador(w_Stream->streamUltimo,true,this->Prefijo);
    //w_Pisador->start();
    w_Pisador ->setPrefijo(Prefijo);
    w_Pisador ->setStream(w_Stream->streamUltimo);
    w_Pisador ->setEstado(true);
    w_Pisador ->start();
}

void PlayerGrupos:: ClickFaderIn()
{
    //Pisador *w_Pisador = new Pisador(w_Stream->streamUltimo,false,this->Prefijo);
    //w_Pisador->start();
    w_Pisador ->setPrefijo(Prefijo);
    w_Pisador ->setStream(w_Stream->streamUltimo);
    w_Pisador ->setEstado(false);
    w_Pisador ->start();
}

void PlayerGrupos::setRojo()
{
    Lista->item(w_Row ,0)->setBackground(QBrush(QColor(Qt::red)));
    Lista->item(w_Row ,1)->setBackground(QBrush(QColor(Qt::red)));
    Lista->item(w_Row ,2)->setBackground(QBrush(QColor(Qt::red)));

    Lista->item(w_Row ,0)->setForeground(QBrush(QColor(Qt::white)));
    Lista->item(w_Row ,1)->setForeground(QBrush(QColor(Qt::white)));
    Lista->item(w_Row ,2)->setForeground(QBrush(QColor(Qt::white)));
}

void PlayerGrupos::setColor()
{
    Lista->item(w_Row ,0)->setBackground(QBrush(QColor(32,39,47)));
    Lista->item(w_Row ,1)->setBackground(QBrush(QColor(32,39,47)));
    Lista->item(w_Row ,2)->setBackground(QBrush(QColor(32,39,47)));

    Lista->item(w_Row ,0)->setForeground(QBrush(QColor(7,250,242)));
    Lista->item(w_Row ,1)->setForeground(QBrush(QColor(7,250,242)));
    Lista->item(w_Row ,2)->setForeground(QBrush(QColor(7,250,242)));
}
