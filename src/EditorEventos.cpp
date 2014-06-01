#include <QInputDialog>
#include <QSqlRecord>
//#include <QSqlError>
#include <QSqlQuery>
//#include <QSqlQueryModel>
#include <QFileInfo>
#include <QDebug>
#include<QMessageBox>
#include "EditorEventos.h"
#include "AddEventos.h"
#include "micelanea/IconoItem.h"

EditorEventos::EditorEventos(QSqlDatabase w_db, QWidget*parent )
    :QDialog(parent)
{
    setupUi(this);

    db=w_db;

    //Botones aceptar/cancelar
    connect(BtnAceptar,SIGNAL(clicked()),this, SLOT(close()));

    //Botones Add/del pestañas
    connect(BtnAddPestana,SIGNAL(clicked()),this, SLOT(AddPestana()));
    connect(BtnDelPestana,SIGNAL(clicked()),this, SLOT(DelPestana()));


    //Botones Add/modificar/borrar eventos
    connect(BtnAddEvento,SIGNAL(clicked()),this, SLOT(AddEvento()));
    connect(BtnEditEvento,SIGNAL(clicked()),this, SLOT(EditEvento()));
    connect(Lista,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(EditEvento()));


    connect(BtnDelEvento,SIGNAL(clicked()),this, SLOT(DelEvento()));

    //borrar expirados
    connect(BtnDelExp,SIGNAL(clicked()),this, SLOT(DelExpirados()));

    connect(Pestanas,SIGNAL(currentChanged(int)),this, SLOT(ClickPestana(int)));

    //connect(Lista,SIGNAL(cellChanged(int,int)),this, SLOT(ChecCambio(int,int)));


    CrearPestanas();//crea las pestañas
    ListarEventos(Pestanas->tabText(Pestanas->currentIndex()));
}


EditorEventos::~EditorEventos()
{
}

void EditorEventos::AddPestana()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Añadir Solapa"),
                                         tr("Nueva Solapa:"), QLineEdit::Normal);
    if (text.isEmpty())
        return;

    QSqlQuery query(db);
    query.prepare("REPLACE INTO PESTANA(NOMBRE)"
                  "VALUES(?)");

    query.addBindValue(text);
    query.exec();

    Pestanas->clear();
    CrearPestanas();


}

void EditorEventos::DelPestana()
{
    QMessageBox msgBox;

    msgBox.setText("Atención al eliminar la solapa, se eliminaran los eventos asociados");
    msgBox.setInformativeText("¿Desea continuar?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();
    if(ret==QMessageBox::No){return;}



    QString Nombre = Pestanas->tabText(Pestanas->currentIndex()).toLatin1();
    Pestanas->removeTab(Pestanas->currentIndex());

    QSqlQuery query(db);

    query.prepare("DELETE FROM PESTANA WHERE NOMBRE = ?");
    query.addBindValue(Nombre);
    query.exec();

    query.prepare("DELETE FROM EVENTOS WHERE PESTANA = ?");   //EVENTOS
    query.addBindValue(Nombre);
    query.exec();

    query.prepare("DELETE FROM HORAS WHERE PESTANA = ?");    //HORAS
    query.addBindValue(Nombre);
    query.exec();
}

void EditorEventos::CrearPestanas()
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM PESTANA");

    query.first();
    while (query.isValid())
    {
        QTabWidget *tab = new QTabWidget;
        QSqlRecord rec =  query.record();
        QString Nombre(rec.value("NOMBRE").toString());

        Pestanas->addTab(tab,tr(Nombre.toLatin1()));
        query.next();
    }
}

void EditorEventos::AddEvento()
{
    if(Pestanas->currentIndex()==-1)//no hay ninguna pestaña debe añadir1
    {
        QMessageBox msgBox;

        msgBox.setText("Debe añadir al menos una solapa");
        msgBox.setIcon(QMessageBox::Question);
        msgBox.exec();
        return;
    }

    AddEventos * w_AddEventos = new AddEventos(db,this);
    w_AddEventos->Pestana=Pestanas->tabText(Pestanas->currentIndex());

    w_AddEventos->exec();

    delete w_AddEventos;

    ListarEventos(Pestanas->tabText(Pestanas->currentIndex()));
}

void EditorEventos::EditEvento()
{
    if(!Lista->rowCount())//if is empty
        return;

    if(Lista->currentRow()==-1)//if dont have marked row
        return;

    AddEventos * w_AddEventos = new AddEventos(db,this);
    w_AddEventos->Pestana=Pestanas->tabText(Pestanas->currentIndex());

    w_AddEventos->setWindowTitle(Lista->item(Lista->currentRow(),2)->text().toLatin1() );

    int cod=Lista->item(Lista->currentRow(),8)->text().toInt();
    w_AddEventos->PoderDatos(cod);

    w_AddEventos->exec();

    delete w_AddEventos;

    ListarEventos(Pestanas->tabText(Pestanas->currentIndex()));
}

