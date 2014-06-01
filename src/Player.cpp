/**
 * Player
 * -----------------------------------------
 *
 * This main Window of Radit.
 * Has conditions to signal to player.
 *
 * @author Victor Algaba
 **/
#include <QDebug>
#include <QDockWidget>

#include "Player.h"
#include "bass.h"
#include "PlayerSetting.h"

#include "micelanea/BotonEstado.h"
#include "micelanea/ListaEstado.h"
#include "micelanea/Aleatorio.h"
#include "micelanea/Dock.h"

#include "iolista/IoLista.h"

#include "engine/Pisadores.h"
#include "engine/StreamMath.h"
#include "PlayerGrupos.h"
#include "AddRadioInternet.h"


Player::Player(QString w_Prefijo, QMainWindow *parent )
    :QMainWindow(parent)
{
    setupUi(this);

    //menuFile
    connect(actionNuevo, SIGNAL(triggered()), this, SLOT(ClickNuevo()));
    connect(actionAbrir, SIGNAL(triggered()), this, SLOT(ClickAbrir()));
    connect(actionGuardar_Como, SIGNAL(triggered()), this, SLOT(ClickGuardarComo()));
    connect(actionGuardar, SIGNAL(triggered()), this, SLOT(ClickGuardar()));

    //menu ver
    connect(actionFichero, SIGNAL(triggered()), this, SLOT(ShowToolBar()));
    connect(actionLista,   SIGNAL(triggered()), this, SLOT(ShowToolBar()));

    //menu editar
    connect(actionCortar, SIGNAL(triggered()), this->w_Lista, SLOT(ClickCortar()));
    connect(actionCopiar, SIGNAL(triggered()), this->w_Lista, SLOT(ClickCopiar()));
    connect(actionAddPisadores, SIGNAL(triggered()), this->w_Lista, SLOT(ClickAddPisador()));

    connect(actionPegar,  SIGNAL(triggered()), this->w_Lista, SLOT(ClickPegar()));
    connect(actionBorrar, SIGNAL(triggered()), this, SLOT(ClickBorrar()));

    //menu Heramientas
    connect(actionPreferencias, SIGNAL(triggered()), this, SLOT(ClickPreferencias()));

    //menu Lista
    connect(actionAddPista, SIGNAL(triggered()), this->w_Lista, SLOT(AddPista()));  //pista
    connect(actionActualizarSeleccion, SIGNAL(triggered()), this->w_Lista, SLOT(DuracionSeleccion()));  //actualizar duraciones

     //hth
    connect(actionAddHora, SIGNAL(triggered()), this->w_Lista, SLOT(AddHora()));
    connect(actionAddTemperatura, SIGNAL(triggered()), this->w_Lista, SLOT(AddTemperatura()));
    connect(actionAddHumedad, SIGNAL(triggered()), this->w_Lista, SLOT(AddHumedad()));

    // radio internet
    connect(actionAddRadioInternet, SIGNAL(triggered()), this, SLOT(AddRadio()));

    //botonera reproductor
    connect(BtnPlay,SIGNAL(clicked()),this, SLOT(ClickBtnStopPlay()));  //duplicado
    connect(BtnStop,SIGNAL(clicked()),this, SLOT(ClickStop()));
    connect(BtnPause,SIGNAL(clicked()),this, SLOT(ClickPause()));
    connect(BtnAtras,SIGNAL(clicked()),this, SLOT(ClickAtras()));
    connect(BtnAlante,SIGNAL(clicked()),this, SLOT(ClickAlante()));
    connect(BtnParar,SIGNAL(clicked()),this, SLOT(ClickParar()));
    connect(BtnStopPlay,SIGNAL(clicked()),this, SLOT(ClickBtnStopPlay()));
    connect(BtnPisador,SIGNAL(clicked()),this, SLOT(ClickPisador()));

    //combo modos
    connect(ComboModo, SIGNAL(currentIndexChanged(int)), this, SLOT(ClickModo()));

    //doble click en la lista
    connect(w_Lista,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(ClickLista()));

    QProgressBar *progressBar = new QProgressBar();
    LTotal = new QLabel("00:00:00");

    LTotal->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    statusBar()->addWidget( progressBar,1 );
    statusBar()->addWidget( LTotal,1);

    progressBar->setVisible(false);
    w_Lista->progressBar=progressBar;   //gestiona la baara de progreso
    w_Lista->LTotalTiempo=LTotal;       // gestiona la etiqueta del tiempo total
    Prefijo=w_Prefijo;  //estable prefijo para las configuraciones

    w_Stream = new  Stream(this);

    w_Stream->SetVumeter(this->Vumeter);
    w_Stream->SetLabel(this->Label);
    w_Stream->SetSlider(this->Slider);
    w_Stream->SetTitulo(this->LTitulo);

    w_Stream->Configuracion(Prefijo);

    // final del streamer
    connect(w_Stream,SIGNAL(Finish()),this, SLOT(Final()));

    //Pisadores
    connect(w_Stream,SIGNAL(PisadorIn()),this, SLOT(ClickPisadorIn()));
    connect(w_Stream,SIGNAL(PisadorOut()),this, SLOT(ClickPisadorOut()));

    //
    w_Pisador = new Pisador(this) ;        //fader out y in

    //hth
    w_Hth = new Hth(this);           // locuciones hth
    connect(w_Hth, SIGNAL(Finish()), this, SLOT(Final()));

    IsPause=false;
    IsGrupo=false;

    NombreLista="Sinnombre.lst";

    IsEventoEspera=false;
    IsFinal=false;

    qsrand(QTime::currentTime().msec());
}

