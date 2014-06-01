/**
 * Main Window
 * -----------------------------------------
 *
 * This main Window of Radit.
 *
 * @author Victor Algaba
 **/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ToolBar.h"
#include "Cunas.h"
#include "General.h"
#include "AcercaDe.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setDockOptions(QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);

    //Dock *DocTool = new Dock(this);
    //DocTool->setFeatures(QDockWidget::NoDockWidgetFeatures);
    //DocTool->setAllowedAreas(Qt::NoDockWidgetArea);
    //DocTool->setWindowFlags(Qt::Sheet);

    //BASS_Init(3, 44100, 0, 0, NULL);


    //Dock *DockPlayer = new Dock(this);
    DockPlayer = new Dock(this);

    Dock *DockEvento = new Dock(this);

    DockIndicadores = new Dock(this);
    DockAux1 = new Dock(this);
    DockAux2 = new Dock(this);

    ToolBar * w_ToolBar = new ToolBar(this);
    //Cunas * w_Cunas = new Cunas(this);

    DockPlayer->setWindowTitle("  Reproductor Principal");
    DockEvento->setWindowTitle("  Eventos y Hth");
    DockIndicadores->setWindowTitle("  Indicadores y Log emisión");
    DockAux1->setWindowTitle("  Auxiliar 1");
    DockAux2->setWindowTitle("  Auxiliar 2");

    w_Player = new Player("Principal",this);
    w_Player->setWindowTitle("  Reproductor Principal"); // esto no aparece pero sirve para identificar
    w_Player->Dock = DockPlayer;
    w_Player->MainWindow=this;


    Player *PlayerAux1 = new Player("Aux1",this);
    PlayerAux1->setWindowTitle("  Auxiliar 1");  // esto no aparece pero sirve para identificar
    PlayerAux1->Dock = DockAux1;
    PlayerAux1->MainWindow=this;

    Player *PlayerAux2 = new Player("Aux2",this);
    PlayerAux2->setWindowTitle("  Auxiliar 2");  // esto no aparece pero sirve para identificar
    PlayerAux2->Dock = DockAux2;
    PlayerAux2->MainWindow=this;

    //BASS_SetDevice(1); // primer device se usa para los tiempos etc

    Eventos *w_Eventos =new Eventos(this);
    Cunas * w_Cunas = new Cunas(this);

    ////////// hth
    //connect(w_Eventos, SIGNAL(startHth()), w_Player, SLOT(ClickFaderOut())); // CONECTA EVENTO CON PRINCIPAL!!!!!!
    //connect(w_Eventos, SIGNAL(finalHth()), w_Player, SLOT(ClickFaderIn())); // CONECTA EVENTO CON PRINCIPAL!!!!!!

    DockPlayer->setWidget(w_Player);
    DockEvento->setWidget(w_Eventos);
    DockAux1->setWidget(PlayerAux1);
    DockAux2->setWidget(PlayerAux2);


    Indicadores *w_Indicadores =new Indicadores(this);
    DockIndicadores->setWidget(w_Indicadores);

    w_Player->w_Indicadores=w_Indicadores;

    Indicadores *w_Null =new Indicadores(this);
    PlayerAux1->w_Indicadores=w_Null;
    PlayerAux2->w_Indicadores=w_Null;



    ui->toolBar->addWidget(w_ToolBar);
    ui->toolBarCunas->addWidget(w_Cunas);

    this->addDockWidget(Qt::LeftDockWidgetArea, DockPlayer,Qt::Horizontal);
    this->addDockWidget(Qt::LeftDockWidgetArea, DockAux1,Qt::Horizontal);
    this->addDockWidget(Qt::LeftDockWidgetArea, DockAux2,Qt::Horizontal);

    this->addDockWidget(Qt::RightDockWidgetArea, DockEvento,Qt::Horizontal);
    this->addDockWidget(Qt::RightDockWidgetArea, DockIndicadores,Qt::Horizontal);


    //HTH
    connect(w_Eventos, SIGNAL(startHth()), w_Player, SLOT(ClickFaderOut())); // CONECTA EVENTO CON PRINCIPAL!!!!!!
    connect(w_Eventos, SIGNAL(finalHth()), w_Player, SLOT(ClickFaderIn())); // CONECTA EVENTO CON PRINCIPAL!!!!!!




    //EVENTS
    connect(w_Eventos, SIGNAL(startEvento(QString,int,QString)), w_Player, SLOT(Evento(QString,int,QString))); // CONECTA EVENTO CON PRINCIPAL!!!!
    connect(w_Eventos, SIGNAL(descartarEvento()), w_Player, SLOT(DescartarEvento())); //Descarta el evento
    connect(w_Player, SIGNAL(borrarEvento()), w_Eventos, SLOT(BorrarEvento())); // borrar el evento una vez reproducido


    //botones auxiliares log******************************
    connect(w_ToolBar->BtnAux1, SIGNAL(clicked(bool)), this, SLOT(ShowAux1(bool))); // Axiliar 1
    connect(w_ToolBar->BtnAux2, SIGNAL(clicked(bool)), this, SLOT(ShowAux2(bool))); // Auxiliar2
    connect(w_ToolBar->BtnCunas, SIGNAL(clicked(bool)), this, SLOT(ShowCunas(bool))); // Paleta de cuñas
    connect(w_ToolBar->BtnLogEmision, SIGNAL(clicked(bool)), this, SLOT(ShowLog(bool))); // Log emision


    //TOOL BAR
    connect(w_ToolBar->BtnAcerca, SIGNAL(clicked()), this, SLOT(ShowAcercaDe()));  // show About
   // connect(w_ToolBar->BtnGeneral, SIGNAL(clicked()), this, SLOT(ShowGeneral()));  // Show General Seting
    connect(w_ToolBar->BtnSalir, SIGNAL(clicked()), this, SLOT(close()));     // Closed Radit


    //bass configuration
    BASS_SetConfig(BASS_CONFIG_BUFFER, 5000 );
    // BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 10);
    BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST,1); // enable playlist processing

    // plugin

    BASS_PluginFree(0);
    QString Path=QCoreApplication::applicationDirPath().toLatin1();




    #ifdef Q_OS_UNIX
        BASS_PluginLoad(Path.toLatin1() + "/Plugin/libbass_aac.so",0);
        BASS_PluginLoad(Path.toLatin1() + "/Plugin/libbassflac.so",0);
    #endif

    DockAux1->hide();//hide of default
    DockAux2->hide();//hide of default
    DockIndicadores->hide();
    showMaximized();



   //  QSettings RaditIni("Radit.ini", QSettings::IniFormat);
   //  this->restoreState(RaditIni.value("General/state").toByteArray());
   //  this->restoreDockWidget(DockPlayer);

}