void EditorEventos::DelEvento()
{
    if(!Lista->rowCount())//si esta vacia nos vamos
        return;

    if(Lista->currentRow()==-1)//si no hay ninguno marcado
        return;

    int cod=Lista->item(Lista->currentRow(),8)->text().toInt();

    QSqlQuery query(db);
    query.prepare("DELETE FROM EVENTOS WHERE COD = ?");   //BORRAMOS EVENTOS
    query.addBindValue(cod);
    query.exec();


    query.prepare("DELETE FROM HORAS WHERE COD = ?");  //BORRAMOS LAS HORAS
    query.addBindValue(cod);
    query.exec();

    query.prepare("DELETE FROM DIAS WHERE COD = ?");  //BORRAMOS LOS DIAS
    query.addBindValue(cod);
    query.exec();

    ListarEventos(Pestanas->tabText(Pestanas->currentIndex()));
}

void EditorEventos::DelExpirados()
{
    QDateTime fechahora;

    //EVENTOS
    QSqlQuery query(db);
    query.prepare("DELETE FROM EVENTOS WHERE EXPIRACION = ? AND EXPIFECHA < ?");
    query.addBindValue(true);
    query.addBindValue(fechahora.currentDateTime());
    query.exec();

    //HORAS
    query.prepare("DELETE FROM HORAS WHERE EXPIRACION = ? AND EXPIFECHA < ?");
    query.addBindValue(true);
    query.addBindValue(fechahora.currentDateTime());
    query.exec();


    //DIAS
    query.prepare("DELETE FROM DIAS WHERE EXPIRACION = ? AND EXPIFECHA < ?");
    query.addBindValue(true);
    query.addBindValue(fechahora.currentDateTime());
    query.exec();

    ListarEventos(Pestanas->tabText(Pestanas->currentIndex()));
}

void EditorEventos::ListarEventos(QString pestana)
{
    disconnect(Lista,SIGNAL(cellChanged(int,int)),this, SLOT(ChecCambio(int,int)));

    Lista->setRowCount(0);//clean all

    QSqlQuery query(db);
    query.prepare("SELECT * FROM EVENTOS WHERE PESTANA = ?"
                  "ORDER BY INICIOHORA");
    query.addBindValue(pestana);
    query.exec();

    query.first();

    while (query.isValid())
    {

        const int currentRow = this->Lista->rowCount();
        this->Lista->setRowCount(currentRow + 1);

        QSqlRecord rec =  query.record();
        QString Nombre(rec.value("VERHORAINICIO").toString());

        QTableWidgetItem *item = new QTableWidgetItem(1);
        item->data(Qt::CheckStateRole);

        int ch=rec.value("CHEQUEADO").toInt();

        if(ch==0)
            item->setCheckState(Qt::Unchecked);
        else
            item->setCheckState(Qt::Checked);

        item->setText(Nombre);


        //QIcon Icon;
        //Icon.addFile(":/general/iconos/ficheros/tanda.png");
        //item->setIcon(Icon);
        IconoItem *w_IconoItem = new IconoItem(item, rec.value("URL").toString());
        delete w_IconoItem;

        this->Lista->setItem(currentRow,0, item );  //icono/ hora inicio
        this->Lista->setItem(currentRow,1, new QTableWidgetItem(rec.value("VERCOMIENZO").toString()));

        this->Lista->setItem(currentRow,2, new QTableWidgetItem(rec.value("VERFICHERO").toString()));
        this->Lista->setItem(currentRow,3, new QTableWidgetItem(rec.value("VERPRIORIDAD").toString()));

        QString Espera;
        if(rec.value("COMPESPERA").toBool())
            Espera=rec.value("COMMINUTOS").toString();
        else
            Espera= tr("Ninguna");

        this->Lista->setItem(currentRow,4, new QTableWidgetItem(Espera));

        //ver DIAS/HORAS
        this->Lista->setItem(currentRow,5, new QTableWidgetItem(rec.value("VERDIAS").toString()));
        this->Lista->setItem(currentRow,6, new QTableWidgetItem(rec.value("VERHORAS").toString()));



        this->Lista->setItem(currentRow,7, new QTableWidgetItem(rec.value("VEREXPIRACION").toString()));
        this->Lista->setItem(currentRow,8, new QTableWidgetItem(rec.value("COD").toString()));
        query.next();
    }

    connect(Lista,SIGNAL(cellChanged(int,int)),this, SLOT(ChecCambio(int,int)));
}

void EditorEventos::ClickPestana(int cual)
{
    // qDebug()     <<Pestanas->tabText(cual);
    ListarEventos(Pestanas->tabText(cual));
}

void EditorEventos::ChecCambio(int row,int col)  //chequea una pestaña
{
    QTableWidgetItem *item = Lista->item(row,0);

    int cheq = item->checkState();
    int cod = Lista->item(row,8)->text().toInt();

    QSqlQuery query(db);
    query.prepare("UPDATE EVENTOS SET CHEQUEADO = ?"
                  "WHERE COD = ?");

    query.addBindValue(cheq);
    query.addBindValue(cod);
    query.exec();
}
