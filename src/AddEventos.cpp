/**
 * AddEventos
 * -----------------------------------------
 *
 * - View of about software
 * - This class represents a Dialog of new Event
 * - Has control of event add
 *
 * @author Victor Algaba
 */
#include <QSettings>
#include <QFileDialog>
#include <QSqlRecord>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>

#include "AddEventos.h"
#include "AddHora.h"
#include "AddRadioInternet.h"

AddEventos::AddEventos(QSqlDatabase w_db, QWidget*parent )
    :QDialog(parent)
{
    setupUi(this);
    db=w_db;//request data base
    setModal(true);


    //set size of form
    QSize fixedSize(this->width(),this->height());
    setMinimumSize(fixedSize);
    setMaximumSize(fixedSize);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    //Aceptar/Cancelar
    connect(BtnAceptar,SIGNAL(clicked()),this, SLOT(ClickAceptar())); //ok
    connect(BtnCancelar,SIGNAL(clicked()),this, SLOT(close())); //cancel

    connect(ComboEvento,SIGNAL(activated(int)),this, SLOT(ClickTipo(int))); //type event
    connect(ChecEspera, SIGNAL(clicked(bool)), SpinEspera, SLOT(setEnabled(bool)));//Espera máxima

    connect(ChecExpiracion, SIGNAL(clicked(bool)), TimeExpiracion, SLOT(setEnabled(bool)));//expiracion
    connect(ChecExpiracion, SIGNAL(clicked(bool)), DateExpiracion, SLOT(setEnabled(bool)));

    connect(RadioOtrasHoras, SIGNAL(clicked()), this, SLOT(OtrasHoras()));
    connect(RadioUnaVez, SIGNAL(clicked()), this, SLOT(OtrasHoras()));
    connect(RadioCadaHora, SIGNAL(clicked()), this, SLOT(OtrasHoras()));


    connect(BtnTodos, SIGNAL(clicked(bool)), BtnLunes, SLOT(click())); //¿todos?
    connect(BtnTodos, SIGNAL(clicked(bool)), BtnMartes, SLOT(click())); //¿todos?
    connect(BtnTodos, SIGNAL(clicked(bool)), BtnMiercoles, SLOT(click())); //¿todos?
    connect(BtnTodos, SIGNAL(clicked(bool)), BtnJueves, SLOT(click())); //¿todos?
    connect(BtnTodos, SIGNAL(clicked(bool)), BtnViernes, SLOT(click())); //¿todos?
    connect(BtnTodos, SIGNAL(clicked(bool)), BtnSabado, SLOT(click())); //¿todos?
    connect(BtnTodos, SIGNAL(clicked(bool)), BtnDomingo, SLOT(click())); //¿todos?

    connect(TimeHoraInicio, SIGNAL(editingFinished()), this, SLOT( HoratoCheckBox()));


    connect(BtnUrl,SIGNAL(clicked()),this, SLOT(ClickUrl()));//url of add radio
    connect(BtnAddRadio,SIGNAL(clicked()),this, SLOT(AddRadio())); //button add radio


    //QString Path=QCoreApplication::applicationDirPath().toLatin1();
    //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(Path + "/Eventos/evento.evt");
    //db.open();

    QSqlQuery query(db);

    w_AddHora = new AddHora(this);

    QDateTime w_DateTime;

    this->TimeHoraInicio->setTime(QDateTime::currentDateTime().time());  //ponemos hora y fecha de inico al dia de hoy
    this->DateFechaInicio->setDate(QDateTime::currentDateTime().date());

    TimeExpiracion->setTime(QDateTime::currentDateTime().time());  //ponemos hora y fecha expiracion  al dia de hoy
    DateExpiracion->setDate(QDateTime::currentDateTime().date());

    HoratoCheckBox();

    ModoEdit=false;
    this->FrameTipoRadio->setVisible(false); //parametros radio on-line false por defecto
}