Player::~Player()
{
    QSettings RaditIni("Radit.ini", QSettings::IniFormat);
    RaditIni.setValue("General/Principal",this->saveGeometry());
}

/**
 * Enable ferramentas bars
 * @brief Player::ShowToolBar
 * @return void
 */
void Player::ShowToolBar()
{
    if(actionFichero->isChecked())
        toolBar->show();
    else
        toolBar->hide();

    if(actionLista->isChecked())
        toolBar2->show();
    else
        toolBar2->hide();
}


void Player::closeEvent(QCloseEvent *e)
{
   // e->ignore();
   //e->accept();
}

/**
 * This function has conditions to play button
 * @brief Player::ClickPlay
 * @return void
 */
void Player::ClickPlay()
{
   // w_Stream->Load("C:/discoteca/NOVEDADES DEL MES/KIKE GABANA - Dímelo Bajito.mp3");


       //Start("C:/discoteca/NOVEDADES DEL MES/KIKE GABANA - Dímelo Bajito.mp3",false);
   // return;

    if(!w_Lista->rowCount())//if have zero elements on list
        return;

    BotonEstado *w_BotonEstado = new BotonEstado(this->BtnPlay);

    if(IsPause)
    {
        w_Stream->Load(" ");
        w_BotonEstado->SetButton(this->BtnPause);
        w_BotonEstado->SetInactivo();
        delete w_BotonEstado;
        IsPause=false;
        return;
    }

    ListaEstado *w_ListaEstado = new ListaEstado(this->w_Lista);
    w_ListaEstado->SetItem(w_ListaEstado->Rojo());//delete the previous red item
    w_ListaEstado->Borrar();

    if(w_ListaEstado->Verde()==-1) //return if dont have green song
    {
        ClickStop();  //make stop
        delete w_BotonEstado;
        delete w_ListaEstado;
        return;
    }



  QString url=this->w_Lista->item(w_ListaEstado->Verde(), 4)->text();  //read file


    /////////////////////
    w_BotonEstado->SetActivo();
    w_ListaEstado->SetItem(w_ListaEstado->Verde());
    w_ListaEstado->SetRojo();

    if(ComboModo->currentIndex()!=1)//if in MANUAL mark as green
         PonerVerde();



Start(url,false);

  /*if(w_Lista->item(w_ListaEstado->Rojo(), 1))
    {
        setIndicadores(w_Lista->item(w_ListaEstado->Rojo(), 1)->text(),true);
    }
    else
    {
        setIndicadores(w_Lista->item(w_ListaEstado->Verde(), 1)->text(),true);
    }*/






    delete w_BotonEstado;
    delete w_ListaEstado;



   //  Start(url,false);

}

