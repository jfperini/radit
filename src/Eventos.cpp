#include <QTimer>
#include <QDateTime>
#include <QFileSystemModel>
#include <QDirModel>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

#include <QSqlRecord>
//#include <QSqlError>
#include <QSqlQuery>

#include "Eventos.h"
#include "EditorEventos.h"
#include "engine/StreamFile.h"
#include "engine/StreamMath.h"

Eventos::Eventos (QMainWindow *parent )
    :QMainWindow(parent)
{
    setupUi(this);

    this->LAviso->setVisible(false);

    QTimer *timer_Hora = new QTimer(this);
    connect(timer_Hora, SIGNAL(timeout()), this, SLOT(UpdateHora())); // temporizaor horario
    timer_Hora->start(1000);

    //hth********************
    w_Hth= new Hth(this);
    connect(w_Hth, SIGNAL(Finish()), this, SLOT(Final()));
    connect(BtnHora, SIGNAL(clicked()), this, SLOT(ClickHtm()));
    connect(BtnTemp, SIGNAL(clicked()), this, SLOT(ClickHtm()));
    connect(BtnHume, SIGNAL(clicked()), this, SLOT(ClickHtm()));

    //botones
    connect(BtnEventos, SIGNAL(clicked()), this, SLOT(ClickEditor()));
    connect(BtnActualizar, SIGNAL(clicked()), this, SLOT(setUpdateEventos()));     // Closed Radit



    //botonera play
    //connect(BtnPlay, SIGNAL(clicked()), this, SLOT(ClickPlay()));

    QFileSystemModel *FileModelo = new QFileSystemModel(this); // modelo de ficheros

    //myModel.setRootPath(myModel.myComputer().toString());

    // QDirModel *FileModelo = new QDirModel(this); // modelo de ficheros

    QStringList filters;
    filters << "*.mp3" << "*.mp2" << "*.mp1" << "*.ogg" << "*.wav" << "*.aif" <<"*.wma" <<"*.flac" <<"*.seq" <<"*.rsc" ;

    FileModelo->setNameFilters ( filters );
    FileModelo->setNameFilterDisables(false);
    FileModelo->setReadOnly(true);
    FileModelo->setRootPath(FileModelo->myComputer().toString());

    QString inicio=QDesktopServices::storageLocation( QDesktopServices::DesktopLocation);


    TExplore->setModel(FileModelo);
    //TExplore->setRootIndex(FileModelo->setRootPath("F:/Music/Music"));

    TExplore->setColumnHidden(1, 1);//hide column
    TExplore->setColumnHidden(2, 1);
    TExplore->setColumnHidden(3, 1);
    //TExplore->setCurrentIndex(FileModelo->index(Destop));

    //comprobamos la base si existe
    Path=QCoreApplication::applicationDirPath().toLatin1();
    QFileInfo file(Path + "/Eventos/evento.evt");

    if(file.exists())
        setBase();
    else
        AddTablas();//if dont has file creat tables of database

    IsEditor=false;
    DeleteEnpuerta();
    FechaHoyToBase();
}


/**
 * Free Event in memory
 * @brief Eventos::~Eventos
 * @return void
 */
Eventos::~Eventos()
{
    db.close();
}

void Eventos::UpdateHora()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString("dddd  dd MMMM yyyy");
    LFecha->setText(dateTimeString);
    dateTimeString = dateTime.toString("hh:mm:ss");
    LHora->setText(dateTimeString);

    QString Path=QCoreApplication::applicationDirPath().toLatin1();

    QFile file(Path.toLatin1() + "/Currenweather/currenweather.txt");

    if (!file.open( QIODevice::ReadOnly| QIODevice::Text))
        return;

    QTextStream out(&file);

    QString line=out.readLine();

    LTemperatura->setText(line.left(2)+ "ºC");
    LHumedad->setText(line.right(3)+ "%");

    file.close();
    ProximoEvento();
    HoraEvento();
    FaltaCinco();
    EsperaMaxima();
}

void Eventos::ClickHtm()
{
    w_Hth->Configuracion("Principal");
    QObject* obj = QObject::sender();
    QPushButton *Boton = qobject_cast<QPushButton *>(obj);
    if(Boton == this->BtnHora){w_Hth->StartHora();}
    if(Boton == this->BtnTemp){w_Hth->StartTemp();}
    if(Boton == this->BtnHume){w_Hth->StartHumedad();}

    emit startHth();  //hace el fader
}