void AddEventos::OtrasHoras()
{
    QObject* obj = QObject::sender();
    QRadioButton *RadioBtn = qobject_cast<QRadioButton *>(obj);

    if(RadioUnaVez==RadioBtn)
    {
        HoratoCheckBox();
    }

    if(RadioCadaHora==RadioBtn)
        w_AddHora->Todos();

    if(RadioOtrasHoras==RadioBtn)
        w_AddHora->exec();


    QCheckBox *hora[24]={w_AddHora->H0,w_AddHora->H1,w_AddHora->H2,w_AddHora->H3,w_AddHora->H4,
                         w_AddHora->H5,w_AddHora->H6,w_AddHora->H7,w_AddHora->H8,w_AddHora->H9,
                         w_AddHora->H10,w_AddHora->H11,w_AddHora->H12,w_AddHora->H13,w_AddHora->H14,
                         w_AddHora->H15,w_AddHora->H16,w_AddHora->H17,w_AddHora->H18,w_AddHora->H19,
                         w_AddHora->H20,w_AddHora->H21,w_AddHora->H22,w_AddHora->H23};
    int Cuantos=0;

    for(int i=0;i < 24;i++)
        if(hora[i]->isChecked())
            Cuantos++;  //averiguamos cuantos hay

    if(Cuantos==1)
        RadioUnaVez->setChecked(true);// SI SOLO es una hora activa radio una vez

    if(Cuantos==24)
        RadioCadaHora->setChecked(true);// si son todas activa radio cada hora

    for(int i=0;i < 24;i++)
    {
        if(hora[i]->isChecked())// pone la primera hora como hora de inicio
        {
            QTime PrimeraHora;
            int thora, tminuto, tsegundo;
            thora=hora[i]->text().toInt();
            tminuto=TimeHoraInicio->time().minute();
            tsegundo=TimeHoraInicio->time().second();

            PrimeraHora.setHMS(thora,tminuto,tsegundo);
            TimeHoraInicio->setTime(PrimeraHora);
            return;
        }
        //qDebug() <<hora[i]->text();
        //qDebug() <<TimeHoraInicio->time().hour();
    }
}

void AddEventos::ClickAceptar()
{
    if(Url.isEmpty())
    {
        QMessageBox msgBox;

        msgBox.setText("Debe añadir un tipo de evento");
        msgBox.setIcon(QMessageBox::Question);
        msgBox.exec();
        return;
    }

    this->setCursor(Qt::BusyCursor);

    if(ModoEdit)
    {
        UpdateEvento();//update
        this->close();
        return;
    }

    NuevoEvento();//add new

    this->close();
}