/**
 * This function has conditions to Play or PlayNext button
 * @brief Player::ClickBtnStopPlay
 * @return void
 */


////////////////////////////////////////////////////////////////////////
void Player::ClickBtnStopPlay()
{
    if(IsGrupo)
    {
        this->removeDockWidget(w_Dock);
        delete w_PlayerGrupos;
        delete w_Dock;
        IsGrupo=false;
     }

    if(IsEventoEspera)//esto es por e pisador de salida que llega aqui tras sonar
    {
        Final();
        return;
    }

    if(!IsPause)
    {
        w_Stream->StopA();
        w_Stream->StopB();
    }

    ClickPlay();
}



/**
 * This function has conditions to stop button
 * @brief Player::ClickStop
 * @return void
 */
void Player::ClickStop()
{
    if(IsGrupo)
    {
        this->removeDockWidget(w_Dock);
        delete w_PlayerGrupos;
        delete w_Dock;
        IsGrupo=false;
   }

   w_Stream->StopA();
   w_Stream->StopB();

   BotonEstado *w_BotonEstado = new BotonEstado(this->BtnPlay);
   w_BotonEstado->SetInactivo();
   w_BotonEstado->SetButton(this->BtnPause);
   w_BotonEstado->SetInactivo();

   ListaEstado *w_ListaEstado = new ListaEstado(this->w_Lista);
   w_ListaEstado->SetItem(w_ListaEstado->Rojo());
   w_ListaEstado->Borrar();//lo borramos

   delete w_BotonEstado;
   delete w_ListaEstado;
}

/**
 * This function has conditions to pause button
 * @brief Player::ClickPause
 * @return void
 */
void Player::ClickPause()
{
   if(IsGrupo)
       return;

   w_Stream->PauseA();
   w_Stream->PauseB();

   BotonEstado *w_BotonEstado = new BotonEstado(this->BtnPause);
   w_BotonEstado->SetActivo();
   delete w_BotonEstado;
   IsPause=true;
}

/**
 * This function has conditions to return button
 * @brief Player::ClickAtras
 * @return void
 */
void Player::ClickAtras()
{
    w_Stream->Retroceso();
}

/**
 * This function has conditions to next button
 * @brief Player::ClickAlante
 * @return void
 */
void Player::ClickAlante()
{
    w_Stream->Avance();
}

/**
 * This function has conditions to Parar button
 * - dont play next song
 * @brief Player::ClickParar
 * @return void
 */
void Player::ClickParar()
{
    ListaEstado *w_ListaEstado = new ListaEstado(this->w_Lista);

    w_ListaEstado->SetItem(w_ListaEstado->Verde());
    w_ListaEstado->Borrar();
    this->LSiguiente->setText("");

    delete  w_ListaEstado;
}

/**
 * This function has conditions to pisador button
 * @brief Player::ClickPisador
 * @return void
 */
void Player::ClickPisador()
{
    w_Stream->pisador();
}



/**
 * This  function select mode normal or manual
 * @brief Player::ClickModo
 * @return void
 */
void Player::ClickModo()
{
    ListaEstado *w_ListaEstado = new ListaEstado(this->w_Lista);

    if(ComboModo->currentIndex()==0)//normal
    {
        if(w_ListaEstado->Verde()!=-1) //si hay verde nos vamos
            return;

        PonerVerde();

        delete w_ListaEstado;
        return;
    }

    if(ComboModo->currentIndex()==1)//manual
        ClickParar();    
}

/**
 * @brief Player::Final
 */
void Player::Final()
{
    if(IsGrupo)//si se esta reproduciendo un grupo se va
      return;

    if(ComboModo->currentIndex()==1)//manual
    {
        ClickStop();
        return;
    }

    if(IsEventoEspera)//reproducimos evento en espera
    {
        QuitarRojo();
        IsEventoEspera=false;
        emit borrarEvento();
        Start(UrlEvento,true);  //hacemos play
        return;
    }

    ClickPlay();
}