/**
 * Free in memory
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
  //  QSettings RaditIni("Radit.ini", QSettings::IniFormat);
  //  RaditIni.setValue("General/state", this->saveState());

    BASS_Free(); //Frees all resources used by the output device, including all its samples, streams and MOD musics.


   delete ui;
}

/**
 * This function update the time
 * @brief MainWindow::UpdateHora
 * @return void
 */
void MainWindow::UpdateHora()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString("dddd  dd MMMM yyyy");
    dateTimeString = dateTimeString + "  " + dateTime.toString("hh:mm:ss");

    L_Hora->setText(dateTimeString);
}

void MainWindow::resizeEvent( QResizeEvent *event){
    //workspace->tile();
}

/**
 * This function checks if aux one is open
 * @brief MainWindow::ShowAux1
 * @param estado
 */
void MainWindow::ShowAux1(bool estado)
{
    if(!estado)
    {
        DockAux1->hide();
        return;
    }
    else
    {
        DockAux1->show();
    }
}

/**
 * This function checks if aux one is open
 * @brief MainWindow::ShowAux2
 * @param estado
 */
void MainWindow::ShowAux2(bool estado)
{
    if(!estado)
    {
        DockAux2->hide();
        return;
    }
    else
    {
        DockAux2->show();
    }
}
/**
 * Checks if jingles is open
 * @brief MainWindow::ShowCunas
 * @param estado
 */
void MainWindow::ShowCunas(bool estado)
{
    if(!estado)
    {
        ui->toolBarCunas->hide();
        return;
    }
    else
    {
        ui->toolBarCunas->show();
    }
}

/**
 * Checks if log is open
 * @brief MainWindow::ShowLog
 * @param estado
 */
void MainWindow::ShowLog(bool estado)
{
    if(!estado)
    {
        DockIndicadores->hide();
        return;
    }
    else
    {
        DockIndicadores->show();
    }
}

void MainWindow::ShowGeneral()
{
    General *w_General= new General();

    w_General->exec();

    delete w_General;
}

/**
 * Open the About of Software
 * @brief MainWindow::ShowAcercaDe
 */
void MainWindow::ShowAcercaDe()
{
    AcercaDe *w_AcercaDe = new AcercaDe();
    w_AcercaDe->exec();

    delete w_AcercaDe;
}