void AddEventos::NuevoEvento()
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM EVENTOS");

    query.last();

    QSqlRecord rec =  query.record();
    int Cod;
    //Cod=100;
    Cod=rec.value("COD").toInt();

    query.prepare("REPLACE INTO EVENTOS"
                  "(COD, UNAVEZ, CADAHORA, OTRAS,"
                  " INICIOHORA, INICIOFECHA,"
                  " EXPIRACION, EXPIHORA, EXPIFECHA,"
                  " PRIORIDAD,"
                  " COMPTIPO, COMPESPERA, COMMINUTOS,"
                  " LUNES, MARTES, MIERCOLES, JUEVES, VIERNES, SABADO, DOMINGO,"
                  " H0,H1,H2,H3,H4,H5,H6,H7,H8,H9,H10,H11,H12,H13,H14,H15,H16,H17,H18,H19,H20,H21,H22,H23,"
                  " PESTANA,"
                  " TIPO,"
                  " URL,"
                  " VEREXPIRACION,VERCOMIENZO,VERPRIORIDAD,"
                  " VERDIAS, VERHORAS,"
                  " VERFICHERO,"
                  " CONEXION,"
                  " VERHORAINICIO,"
                  " CHEQUEADO,ENPUERTA) VALUES"

                  "(?,?,?,?,"
                  " ?,?,"
                  " ?,?,?,"
                  " ?,"
                  " ?,?,?,"
                  " ?,?,?,?,?,?,?,"
                  " ?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,"
                  " ?,"
                  " ?,"
                  " ?,"
                  " ?,?,?,"
                  " ?,?,"
                  " ?,"
                  " ?,"
                  " ?,"
                  " ?,?)"
                  );
    Cod++;
    QDateTime expi;
    query.addBindValue(Cod);

    //Periodicidad
    query.addBindValue(this->RadioUnaVez->isChecked());
    query.addBindValue(this->RadioCadaHora->isChecked());
    query.addBindValue(this->RadioOtrasHoras->isChecked());

    //date and time
    query.addBindValue(this->TimeHoraInicio->time());

    expi.setDate(DateFechaInicio->date());
    expi.setTime(TimeHoraInicio->time());
    query.addBindValue(expi.toLocalTime());

    //Expiracion
    query.addBindValue(this->ChecExpiracion->isChecked());
    query.addBindValue(this->TimeExpiracion->time());

    expi.setDate(DateExpiracion->date());
    expi.setTime(TimeExpiracion->time());
    query.addBindValue(expi.toLocalTime());

    //PRIORIDAD
    if(RadioPrioridadAlta->isChecked())
    {
        query.addBindValue(true);
    }
    else
    {
        query.addBindValue(false);
    }

    //COMPORTAMIENTO
    query.addBindValue(this->ComboComportamiento->currentIndex());
    query.addBindValue(this->ChecEspera->isChecked());
    query.addBindValue(this->SpinEspera->value());

    //days
    query.addBindValue(this->BtnLunes->isChecked());
    query.addBindValue(this->BtnMartes->isChecked());
    query.addBindValue(this->BtnMiercoles->isChecked());
    query.addBindValue(this->BtnJueves->isChecked());
    query.addBindValue(this->BtnViernes->isChecked());
    query.addBindValue(this->BtnSabado->isChecked());
    query.addBindValue(this->BtnDomingo->isChecked());

    //Horas
    query.addBindValue(w_AddHora->H0->isChecked());
    query.addBindValue(w_AddHora->H1->isChecked());
    query.addBindValue(w_AddHora->H2->isChecked());
    query.addBindValue(w_AddHora->H3->isChecked());
    query.addBindValue(w_AddHora->H4->isChecked());
    query.addBindValue(w_AddHora->H5->isChecked());
    query.addBindValue(w_AddHora->H6->isChecked());
    query.addBindValue(w_AddHora->H7->isChecked());
    query.addBindValue(w_AddHora->H8->isChecked());
    query.addBindValue(w_AddHora->H9->isChecked());
    query.addBindValue(w_AddHora->H10->isChecked());
    query.addBindValue(w_AddHora->H11->isChecked());
    query.addBindValue(w_AddHora->H12->isChecked());
    query.addBindValue(w_AddHora->H13->isChecked());
    query.addBindValue(w_AddHora->H14->isChecked());
    query.addBindValue(w_AddHora->H15->isChecked());
    query.addBindValue(w_AddHora->H16->isChecked());
    query.addBindValue(w_AddHora->H17->isChecked());
    query.addBindValue(w_AddHora->H18->isChecked());
    query.addBindValue(w_AddHora->H19->isChecked());
    query.addBindValue(w_AddHora->H20->isChecked());
    query.addBindValue(w_AddHora->H21->isChecked());
    query.addBindValue(w_AddHora->H22->isChecked());
    query.addBindValue(w_AddHora->H23->isChecked());

    //Pestaña
    query.addBindValue(Pestana);

    //event type
    query.addBindValue(ComboEvento->currentIndex());

    //url
    query.addBindValue(Url);

    //VER
    if(ChecExpiracion->isChecked())
        query.addBindValue(DateExpiracion->text() +"  " + TimeExpiracion->text());
    else
        query.addBindValue("No exprira");

    query.addBindValue(ComboComportamiento->itemText(ComboComportamiento->currentIndex()));

    if(RadioPrioridadAlta->isChecked())
        query.addBindValue(tr("Alta"));
    else
        query.addBindValue(tr("Baja"));

    // VER DIAS / HORAS
    query.addBindValue(DiasToString().toLatin1());
    query.addBindValue(HorasToString().toLatin1());


    //checks file
    if(ComboEvento->currentIndex()==5) // si es radio on-line ponemos este nombre
    {
        query.addBindValue(this->LNombreRadio->text().toLatin1());
        query.addBindValue(this->LConexion->text().toLatin1());//DURACION CONEXION
    }
    else
    {
        QFileInfo UrlName(Url);  //no es radio on-line nombre del fichero sin extension
        query.addBindValue(UrlName.completeBaseName());
        query.addBindValue("");  //DURACION CONEXION
    }

    //VER HORA DE INICIO
    QString verini, min, seg, cero;
    cero="0";

    min= min.setNum(TimeHoraInicio->time().minute());
    seg = seg.setNum(TimeHoraInicio->time().second());

    if(TimeHoraInicio->time().minute() < 10)
    {
        min=cero+min;
    }

    if(TimeHoraInicio->time().second() < 10)
    {
        verini = "__:" + min + ":" + cero + seg;
    }
    else
    {
        verini = "__:" + min + ":"  + seg;
    }

    if(RadioCadaHora->isChecked() or RadioOtrasHoras->isChecked())
    {
        query.addBindValue(verini.toLatin1());
    }
    else
    {
        query.addBindValue(TimeHoraInicio->text());
    }

    //CHEQUEADO/ENPUERTA
    query.addBindValue(2);
    query.addBindValue(false);

    query.exec();

    HoraTotabla(TimeHoraInicio->time(), Cod);
}