/////////////////////////////////////////////////////////////////////////////////
void Player::ClickLista()
{
    ListaEstado *w_ListaEstado = new ListaEstado(this->w_Lista);

    if(w_ListaEstado->Verde() !=-1)  //vemos si hay verde
    {
        w_ListaEstado->SetItem(w_ListaEstado->Verde());  //aberiguamos el verde
        w_ListaEstado->Borrar();                            //lo borramos
    }

    w_ListaEstado->SetItem(w_Lista->currentRow());  //el item nuevo
    w_ListaEstado->SetVerde();   //ponemos verde

    this->LSiguiente->setText(this->w_Lista->item(w_ListaEstado->Verde(), 1)->text());
    delete w_ListaEstado;

    setIndicadores(w_Lista->item(w_ListaEstado->Verde(), 1)->text(),false);
}

///////////////////////////////////////////////////////////////////////////////////

void Player:: PonerVerde(){


    if(!w_Lista->rowCount()){return;} //si esta vacia nos vamos


    ListaEstado *w_ListaEstado = new ListaEstado(this->w_Lista);



       if(w_Lista->rowCount()==w_ListaEstado->Rojo()+1){  // es el ultimo
           w_ListaEstado->SetItem(0);
       }else{
           w_ListaEstado->SetItem(w_ListaEstado->Rojo()+1);

       }

       w_ListaEstado->SetVerde();

        this->LSiguiente->setText(this->w_Lista->item(w_ListaEstado->Verde(), 1)->text());


       QString Ahora;

       Ahora=w_Lista->item(w_ListaEstado->Verde(), 1)->text();

       setIndicadores(w_Lista->item(w_ListaEstado->Verde(), 1)->text(),false);



delete w_ListaEstado;


}


//////////////////////////////////////////////////////
 void Player::ClickBorrar(){

     if(!w_Lista->rowCount()){return;} //si esta vacia nos vamos
     if(w_Lista->currentRow()==-1){return;} // si no hay ninguno marcado


     ListaEstado *w_ListaEstado = new ListaEstado(w_Lista);
      w_ListaEstado->SetItem(w_Lista->currentRow());




      if(w_ListaEstado->Verde()==w_Lista->currentRow()){
        w_Lista->removeRow(w_Lista->currentRow());
         if(!w_Lista->rowCount()){return;} //si esta vacia nos vamos

         w_Lista->setCurrentCell(w_Lista->currentRow(),1); //lo posicionamos
         w_ListaEstado->SetItem(w_Lista->currentRow());
         w_ListaEstado->SetVerde();
         LSiguiente->setText(this->w_Lista->item(w_ListaEstado->Verde(), 1)->text());
        }else{
         w_Lista->removeRow(w_Lista->currentRow());
         w_Lista->setCurrentCell(w_Lista->currentRow(),1); //lo posicionamos

        }



     setFocus(); // lo emfocamos
     delete w_ListaEstado;

 }


 /////////////////////////////////////////////////
 void Player::AddRadio(){

     AddRadioInternet *w_AddRadioInternet = new AddRadioInternet(this);
     w_AddRadioInternet->Dispositivo=w_Stream->Dispositivo;

     w_AddRadioInternet->exec();


     if(w_AddRadioInternet->Url.isEmpty()){
        return;
        delete w_AddRadioInternet;
       }

     const int pos =  w_Lista->rowCount();
     w_Lista->setRowCount(pos + 1);



     w_Lista->setItem(pos, 0, new QTableWidgetItem(w_AddRadioInternet->Url));
     w_Lista->setItem(pos, 1, new QTableWidgetItem(w_AddRadioInternet->Nombre)); //nombre de la cancion
     w_Lista->setItem(pos, 2, new QTableWidgetItem(w_AddRadioInternet->Duracion));// duracion con formato
     w_Lista->item(pos, 2 )->setTextAlignment(Qt::AlignRight); // justificamos a la derecha  el tiempo

     w_Lista->setItem(pos, 3, new QTableWidgetItem(w_AddRadioInternet->Url));// duracion en segundos******* carbiarlo
     w_Lista->setItem(pos, 4, new QTableWidgetItem(w_AddRadioInternet->Url));// direccion paht del fichero

     w_Lista->setItem(pos, 5, new QTableWidgetItem(""));// pisador tiempo de in
     w_Lista->setItem(pos, 6, new QTableWidgetItem(""));//  pisador in url
     w_Lista->setItem(pos, 7, new QTableWidgetItem(""));//  pisador out url

     QIcon icono;
     icono.addFile(":/ficheros/iconos/ficheros/online.png");
     w_Lista->item(pos, 0)->setIcon(icono);



    delete w_AddRadioInternet;

 }