void Eventos::ProximoEvento()
{
   // if(IsEditor) //cambio
     //   return;

    QDateTime FechaActual;

    QDateTime t, HoraActual, Mas15Minutos;
    int Diahoy = QDate::currentDate().dayOfWeek();

    HoraActual = t.currentDateTime();
    Mas15Minutos = HoraActual.addSecs(900);


///Actualizacion de las fechas en la db
    if(QTime::currentTime().secsTo(QTime(23,45,00))==0)  //problemas eventos
        FechaHoyToBase();

    if(QTime::currentTime().secsTo(QTime(00,00,00))==0)  //problemas eventos
        FechaHoyToBase();

   // if(QTime::currentTime().secsTo(QTime(12,00,00))==0)  //problemas eventos
     //   FechaHoyToBase();



    //qDebug() <<HoraActual.time().secsTo(QTime(12,00,00));


    QSqlQuery query(db);
    query.prepare("SELECT * FROM EVENTOS  WHERE CHEQUEADO = ? AND "
                  "ENPUERTA = ? AND INICIOFECHA <= ? AND (EXPIRACION = ? OR EXPIRACION = ? AND EXPIFECHA >= ?) ORDER BY INICIOHORA");

    query.addBindValue(2);
    query.addBindValue(false);
    query.addBindValue(FechaActual.currentDateTime().addSecs(900));  //fecha hora inicio

    query.addBindValue(false);
    query.addBindValue(true);
    query.addBindValue(FechaActual.currentDateTime());
    query.exec();

    QSqlQuery queryHoras(db);

    queryHoras.prepare("SELECT * FROM HORAS  WHERE COD = ? AND "
                       "HORA >= ? AND HORA <= ?");

  /*  QDateTime t, HoraActual, Mas15Minutos;
    HoraActual = t.currentDateTime();
    Mas15Minutos = HoraActual.addSecs(900);



    if(HoraActual.time()==QTime(23,45,00) || HoraActual.time()==QTime(00,00,00))
        FechaHoyToBase();

    int Diahoy = QDate::currentDate().dayOfWeek();*/






    QSqlQuery queryDias(db);
    queryDias.prepare("SELECT * FROM DIAS  WHERE COD = ? AND "
                      "DIA = ?");

    QSqlQuery queryPuerta(db);
    queryPuerta.prepare("UPDATE EVENTOS SET"
                        " ENPUERTA = ?"
                        " WHERE COD = ?");

    query.first();
    while (query.isValid())
    {
        QSqlRecord rec =  query.record();

        queryHoras.addBindValue(rec.value("COD").toInt());
        queryHoras.addBindValue(HoraActual) ;
        queryHoras.addBindValue(Mas15Minutos) ;
        queryHoras.exec();
        queryHoras.first();

        queryDias.addBindValue(rec.value("COD").toInt());
        queryDias.addBindValue(Diahoy) ;
        queryDias.exec();
        queryDias.first();

        // si las horas y los dias son validos
        if (queryHoras.isValid()&& queryDias.isValid())
        {
            QSqlRecord recHoras =  queryHoras.record();

            const int currentRow = this->w_Lista->rowCount();
            this->w_Lista->setRowCount(currentRow + 1);

            QTime  Ho =recHoras.value("HORA").toDateTime().time();
            QString Nombre = Ho.toString("HH:mm:ss");


            QTableWidgetItem *item = new QTableWidgetItem(1);

            item->setText(Nombre);

            this->w_Lista->setItem(currentRow,1, item );  //hora inicio
            this->w_Lista->setItem(currentRow,2, new QTableWidgetItem(rec.value("VERCOMIENZO").toString()));//comienzo
            this->w_Lista->setItem(currentRow,3, new QTableWidgetItem(rec.value("VERFICHERO").toString()));// fichero

            if(!rec.value("CONEXION").toString().isEmpty()) //es una conexion radio
            {
                this->w_Lista->setItem(currentRow,4, new QTableWidgetItem(rec.value("CONEXION").toString()));
                this->w_Lista->item(currentRow, 4 )->setTextAlignment(Qt::AlignRight); // justificamos a la derecha  el tiempo
            }
            else
            {
                BASS_SetDevice(1);//dispositivo
                StreamFile *w_StreamFile = new  StreamFile(rec.value("URL").toString()) ;
                StreamMath * w_StreamMath = new StreamMath(w_StreamFile->stream);

                this->w_Lista->setItem(currentRow,4, new QTableWidgetItem(w_StreamMath->SegundoToFormato("hh:mm:ss")));
                this->w_Lista->item(currentRow, 4 )->setTextAlignment(Qt::AlignRight); // justificamos a la derecha  el tiempo


                BASS_StreamFree(w_StreamFile->stream); //free stream
                delete w_StreamFile;
                delete w_StreamMath;

            }

            //  this->w_Lista->setItem(currentRow,5, new QTableWidgetItem(w_StreamMath->SegundoToFormato(Segundos,"hh:mm:ss")));
            this->w_Lista->setItem(currentRow,6, new QTableWidgetItem(rec.value("URL").toString())); //url
            this->w_Lista->setItem(currentRow,7, new QTableWidgetItem(rec.value("COD").toString())); //cod
            this->w_Lista->setItem(currentRow,8, new QTableWidgetItem(recHoras.value("HORA").toString())); //fechahora

            queryPuerta.addBindValue(true) ;
            queryPuerta.addBindValue(recHoras.value("COD").toInt()) ;
            queryPuerta.exec();
        }
        query.next();
    }

    this->w_Lista->sortItems(8);  //debemos ordenarlos por fechahora
}

