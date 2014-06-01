/**
 * EditorCunas
 * -----------------------------------------
 *
 * - Control with views
 * - This class work with jingles.
 *
 * @author Victor Algaba
 */
#include <QSettings>
#include <QFileDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include "EditorCunas.h"

EditorCunas::EditorCunas(QSqlDatabase w_db, QWidget *parent)
    :QDialog(parent)
{
    setupUi(this);


    setModal(true);


    //set size of form
    QSize fixedSize(this->width(),this->height());
    setMinimumSize(fixedSize);
    setMaximumSize(fixedSize);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    Lista->setSelectionMode(QAbstractItemView::SingleSelection); //seleccionamos en una linea todas las columnas
    //this->horizontalHeader()->setMovable(false);  //evita cambiar las columnas
    //Lista->horizontalHeader()->setResizeMode(QHeaderView::Fixed); //evita redimensionar las columnas
    Lista->setEditTriggers(QAbstractItemView::NoEditTriggers);


    this->Lista->setColumnWidth(0,330);
    this->Lista->setColumnWidth(1,100);
    this->Lista->setColumnHidden(2, true);//hide url

    db=w_db;

    connect(BtnModificar,SIGNAL(clicked()),this, SLOT(ClickModificar()));//change
    connect(BtnAceptar,SIGNAL(clicked()),  this, SLOT(ClickAceptar()));//ok
    connect(BtnEliminar,SIGNAL(clicked()),  this, SLOT(ClickEliminar()));//delete
    connect(BtnAplicar,SIGNAL(clicked()),  this, SLOT(ClickGuardar()));//save

    connect(ComboPagina,SIGNAL(currentIndexChanged(int)),  this, SLOT(Listar(int)));//combox

    connect(Lista,SIGNAL(doubleClicked(QModelIndex)),  this, SLOT(ClickModificar()));
    Listar(0);
}


void EditorCunas::ClickModificar()
{
    if(!Lista->rowCount()) //if count is empty return
        return;

    if(Lista->currentRow()==-1)//if dont have row marked
        return;

    QSettings RaditIni("Radit.ini", QSettings::IniFormat);
    QString Dir=QDir::toNativeSeparators(RaditIni.value("General/Path").toString());
    QString url;

    url = QFileDialog::getOpenFileName(0,
                                       QObject::tr("Abrir"),
                                       Dir,
                                       QObject::tr("Sound file (*.wav *.mp2 *.mp3 *.ogg *.flac *.wma)"
                                       ));
    ModificarItem(url);
}

/**
 * This function close the editor of jingles
 * @brief EditorCunas::ClickAceptar
 * @return void
 */
void EditorCunas::ClickAceptar()
{
    // this->setCursor(Qt::BusyCursor);
    // this->BtnAceptar->setCursor(Qt::BusyCursor);
    //ClickGuardar();
    close();
}

/**
 * This function remove current jingle
 * @brief EditorCunas::ClickEliminar
 */
void EditorCunas::ClickEliminar()
{
    QString Pagina=this->ComboPagina->currentText();
    int codigo=Lista->currentRow();

    QSqlQuery query(db);
    query.prepare("UPDATE JIMGLES SET"
                  " NOMBRE = ?, URL = ?, TIPO = ?"
                  " WHERE PAGINA = ? AND COD = ?"
                  );

    query.addBindValue("");
    query.addBindValue("");
    query.addBindValue("");

    query.addBindValue(Pagina);
    query.addBindValue(codigo);
    query.exec();

    Listar(ComboPagina->currentIndex());
}

/**
 * @brief EditorCunas::ModificarItem
 * @param url
 */
void EditorCunas::ModificarItem(QString url)
{
    if(url.isEmpty())
        return;

    QFileInfo NombreCorto(url);
    QIcon Icono;
    QTableWidgetItem *item = Lista->item(Lista->currentRow(),0);
    // Icono=Lista->item(Lista->currentRow(),0)->icon();
    item->setText(NombreCorto.completeBaseName());
    // item->setIcon(Icono);

    Lista->setItem(Lista->currentRow(),1, new QTableWidgetItem("Normal"));
    Lista->setItem(Lista->currentRow(),2, new QTableWidgetItem(url));
    //ClickGuardar();
}

/**
 * Save the jingles
 * @brief EditorCunas::ClickGuardar
 */
void EditorCunas::ClickGuardar()
{
    if(!Lista->rowCount())//if is empty
        return;

    this->setCursor(Qt::BusyCursor);
    this->BtnAceptar->setCursor(Qt::BusyCursor);
    this->BtnAplicar->setCursor(Qt::BusyCursor);

    QString Pagina=this->ComboPagina->currentText();

    QSqlQuery query(db);
    query.prepare("DELETE FROM JIMGLES WHERE PAGINA = ?");
    query.addBindValue(Pagina);
    query.exec();

    int rowCount = Lista->rowCount();

    QSqlDatabase::database().transaction();
    query.prepare("REPLACE INTO JIMGLES"
                  "(COD, PAGINA, NOMBRE, URL, TIPO) VALUES"
                  "(?,?,?,?,?)"
                  );

    for(int row = 0; row < rowCount; row++)
    {
        if(!Lista->item(row,0)->text().isNull())
        {
            QString Nombre=Lista->item(row,0)->text();
            QString Tipo=Lista->item(row,1)->text();
            QString Url=Lista->item(row,2)->text();

            query.addBindValue(row);
            query.addBindValue(Pagina);
            query.addBindValue(Nombre);
            query.addBindValue(Url);
            query.addBindValue(Tipo);

            query.exec();
        }
    }

    QSqlDatabase::database().commit();

    this->setCursor(Qt::ArrowCursor);
    this->BtnAceptar->setCursor(Qt::ArrowCursor);
    this->BtnAplicar->setCursor(Qt::ArrowCursor);
}

/**
 * List the jingles
 * @brief EditorCunas::Listar
 * @param in
 */
void EditorCunas::Listar(int in)
{
    QString Pagina=ComboPagina->itemText(in);
    QSqlQuery query(db);

    query.prepare("SELECT * FROM JIMGLES  WHERE PAGINA = ?");
    query.addBindValue(Pagina);
    query.exec();

    QIcon Icono;
    QTableWidgetItem *item = Lista->item(Lista->currentRow(),0);

    Icono=Lista->item(0,0)->icon();
    //item->setText(NombreCorto.completeBaseName());
    // item->setIcon(Icono);

    int row=0;

    query.first();
    while (query.isValid())
    {
        QSqlRecord rec =  query.record();

        Icono=Lista->item(row,0)->icon();
        Lista->setItem(row,0, new QTableWidgetItem(Icono,rec.value("NOMBRE").toString()));
        Lista->setItem(row,1, new QTableWidgetItem(rec.value("TIPO").toString()));
        Lista->setItem(row,2, new QTableWidgetItem(rec.value("URL").toString()));

        query.next();

        row++;
    }
}