//////////////////////////////////////////////////////////////////
void AddEventos::UpdateEvento()
{
    QSqlQuery query(db);
    query.prepare("UPDATE EVENTOS SET"
                  " UNAVEZ = ?, CADAHORA = ?, OTRAS =?,"
                  " INICIOHORA = ?, INICIOFECHA = ?,"
                  " EXPIRACION = ?, EXPIHORA=?, EXPIFECHA=?,"
                  " PRIORIDAD=?,"
                  " COMPTIPO=?, COMPESPERA=?, COMMINUTOS=?,"
                  " LUNES=?, MARTES=?, MIERCOLES=?, JUEVES=?, VIERNES=?, SABADO=?, DOMINGO=?,"
                  " H0=?,H1=?,H2=?,H3=?,H4=?,H5=?,H6=?,H7=?,H8=?,H9=?,H10=?,H11=?,H12=?,"
                  " H13=?,H14=?,H15=?,H16=?,H17=?,H18=?,H19=?,H20=?,H21=?,H22=?,H23=?,"
                  " TIPO=?,"
                  " URL=?,"
                  " VEREXPIRACION=?,VERCOMIENZO=?,VERPRIORIDAD=?,"
                  " VERDIAS=?, VERHORAS=?,"
                  " VERFICHERO = ?,"
                  " CONEXION = ?,"
                  " VERHORAINICIO = ?"
                  " WHERE COD = ?"
                  );

    QDateTime expi;

    //Periodicidad
    query.addBindValue(this->RadioUnaVez->isChecked());
    query.addBindValue(this->RadioCadaHora->isChecked());
    query.addBindValue(this->RadioOtrasHoras->isChecked());

    //fecha/hora
    query.addBindValue(this->TimeHoraInicio->time());
    expi.setDate(DateFechaInicio->date());
    expi.setTime(TimeHoraInicio->time());
    query.addBindValue(expi.toLocalTime());

    //Expiracion
    query.addBindValue(this->ChecExpiracion->isChecked());
    query.addBindValue(this->TimeExpiracion->time());

    expi.setDate(DateExpiracion->date());
    expi.setTime(TimeExpiracion->time());
    query.addBindValue(expi.toLocalTime());

    //PRIORIDAD
    if(RadioPrioridadAlta->isChecked())
        query.addBindValue(true);
    else
        query.addBindValue(false);

    //COMPORTAMIENTO
    query.addBindValue(this->ComboComportamiento->currentIndex());
    query.addBindValue(this->ChecEspera->isChecked());
    query.addBindValue(this->SpinEspera->value());

    //dias
    query.addBindValue(this->BtnLunes->isChecked());
    query.addBindValue(this->BtnMartes->isChecked());
    query.addBindValue(this->BtnMiercoles->isChecked());
    query.addBindValue(this->BtnJueves->isChecked());
    query.addBindValue(this->BtnViernes->isChecked());
    query.addBindValue(this->BtnSabado->isChecked());
    query.addBindValue(this->BtnDomingo->isChecked());

    //Horas
    query.addBindValue(w_AddHora->H0->isChecked());
    query.addBindValue(w_AddHora->H1->isChecked());
    query.addBindValue(w_AddHora->H2->isChecked());
    query.addBindValue(w_AddHora->H3->isChecked());
    query.addBindValue(w_AddHora->H4->isChecked());
    query.addBindValue(w_AddHora->H5->isChecked());
    query.addBindValue(w_AddHora->H6->isChecked());
    query.addBindValue(w_AddHora->H7->isChecked());
    query.addBindValue(w_AddHora->H8->isChecked());
    query.addBindValue(w_AddHora->H9->isChecked());
    query.addBindValue(w_AddHora->H10->isChecked());
    query.addBindValue(w_AddHora->H11->isChecked());
    query.addBindValue(w_AddHora->H12->isChecked());
    query.addBindValue(w_AddHora->H13->isChecked());
    query.addBindValue(w_AddHora->H14->isChecked());
    query.addBindValue(w_AddHora->H15->isChecked());
    query.addBindValue(w_AddHora->H16->isChecked());
    query.addBindValue(w_AddHora->H17->isChecked());
    query.addBindValue(w_AddHora->H18->isChecked());
    query.addBindValue(w_AddHora->H19->isChecked());
    query.addBindValue(w_AddHora->H20->isChecked());
    query.addBindValue(w_AddHora->H21->isChecked());
    query.addBindValue(w_AddHora->H22->isChecked());
    query.addBindValue(w_AddHora->H23->isChecked());


    //EVENTO TIPO
    query.addBindValue(ComboEvento->currentIndex());

    //url
    query.addBindValue(Url);

    //VER
    if(ChecExpiracion->isChecked())
        query.addBindValue(DateExpiracion->text() +"  " + TimeExpiracion->text());
    else
        query.addBindValue("No exprira");

    query.addBindValue(ComboComportamiento->itemText(ComboComportamiento->currentIndex()));

    if(RadioPrioridadAlta->isChecked())
        query.addBindValue(tr("Alta"));
    else
        query.addBindValue(tr("Baja"));

    // VER DIAS / HORAS
    query.addBindValue(DiasToString().toLatin1());
    query.addBindValue(HorasToString().toLatin1());

    //VER FICHERO
    if(ComboEvento->currentIndex()==5) // si es radio on-line ponemos este nombre
    {
        query.addBindValue(this->LNombreRadio->text().toLatin1());
        query.addBindValue(this->LConexion->text().toLatin1());  // CONEXION
    }
    else
    {
        QFileInfo UrlName(Url);  //no es radio on-line nombre del fichero sin extension
        query.addBindValue(UrlName.completeBaseName());
        query.addBindValue("");  //DURACION CONEXION
    }

    //VER HORA DE INICIO
    QString verini, min, seg, cero;
    cero="0";

    min= min.setNum(TimeHoraInicio->time().minute());
    seg = seg.setNum(TimeHoraInicio->time().second());


    if(TimeHoraInicio->time().minute() < 10)
        min=cero+min;

    if(TimeHoraInicio->time().second() < 10)
        verini = "__:" + min + ":" + cero + seg;
    else
        verini = "__:" + min + ":"  + seg;

    if(RadioCadaHora->isChecked() or RadioOtrasHoras->isChecked())
        query.addBindValue(verini.toLatin1());
    else
        query.addBindValue(TimeHoraInicio->text());

    //codigo
    query.addBindValue(Cod);
    query.exec();

    HoraTotabla(TimeHoraInicio->time(), Cod, false);
}