////////////////////////////////////////////////

void Player::ClickPreferencias(){

      PlayerSetting *w_PlayerSetting = new PlayerSetting(Prefijo);
      w_PlayerSetting->exec();

      w_Stream->Configuracion(Prefijo); //establecmos las propidades

     delete w_PlayerSetting;
}




/////////////// menu file ////////////////

//menu File**************
void Player::ClickNuevo(){
    w_Lista->setRowCount(0);
  //  w_Lista->TotalTiempo=0;

    NombreLista="Sinnombre.lst";

   QString Titulo = windowTitle() + " - [ " + NombreLista + " ]";
     Dock->setWindowTitle(Titulo);

}



//////////////////// abrir lista /////////////////////////////////////
void Player::ClickAbrir(){
    IoLista *w_IoLista= new IoLista(this->w_Lista);
w_IoLista->Abrir();

QString Titulo;

Titulo = windowTitle() + " - [ " + w_IoLista->Nombre.toLatin1() + " ]";



         if(!w_IoLista->Nombre.toLatin1().isNull()){
            Dock->setWindowTitle(Titulo);
            NombreLista=w_IoLista->Nombre.toLatin1(); // establece el nnuevo nombre
            PonerVerde();
            }

delete w_IoLista;



}


//************** gruarda la lista en el reproductor

void Player::ClickGuardarComo(){

    if(!w_Lista->rowCount()){return;} //si esta vacia nos vamos


    IoLista *w_IoLista= new IoLista(this->w_Lista);
    w_IoLista->Nombre=NombreLista;
    w_IoLista->Guardar();

QString Titulo;
Titulo = windowTitle() + " - [ " + w_IoLista->Nombre.toLatin1() + " ]";
Dock->setWindowTitle(Titulo);


delete w_IoLista;

}


//////////////////////////////////////////////////////////


void Player::ClickGuardar(){

    if(!w_Lista->rowCount()){return;} //si esta vacia nos vamos


    IoLista *w_IoLista= new IoLista(this->w_Lista);
   // w_IoLista->Nombre=NombreLista;
    w_IoLista->Guardar(NombreLista);


delete w_IoLista;

}



void Player::resizeEvent( QResizeEvent *event){



}

////////////Señal que viene del streamer///////////////////////

void Player::ClickPisadorIn(){

     if(IsGrupo){return;}



    Pisadores *w_Pisadores = new Pisadores(UrlPisadorIn.toLatin1(),this);  //!!!!goteo**************
    w_Pisadores->Configuracion(Prefijo);
    w_Pisadores->Prefijo=this->Prefijo;
    connect(w_Pisadores , SIGNAL(Iniciar()), this, SLOT(ClickFaderOut()));
    connect(w_Pisadores , SIGNAL(Finish()), this, SLOT(ClickFaderIn()));

    w_Pisadores->Start();


}


void Player::ClickPisadorOut(){

 if(IsGrupo){return;}
       Pisadores *w_Pisadores = new Pisadores(UrlPisadorOut.toLatin1(),this);    //!!!!goteo**************
       w_Pisadores->Configuracion(Prefijo);
       w_Pisadores->Prefijo=this->Prefijo;
       connect(w_Pisadores , SIGNAL(Iniciar()), this, SLOT(ClickFaderOut()));
       connect(w_Pisadores , SIGNAL(Finish()), this, SLOT(ClickBtnStopPlay()));
       // connect(w_Pisadores , SIGNAL(Finish()), this, SLOT(ClickFaderIn()));
       w_Pisadores->Start();



}