void Eventos::FechaHoyToBase()
{
    QSqlQuery query(db);

    query.exec("UPDATE HORAS SET HORA =date('now', 'localtime') || HORAT");
    query.exec("UPDATE HORAS SET HORA =date('now', '+1 day', 'localtime') || HORAT WHERE HORAT < time('now', 'localtime')");
    query.exec("UPDATE HORAS SET HORA = strftime('%Y-%m-%dT%H:%M:%S ',HORA)");

   // qDebug() <<QTime::currentTime().toString();
}
//////////////////////////////////////////////////
void Eventos::FaltaCinco()
{
    if(!w_Lista->rowCount())//si esta vacia nos vamos
        return;

    if(!this->BtnStart->isChecked())//eventos inactivos
    {
        LAviso->setVisible(false);
        return;
    }

    QString h= w_Lista->item(0,8)->text();  //leemos el fichero

    QDateTime HoraEvento, HoraActual;
    HoraEvento = HoraEvento.fromString(h,Qt::ISODate);

    HoraActual = HoraActual.currentDateTime();
    HoraActual = HoraActual.addSecs(300);

    if(HoraEvento <= HoraActual)
    {
        if(LAviso->isVisible())
        {
            LAviso->setVisible(false);
            return;
        }
        else
        {
            LAviso->setVisible(true);
            return;
        }
    }

    LAviso->setVisible(false);
}
////////////////////////////////////////////////////////////
void Eventos::EsperaMaxima()
{
    if(!w_Lista->rowCount())//if is empty returns
        return;

    int cod = QString(w_Lista->item(0,7)->text()).toInt();//leemos el cod

    QSqlQuery query(db);
    query.prepare("SELECT * FROM EVENTOS WHERE COD = ?");
    query.addBindValue(cod);
    query.exec();
    query.first();
    QSqlRecord rec =  query.record();

    bool isEspera= rec.value("COMPESPERA").toBool();
    if(!isEspera){return;} // si no hay espera nos vamos

    QString h= w_Lista->item(0,8)->text();  //leemos el fichero
    int minutos= rec.value("COMMINUTOS").toInt();
    int segundos=minutos*60;

    QDateTime HoraEspera, HoraActual;

    HoraEspera = HoraEspera.fromString(h,Qt::ISODate);
    HoraEspera=HoraEspera.addSecs(segundos);
    HoraActual = QDateTime::currentDateTime();

    if(HoraEspera <= HoraActual)//lo descartamos
    {
        emit descartarEvento();
        BorrarEvento();
        LAviso->setVisible(false);
    }
}

void Eventos::HoraEvento()//comprobamos si es la hora de emisión
{
    if(!w_Lista->rowCount()) //si esta vacia nos vamos
        return;

    if(!this->BtnStart->isChecked()) // si estan desactivados
        return;

    QString Hora = w_Lista->item(0,8)->text();  //leemos el fichero
    QDateTime HoraEvento= QDateTime::fromString(Hora,Qt::ISODate);
    QDateTime HoraActual= QDateTime::currentDateTime();

    if(HoraEvento <= HoraActual)//es la hora
        ClickPlay();
}

void Eventos::ClickPlay() // emitimos el fichero
{
    int cod = QString(w_Lista->item(0,7)->text()).toInt();  //leemos el cod

    QSqlQuery query(db);
    query.prepare("SELECT * FROM EVENTOS WHERE COD = ?");
    query.addBindValue(cod);
    query.exec();
    query.first();
    QSqlRecord rec =  query.record();

    int Comportamiento= rec.value("COMPTIPO").toInt();

    QString url = w_Lista->item(0,6)->text();  //leemos el fichero
    QString Duracion=w_Lista->item(0,4)->text(); //duracion conexion
    emit startEvento(url,Comportamiento,Duracion);

}