void AddEventos::ClickTipo(int Tipo)
{
    Url="";
    LUrl->setText(Url);  //borramos todo
    this->FrameTipoRadio->setVisible(false); //hora

    //qDebug() <<Tipo;
    switch (Tipo)
    {
        case 0:
            this->FrameTipoFichero->setVisible(true);  //file
            break;

        case 1:
            this->FrameTipoFichero->setVisible(true);  //carpeta
            break;

        case 2:
            this->FrameTipoFichero->setVisible(false); //time
            Url="Locucion Hora.hora";
            break;

        case 3:
            this->FrameTipoFichero->setVisible(false); //temp
            Url="Locucion Temperatura.tempe";
            break;

        case 4:
            this->FrameTipoFichero->setVisible(false); //humedad
            Url="Locucion Humedad.hume";
            break;

        case 5:
            this->FrameTipoFichero->setVisible(false); //radio on-line
            this->FrameTipoRadio->setVisible(true); //hora
            break;
    }
}


////////////////////////////////////// pasa la hora al checkbox
void AddEventos:: HoratoCheckBox()
{
    QCheckBox *hora[24]={w_AddHora->H0,w_AddHora->H1,w_AddHora->H2,w_AddHora->H3,w_AddHora->H4,
                         w_AddHora->H5,w_AddHora->H6,w_AddHora->H7,w_AddHora->H8,w_AddHora->H9,
                         w_AddHora->H10,w_AddHora->H11,w_AddHora->H12,w_AddHora->H13,w_AddHora->H14,
                         w_AddHora->H15,w_AddHora->H16,w_AddHora->H17,w_AddHora->H18,w_AddHora->H19,
                         w_AddHora->H20,w_AddHora->H21,w_AddHora->H22,w_AddHora->H23};

    //for(int i=0;i < 24;i++){
    //  hora[i]->setChecked(false);
    // }

    if(RadioUnaVez->isChecked())// SI SOLO es una hora activa radio una vez
    {
        int HoraActual=TimeHoraInicio->time().hour();
        w_AddHora->Ninguna();
        QCheckBox *cual=hora[HoraActual];
        cual->setChecked(true);
    }

    //   RadioUnaVez->setChecked(true);  // SI SOLO es una hora activa radio una vez
}