////////////////////////////////////////////////////////////////////////////////
void Player::ClickFaderOut(){

    w_Pisador ->setPrefijo(Prefijo);
    w_Pisador ->setStream(w_Stream->streamUltimo);
    w_Pisador ->setEstado(true);
    w_Pisador ->start();



}

void Player::ClickFaderIn(){

    w_Pisador ->setPrefijo(Prefijo);
    w_Pisador ->setStream(w_Stream->streamUltimo);
    w_Pisador ->setEstado(false);
    w_Pisador ->start();


}




//////////////////////////////////////////////////////////////////////////////////
void Player::contextMenuEvent(QContextMenuEvent *event)
 {
    QMenu *menu = new QMenu(this);

     // QIcon *icono;
     // QAction *Act;

      menu->setStyleSheet(QString::fromUtf8("\n"
                                            " QMenu {\n"
                                            " background-color: rgb(34, 34, 34);\n"
                                            " color: rgb(255, 255, 255);\n"
                                            " border: 1px solid black;\n"
                                            " }\n"

                                            "\n"
                                            " QMenu::item {\n"
                                            "  \n"
                                            " background-color: transparent;\n"
                                            " }\n"

                                            "\n"
                                            " QMenu::item:selected { \n"
                                            "\n"
                                            "background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(129, 166, 226, 255), stop:1 rgba(93, 131, 190, 255));\n"
                                            " }\n"
                                            "\n"
                                            ));




         menu->addAction(actionNuevo);
         menu->addAction(actionAbrir);
         menu->addAction(actionGuardar);
         menu->addSeparator();//separador
         menu->addAction(actionBorrar);
         menu->addAction(actionCortar);
         menu->addAction(actionCopiar);
         menu->addAction(actionPegar);
         menu->addSeparator();//separador
         menu->addAction(actionAddPisadores);

  menu->exec(event->globalPos());

delete menu;

}

/////////////////////////////////////////////////////////////////////

void Player::setIndicadores(QString cual, bool OnAir ){


   if(!OnAir){

    w_Indicadores->TSiguiente->setPlainText(cual);
     return;

   }else{
       w_Indicadores->TOnAir->setPlainText(cual);
   }


  // QString Song=this->LTitulo->text().toLatin1();

QString Song=cual;

   QDateTime dateTime = QDateTime::currentDateTime();
   QString Hora = dateTime.toString("hh:mm:ss");
   QString Fecha = dateTime.toString("dd-MM-yy");

   QString Path=QCoreApplication::applicationDirPath().toLatin1();

   QFile file(Path + "/Logs/" + Fecha + ".txt");
         file.open(QIODevice::WriteOnly| QIODevice::ReadOnly| QIODevice::Text);



        QTextStream out(&file);

      while (!out.atEnd()) {
             QString line = out.readLine();

           }



        out << Hora + " "<< Song;
        out << "\n";


       file.close();
       w_Indicadores->setAdd(dateTime.toString("dd-MM-yy hh:mm:ss") + " " + Song);
}


