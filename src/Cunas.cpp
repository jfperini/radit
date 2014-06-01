/**
 * Cunas
 * -----------------------------------------
 *
 * - This class represent jingles
 * - Control with views
 *
 * @author Victor Algaba
 */
#include <QDebug>
#include <QSqlRecord>
#include <QSqlQuery>
#include "Cunas.h"
#include "EditorCunas.h"
#include "engine/Pisadores.h"


Cunas::Cunas(QWidget*parent )
    :QDialog(parent)
{
  setupUi(this);

  connect(BtnEditar, SIGNAL(clicked()), this, SLOT(ShowEditorCunas()));//edit jingles

  //buttons
  connect(Btn1, SIGNAL(clicked()), this, SLOT(clickBoton()));
  connect(Btn2, SIGNAL(clicked()), this, SLOT(clickBoton()));
  connect(Btn3, SIGNAL(clicked()), this, SLOT(clickBoton()));
  connect(Btn4, SIGNAL(clicked()), this, SLOT(clickBoton()));
  connect(Btn5, SIGNAL(clicked()), this, SLOT(clickBoton()));
  connect(Btn6, SIGNAL(clicked()), this, SLOT(clickBoton()));
  connect(Btn7, SIGNAL(clicked()), this, SLOT(clickBoton()));
  connect(Btn8, SIGNAL(clicked()), this, SLOT(clickBoton()));
  connect(Btn9, SIGNAL(clicked()), this, SLOT(clickBoton()));

  CrearBase();
}

/**
 * This function play jingles of button
 * @brief Cunas::clickBoton
 * @return void
 */
void Cunas::clickBoton()
{
    QObject* obj = QObject::sender();
    CunaButton *Boton = qobject_cast<CunaButton *>(obj);
    QString bo = Boton->objectName();
    bo = bo.right(1);
    int Codigo=bo.toInt();

    QString Pagina=ComboPagina->itemText(ComboPagina->currentIndex());
    QSqlQuery query(db);


    query.prepare("SELECT * FROM JIMGLES  WHERE COD= ? AND PAGINA = ?");
    query.addBindValue(Codigo-1);
    query.addBindValue(Pagina);
    query.exec();
    query.first();
    QSqlRecord rec =  query.record();

    if(rec.value("URL").toString().isEmpty())
        return;

    if(!Boton->isChecked())
    {
        delete Boton->SetPisadore;  //make stop
        Boton->SetPisadore=NULL;
        BotonColor(Boton,false); //hide green
        return;
    }




    Pisadores * w_Pisadores = new Pisadores(rec.value("URL").toString());
    Boton->SetPisadore=w_Pisadores;
    w_Pisadores->Configuracion("Principal");
    w_Pisadores->boton=Boton;
    connect(w_Pisadores, SIGNAL(FinishCuna(QPushButton*)), this, SLOT(Final(QPushButton*))); // Editor cuñas
    w_Pisadores->Start();
    BotonColor(Boton,true); //set green
}

/**
 * Checks if is Final
 * @brief Cunas::Final
 * @param boton
 */
void Cunas::Final(QPushButton *boton)
{
    QObject* obj = QObject::sender();
    Pisadores *objeto = qobject_cast<Pisadores *>(obj);
    delete objeto;
    BotonColor(boton,false); //hide green
}

/**
 * This function set color of button
 * @brief Cunas::BotonColor
 * @param boton
 * @param verde
 */
void Cunas::BotonColor(QPushButton *boton, bool verde)
{
    //green
    boton->setStyleSheet(QString::fromUtf8(
                        "border: 1px solid #000000;\n"
                        "border-radius:4px;\n"
                        "background-color:\n"
                        "qlineargradient(spread:pad, x1:0.994318, y1:1, x2:1, y2:0, stop:0\n"
                        "rgba(253, 248, 247, 255), stop:0.988636 rgba(1, 138, 12, 255));\n"
                        ""));
    boton->setChecked(true);

    if(verde)
        return;

    boton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
                        "border: 1px solid #000000;\n"
                        "border-radius:4px;\n"
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                        "stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
                        "min-width: 30px;\n"
                        "font: 75 11pt \"MS Shell Dlg 2\";\n"
                        "\n"
                        "\n"
                        " }\n"
                        "\n"
                        "QPushButton:pressed {\n"
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                        "                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
                        " }\n"
                        "\n"
                        " QPushButton:flat {\n"
                        "     border: none; /* no border for a flat push button */\n"
                        " }\n"
                        "\n"
                        "QPushButton:hover {\n"
                        "     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                        "                                       stop: 0 #f5b14e, stop: 1 #fff9ef);\n"
                        " }\n"
                        "\n"
                        " QPushButton:default {\n"
                        "     border-color: navy; /* make the default button prominent */\n"
                        " }\n"
                        ""));
  boton->setChecked(false);
}


//////////////////////////////////////////////////////////////////////////////////
void Cunas::ShowNombre(){}

/**
 * This function create base jingles of file Jingles.rsd
 * @brief Cunas::CrearBase
 */
void Cunas::CrearBase()
{
    QString Path=QCoreApplication::applicationDirPath().toLatin1();

    db = QSqlDatabase::addDatabase("QSQLITE","conn_cunas");
    db.setDatabaseName(Path + "/Jingles/Jingles.rsd");
    db.open();

    QSqlQuery query(db);

    query.exec("create table PAGINAS (NOMBRE text primary key)");
    query.exec("create table JIMGLES(COD int,"
                                     "PAGINA text,"
                                     "NOMBRE text,"
                                     "URL text,"
                                     "TIPO int)"
                                      );
    query.exec("SELECT * FROM PAGINAS");

    query.first();
    if(!query.isValid())
        Llenar();
}

/**
 * Show editor of  jingles
 * @brief Cunas::ShowEditorCunas
 * @return void
 */
void Cunas::ShowEditorCunas()
{
    EditorCunas *w_EditorCunas = new EditorCunas(db);
    w_EditorCunas->exec();

    delete w_EditorCunas;
}

/**
 * Creates jingles empty
 * @brief Cunas::Llenar
 * @return void
 */
void Cunas::Llenar()
{
    QSqlDatabase::database().transaction();
    QSqlQuery query(db);

    query.prepare("REPLACE INTO PAGINAS"
                  "(NOMBRE) VALUES"
                  "(?)"
                  );
    QString pa;

    for(int row = 1; row < 10; row++)
    {
        query.addBindValue(pa.setNum(row));
        query.exec();
    }

    query.prepare("REPLACE INTO JIMGLES"
                  "(COD, PAGINA, NOMBRE, URL, TIPO) VALUES"
                  "(?,?,?,?,?)"
                  );
   int pagina=1;

   for(pagina=1; pagina<10; pagina++)
   {
        for(int row = 1; row < 10; row++)
        {
            query.addBindValue(row);
            query.addBindValue(pagina);
            query.addBindValue("");
            query.addBindValue("");
            query.addBindValue("");
            query.exec();
        }
    }

   QSqlDatabase::database().commit();
}