void AddEventos::ClickUrl()
{
    if(ComboEvento->currentIndex()==0)
        AddUrl(true);
    else
        AddUrl(false);
}

void AddEventos::AddUrl(bool fichero)
{
    QSettings RaditIni("Radit.ini", QSettings::IniFormat);
    QString Dir=QDir::toNativeSeparators(RaditIni.value("General/Path").toString());
    QString url;


    if(fichero)
    {
        url = QFileDialog::getOpenFileName(0,
                                           QObject::tr("Abrir"),
                                           Dir,
                                           QObject::tr("Todos (*.wav *.mp2 *.mp3 *.ogg *.flac *.wma *.lst *.rot *.seq *.tad);;"
                                                       "Sound file (*.wav *.mp2 *.mp3 *.ogg *.flac *.wma);;"
                                                       "Listas  (*.lst);;"
                                                       "Rotacion  (*.rot);;"
                                                       "Secuencia (*.seq);;"
                                                       "Tanda  (*.tad)"
                                                       ));
    }
    else
    {
        url = QFileDialog::getExistingDirectory(this, tr("Abrir Carpeta"),
                                                Dir,
                                                QFileDialog::ShowDirsOnly
                                                |QFileDialog::DontResolveSymlinks);
    }

    LUrl->setText(url);
    Url=url;
}

void AddEventos::AddRadio()
{
    AddRadioInternet *w_AddRadioInternet = new AddRadioInternet(this);

    if(!LUrlRadio->text().isEmpty())
    {
        w_AddRadioInternet->EditNombre->setText(this->LNombreRadio->text());
        w_AddRadioInternet->EditUrl->setText(LUrlRadio->text());
        w_AddRadioInternet->TimeDuracion->setTime(QTime::fromString(LConexion->text()));
    }

    w_AddRadioInternet->exec();


    if(w_AddRadioInternet->Url.isEmpty())
    {
        delete w_AddRadioInternet;
        return;
    }

    Url=w_AddRadioInternet->Url;
    this->LUrlRadio->setText(w_AddRadioInternet->Url);
    this->LNombreRadio->setText(w_AddRadioInternet->Nombre);
    this->LConexion->setText(w_AddRadioInternet->Duracion);
    delete w_AddRadioInternet;
}

//Combierte los vias en una cadena de días "L,M,MI"
QString AddEventos::DiasToString()
{
    QCheckBox *dias[7]={BtnLunes,BtnMartes,BtnMiercoles,BtnJueves,BtnViernes,
                        BtnSabado,BtnDomingo};
    QString w_Dias;
    int CuantosDias=0;
    //CuantosDias=0;

    for(int i=0;i < 7;i++)
    {
        if(dias[i]->isChecked())
        {
            CuantosDias++;
            QString dia=dias[i]->text();
            dia=dia.left(2);
            w_Dias = w_Dias + dia + " ,";
        }
    }

    if(CuantosDias==7)                   //todos loa dias
        return("Toda la semana");

    //CuantosDias=CuantosDias*4-1;  // quitamos la ultima coma
    w_Dias=w_Dias.left(w_Dias.size()-1);

    return(w_Dias);
}