/////////////////////////////////////////////////
void Player::PlayGrupos(QString Url){


 QFileInfo w_FileInfo(Url);
 QSplitter *splitter = new QSplitter(this);
 splitter->setOrientation(Qt::Vertical);


    w_Dock = new QDockWidget(this);
    w_Dock->setFeatures(QDockWidget::DockWidgetMovable);
    w_Dock->setAllowedAreas(Qt::LeftDockWidgetArea |Qt::RightDockWidgetArea);

   w_Dock->setStyleSheet(QString::fromUtf8("/* background-color: rgb(33, 48, 58);*/\n"
    "\n"
    " QDockWidget {\n"
    "	color: rgb(255, 255, 255);\n"
    "     \n"
    " }\n"
    "\n"
    " QDockWidget::title {\n"
    "   \n"
    "    \n"
    "	background-color: rgb(33, 135, 207);\n"
    "	\n"
    "	\n"
    "   \n"
    " }\n"
    "\n"
    ""));

    w_PlayerGrupos = new PlayerGrupos(this->Prefijo,this);


    QString Titulo;
    QFileInfo File(Url);   //este puede sobrar--------------
    Titulo=File.baseName().toLatin1();

    w_Dock->setWindowTitle(" " + Titulo);

    w_PlayerGrupos->setRotacion(Url);
    w_PlayerGrupos->setStream(w_Stream,0);
    w_PlayerGrupos->splitter=splitter;

    if(w_FileInfo.suffix().toLower()=="tad"){          //tanda
         w_PlayerGrupos->IsTanda=true;
       } else{
          w_PlayerGrupos->IsTanda=false;
       }

    w_PlayerGrupos->setMinimumSize(311,0);

    w_PlayerGrupos->w_Row=0; // lo fijamos al principio
    w_PlayerGrupos->Start();
    IsGrupo=true;
    splitter->addWidget(w_PlayerGrupos);

     // w_Dock->setFeatures(QDockWidget::DockWidgetFloatable);
      w_Dock->setWidget(splitter);
     // w_Dock->setFloating(true);

  // this->addDockWidget(Qt::RightDockWidgetArea, w_Dock,Qt::Vertical);
   MainWindow->addDockWidget(Qt::LeftDockWidgetArea, w_Dock,Qt::Horizontal);


    connect(w_PlayerGrupos,SIGNAL(Finish()),this, SLOT(FinalGrupo()));



}


///////////////////////////////////////////////////
void Player::FinalGrupo(){

  //  QObject* obj = QObject::sender();
 // PlayerGrupos *w_Radio = qobject_cast<PlayerGrupos *>(obj);


   this->removeDockWidget(w_Dock);
   IsGrupo=false;

   Final();

}

////////////////////////////////////////////////////////////
void Player::PlayHth(int Tipo){




    w_Hth->Configuracion(Prefijo);

    if (Tipo==1){w_Hth->StartHora();}  // tipo 1 hora
    if (Tipo==2){w_Hth->StartTemp();}  // tipo 1 hora
    if (Tipo==3){w_Hth->StartHumedad();}  // tipo 1 hora

    ClickStop();  //hacemos stop para dar la hora

}



/// ========================================================================