void Eventos::Final()
{
    emit finalHth();
}

void Eventos::ClickEditor()
{
    IsEditor=true;
    EditorEventos * w_EditorEventos = new  EditorEventos(db,this);

    w_EditorEventos->exec();
    delete w_EditorEventos;
    emit descartarEvento(); // descartamos los posibles eventos que hubiesen

   // FechaHoyToBase();
   // DeleteEnpuerta();
   // w_Lista->setRowCount(0);
   // IsEditor=false;
   // LAviso->setVisible(false);

    setUpdateEventos();

}

void Eventos::setUpdateEventos(){  //eventos desaparece posible correccion

    //emit descartarEvento(); // descartamos los posibles eventos que hubiesen

    FechaHoyToBase();
    DeleteEnpuerta();
    w_Lista->setRowCount(0);
    IsEditor=false;
    LAviso->setVisible(false);

}

void Eventos::DeleteEnpuerta()//ponemos todos en puerta como falso para emitirlos
{
    QSqlQuery query(db);
    query.prepare("UPDATE EVENTOS SET"
                  " ENPUERTA = ? WHERE ENPUERTA = ?");

    query.addBindValue(false) ;
    query.addBindValue(true) ;
    query.exec();
}

void Eventos::BorrarEvento() //una vez reproducido borramos el evento señal que viene del player
{
    int cod = QString(w_Lista->item(0,7)->text()).toInt();  //leemos el fichero

    QSqlQuery query(db);
    query.prepare("UPDATE EVENTOS SET ENPUERTA = ?"   //lo desmarca de enpuerta
                  "WHERE COD = ?");

    query.addBindValue(false);
    query.addBindValue(cod);
    query.exec();

    w_Lista->removeRow(0);
    LAviso->setVisible(false);



}

void Eventos::setBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(Path + "/Eventos/evento.evt");
    db.open();
}

void Eventos::AddTablas()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(Path + "/Eventos/evento.evt");
    db.open();

    QSqlQuery query(db);

    // PESTAÑAS//////////////////////////////////////////
    query.exec("create table PESTANA (NOMBRE text primary key)");

    //eventos
    query.exec("create table EVENTOS (COD int primary key,"

               "UNAVEZ bool,"
               "CADAHORA bool,"           //prioridad
               "OTRAS bool,"

               "INICIOHORA time,"
               "INICIOFECHA datetime,"          //fecha/hora inicio

               "EXPIRACION bool, "
               "EXPIHORA time,"              //fecha/hora expriracion
               "EXPIFECHA datetime,"

               "PRIORIDAD bool,"

               "COMPTIPO int,"
               "COMPESPERA bool,"
               "COMMINUTOS int,"

               "LUNES bool,"
               "MARTES bool,"
               "MIERCOLES bool,"
               "JUEVES bool,"
               "VIERNES bool,"
               "SABADO bool,"
               "DOMINGO bool,"

               "H0 bool,"
               "H1 bool,"
               "H2 bool,"
               "H3 bool,"
               "H4 bool,"
               "H5 bool,"
               "H6 bool,"
               "H7 bool,"
               "H8 bool,"
               "H9 bool,"
               "H10 bool,"
               "H11 bool,"
               "H12 bool,"
               "H13 bool,"
               "H14 bool,"
               "H15 bool,"
               "H16 bool,"
               "H17 bool,"
               "H18 bool,"
               "H19 bool,"
               "H20 bool,"
               "H21 bool,"
               "H22 bool,"
               "H23 bool,"

               "PESTANA text,"

               "TIPO int,"
               "URL text,"
               "CONEXION text,"

               "VERHORAINICIO text,"
               "VEREXPIRACION text,"
               "VERCOMIENZO text,"
               "VERPRIORIDAD text,"
               "VERHORAS text,"
               "VERDIAS text,"
               "VERFICHERO text,"

               "CHEQUEADO int,"
               "ENPUERTA bool)"
               );

    //horas
    query.exec("create table HORAS (COD int,"
               "HORA datetime,"
               "HORAT time,"
               "EXPIRACION bool, "
               "EXPIFECHA datetime,"
               "PESTANA text)"
               );
    //dias
    query.exec("create table DIAS (COD int,"
               "DIA int,"
               "EXPIRACION bool, "
               "EXPIFECHA datetime,"
               "PESTANA text)"
               );
}