////////////////////////Combiete las horas en en una cadena de horas (1 2 3 4 to "1,2,3,4")
QString AddEventos::HorasToString()
{
    QCheckBox *horas[24]={w_AddHora->H0,w_AddHora->H1,w_AddHora->H2,w_AddHora->H3,w_AddHora->H4,
                          w_AddHora->H5,w_AddHora->H6,w_AddHora->H7,w_AddHora->H8,w_AddHora->H9,
                          w_AddHora->H10,w_AddHora->H11,w_AddHora->H12,w_AddHora->H13,w_AddHora->H14,
                          w_AddHora->H15,w_AddHora->H16,w_AddHora->H17,w_AddHora->H18,w_AddHora->H19,
                          w_AddHora->H20,w_AddHora->H21,w_AddHora->H22,w_AddHora->H23};

    QString w_Horas;
    int CuantasHoras=0;

    for(int i=0;i < 24;i++)
    {
        if(horas[i]->isChecked())
        {
            CuantasHoras++;
            QString hora=horas[i]->text();
            w_Horas = w_Horas + hora + " ,";
        }
    }

    if(CuantasHoras==24)//all days
        return("Cada hora");

    w_Horas=w_Horas.left(w_Horas.size()-1);

    return(w_Horas);
}

void AddEventos::HoraTotabla(QTime hora, int cod , bool nuevo)
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM HORAS");

    QCheckBox *horas[24]={w_AddHora->H0,w_AddHora->H1,w_AddHora->H2,w_AddHora->H3,w_AddHora->H4,
                          w_AddHora->H5,w_AddHora->H6,w_AddHora->H7,w_AddHora->H8,w_AddHora->H9,
                          w_AddHora->H10,w_AddHora->H11,w_AddHora->H12,w_AddHora->H13,w_AddHora->H14,
                          w_AddHora->H15,w_AddHora->H16,w_AddHora->H17,w_AddHora->H18,w_AddHora->H19,
                          w_AddHora->H20,w_AddHora->H21,w_AddHora->H22,w_AddHora->H23};
    QTime t_Hora;
    QString H;
    QDateTime expi, FechaHora;

    if(!nuevo)
    {
        query.prepare("DELETE FROM HORAS WHERE COD = ?");
        query.addBindValue(cod);
        query.exec();
    }

    QSqlDatabase::database().transaction();

    query.prepare("REPLACE INTO HORAS "
                  "(COD, HORA, HORAT, EXPIRACION, EXPIFECHA, PESTANA) VALUES"
                  "(?,?,?,?,?,?)"
                  );

    for(int i=0;i < 24;i++)
    {
        if(horas[i]->isChecked())
        {
            H=horas[i]->text();
            t_Hora.setHMS(H.toInt(),hora.minute(),hora.second());
            FechaHora.setTime(t_Hora);
            FechaHora.setDate(QDate::currentDate());

            query.addBindValue(cod);
            query.addBindValue(FechaHora.toLocalTime());
            query.addBindValue(t_Hora);

            query.addBindValue(this->ChecExpiracion->isChecked());

            expi.setDate(DateExpiracion->date());
            expi.setTime(TimeExpiracion->time());
            query.addBindValue(expi.toLocalTime());

            query.addBindValue(Pestana);
            query.exec();
        }

    }

    QSqlDatabase::database().commit();
    DiaTotabla(cod, nuevo);
}

void AddEventos::DiaTotabla(int cod, bool nuevo)
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM HORAS");

    QCheckBox *dias[7]={BtnLunes,BtnMartes,BtnMiercoles,BtnJueves,BtnViernes,
                        BtnSabado,BtnDomingo};

    QDateTime expi;

    if(!nuevo)
    {
        query.prepare("DELETE FROM DIAS WHERE COD = ?");
        query.addBindValue(cod);
        query.exec();
    }

    QSqlDatabase::database().transaction();

    query.prepare("REPLACE INTO DIAS "
                  "(COD, DIA, EXPIRACION, EXPIFECHA, PESTANA) VALUES"
                  "(?,?,?,?,?)"
                  );

    for(int i=0;i < 7;i++)
    {
        if(dias[i]->isChecked())
        {
            query.addBindValue(cod);
            query.addBindValue(i+1); //codigo del dia siendo 1-lunes 7-domingo
            query.addBindValue(this->ChecExpiracion->isChecked());


            expi.setDate(DateExpiracion->date());
            expi.setTime(TimeExpiracion->time());
            query.addBindValue(expi.toLocalTime());

            query.addBindValue(Pestana);
            query.exec();
        }
    }

    QSqlDatabase::database().commit();
}