/////////////////////////////////////////////////////////
 void Player::Start(QString Url,bool IsEvento){


     BotonEstado *w_BotonEstado = new BotonEstado(this->BtnPlay);
     ListaEstado *w_ListaEstado = new ListaEstado(this->w_Lista);
     QFileInfo w_FileInfo(Url);

     w_Stream->IsPisadorIn=false;  //falsos de inicio
     w_Stream->IsPisadorOut=false;


setIndicadores(QFileInfo(Url).baseName(),true);


     ///hth**************************************************************

 if(w_FileInfo.suffix().toLower()=="hora" || w_FileInfo.suffix().toLower()=="tempe" || w_FileInfo.suffix().toLower()=="hume" ){

     w_Stream->PuestaCero();//ponemos el vumetro a 0

     if(w_FileInfo.suffix().toLower()=="hora"){ PlayHth(1);}
     if(w_FileInfo.suffix().toLower()=="tempe"){ PlayHth(2);}
     if(w_FileInfo.suffix().toLower()=="hume"){ PlayHth(3);}

       delete w_BotonEstado;
       delete w_ListaEstado;

    return;

 }


 ///////////Listas lst*******************************************************

 if(w_FileInfo.suffix().toLower()=="lst") {  //listas

     QFile file;
     IoLista *w_IoLista= new IoLista(this->w_Lista);

     file.setFileName(Url);


   if(file.open(QIODevice::ReadOnly))
   w_IoLista->loadModel(&file, this->w_Lista);

   file.close();

   delete w_IoLista;
   delete w_BotonEstado;
   delete w_ListaEstado;

    PonerVerde();
    ClickBtnStopPlay();

   //Path(fileName);
   //Nombre = QFileInfo(fileName).fileName();

     return;
 }


 /////////// grupos********************************************************************
 if(w_FileInfo.suffix().toLower()=="seq" ||w_FileInfo.suffix().toLower()=="tad" ){  //secuencias/ tandas

     PlayGrupos(Url);
     delete w_BotonEstado;
     delete w_ListaEstado;
     return;
 }

////////////////////Ficheros / carpetas ///////////////////////////////////////////////
     if(w_FileInfo.isDir()){                 //es una carpeta musical
         Aleatorio *w_Aleatorio= new Aleatorio(Url,LTotal);
         Url=w_Aleatorio->FicheroPath();
         delete w_Aleatorio;
        }



      w_Stream->IsTanda=false;  //falso por defecto en la lista principal
      w_Stream->Load(Url);



      int ItemRow;

        if(w_ListaEstado->Rojo()==-1){  //si no hay item rojo
            ItemRow=w_ListaEstado->Verde();
        }else{

             ItemRow=w_ListaEstado->Rojo();

        }



 ///////////// radio on-line*************************************************************


        if(w_Stream->IsRadioOnLine){  // es una radio on-line ajustamos el tiempo de emision

            StreamMath *w_StreamMath = new StreamMath;

            if(!IsEvento){  //si no es evento
               w_Stream->TiempoRadioOnLine=w_StreamMath->FormatoToSegundos(w_Lista->item(ItemRow, 2)->text());
            }else{
                w_Stream->TiempoRadioOnLine=w_StreamMath->FormatoToSegundos(EventoConexion.toLatin1()); // si es un evento cojemos conexion

            }
            delete w_StreamMath;
            delete w_BotonEstado;
            delete w_ListaEstado;

           return;

        }



///////////////pisadores
        if(!w_Lista->rowCount()){return;} //si esta vacia nos vamos
        if(w_ListaEstado->Rojo()==-1){return;}//si no hay item rojo es por los eventos



        if(w_Lista->item(ItemRow, 6)->text()!=""){ //si hay algo en el urlin del pisador
            w_Stream->IsPisadorIn=true;
            w_Stream->PisadorSegundos=w_Lista->item(ItemRow, 5)->text().toInt();//tiempo de inicio del pisador
           UrlPisadorIn=w_Lista->item(ItemRow, 6)->text();

          }

        if(w_Lista->item(ItemRow, 7)->text()!=""){ //si hay algo en el urlin del pisador
              w_Stream->IsPisadorOut=true;
              UrlPisadorOut=w_Lista->item(ItemRow, 7)->text();

            }




    delete w_BotonEstado;
    delete w_ListaEstado;





 }


////////////////////////////////////////////////////////////////////////////////
 void Player::Evento(QString Url, int tipo, QString Duracion){ //aqui llegan los eventos procedente de eventos

    EventoConexion=Duracion;

     if(tipo==1){  //inmediato
        QuitarRojo();
        emit borrarEvento();
        ClickStop(); //hacemos stop
        setIndicadores(QFileInfo(Url).baseName(),true);
        Start(Url,true);  //hacemos play
        return;
     }

     if(tipo==0){ // retardado
         IsEventoEspera=true;
         UrlEvento=Url;
         setIndicadores(QFileInfo(Url).baseName(),false);
         return;
       }


 }
////////////////////////////////////////////////////////////////////////////////
 void Player::DescartarEvento(){
     IsEventoEspera=false;


 }



////////////////////// esto es por los eventos quita el item rojo cuando se reproduce//////////////////
void Player::QuitarRojo(){
    if(!w_Lista->rowCount()){return;} //si esta vacia nos vamos

      ListaEstado *w_ListaEstado = new ListaEstado(this->w_Lista);
      w_ListaEstado->SetItem(w_ListaEstado->Rojo());
      w_ListaEstado->Borrar();                            //lo borramos
      delete w_ListaEstado;



}