void AddEventos::PoderDatos(int cod)
{

     ModoEdit=true;
    Cod=cod;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM EVENTOS WHERE COD = ?");
    query.addBindValue(cod);
    query.exec();
    query.first();
    QSqlRecord rec =  query.record();

    ClickTipo(rec.value("TIPO").toInt());

    // prioridad
    this->RadioUnaVez->setChecked(rec.value("UNAVEZ").toBool());
    this->RadioCadaHora->setChecked(rec.value("CADAHORA").toBool());
    this->RadioOtrasHoras->setChecked(rec.value("OTRAS").toBool());

    //fecha/hora
    this->TimeHoraInicio->setTime(rec.value("INICIOHORA").toTime());
    this->DateFechaInicio->setDate(rec.value("INICIOFECHA").toDate());

    //expiracion
    this->ChecExpiracion->setChecked(rec.value("EXPIRACION").toBool());
    this->TimeExpiracion->setTime(rec.value("EXPIHORA").toTime());
    this->DateExpiracion->setDate(rec.value("EXPIFECHA").toDate());

    //PRIORIDAD

    bool prioridad=rec.value("PRIORIDAD").toBool();

    if(prioridad)
        RadioPrioridadAlta->setChecked(true);
    else
        RadioPrioridadBaja->setChecked(true);

    //compartamiento******************************************************

    this->ComboComportamiento->setCurrentIndex(rec.value("COMPTIPO").toInt());
    this->ChecEspera->setChecked(rec.value("COMPESPERA").toBool());
    this->SpinEspera->setValue(rec.value("COMMINUTOS").toInt());

    //dias
    this->BtnLunes->setChecked(rec.value("LUNES").toBool());
    this->BtnMartes->setChecked(rec.value("MARTES").toBool());
    this->BtnMiercoles->setChecked(rec.value("MIERCOLES").toBool());
    this->BtnJueves->setChecked(rec.value("JUEVES").toBool());
    this->BtnViernes->setChecked(rec.value("VIERNES").toBool());
    this->BtnSabado->setChecked(rec.value("SABADO").toBool());
    this->BtnDomingo->setChecked(rec.value("DOMINGO").toBool());


    //Horas
    w_AddHora->H0->setChecked(rec.value("H0").toBool());
    w_AddHora->H1->setChecked(rec.value("H1").toBool());
    w_AddHora->H2->setChecked(rec.value("H2").toBool());
    w_AddHora->H3->setChecked(rec.value("H3").toBool());
    w_AddHora->H4->setChecked(rec.value("H4").toBool());
    w_AddHora->H5->setChecked(rec.value("H5").toBool());
    w_AddHora->H6->setChecked(rec.value("H6").toBool());
    w_AddHora->H7->setChecked(rec.value("H7").toBool());
    w_AddHora->H8->setChecked(rec.value("H8").toBool());
    w_AddHora->H9->setChecked(rec.value("H9").toBool());
    w_AddHora->H10->setChecked(rec.value("H10").toBool());
    w_AddHora->H11->setChecked(rec.value("H11").toBool());
    w_AddHora->H12->setChecked(rec.value("H12").toBool());
    w_AddHora->H13->setChecked(rec.value("H13").toBool());
    w_AddHora->H14->setChecked(rec.value("H14").toBool());
    w_AddHora->H15->setChecked(rec.value("H15").toBool());
    w_AddHora->H16->setChecked(rec.value("H16").toBool());
    w_AddHora->H17->setChecked(rec.value("H17").toBool());
    w_AddHora->H18->setChecked(rec.value("H18").toBool());
    w_AddHora->H19->setChecked(rec.value("H19").toBool());
    w_AddHora->H20->setChecked(rec.value("H20").toBool());
    w_AddHora->H21->setChecked(rec.value("H21").toBool());
    w_AddHora->H22->setChecked(rec.value("H22").toBool());
    w_AddHora->H23->setChecked(rec.value("H23").toBool());

    //EVENTO TIPO
    ComboEvento->setCurrentIndex(rec.value("TIPO").toInt());

    if(ComboEvento->currentIndex()==5)// si es radio on-line ponemos este nombre
    {
        this->LNombreRadio->setText(rec.value("VERFICHERO").toString());
        this->LConexion->setText(rec.value("CONEXION").toString());
        this->LUrlRadio->setText(rec.value("URL").toString());
    }

    //url
    LUrl->setText(rec.value("URL").toString());
    Url=rec.value("URL").toString();
}
