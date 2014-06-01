#include <QHeaderView>
#include "ListaGrupos.h"

ListaGrupos::ListaGrupos(QWidget *parent): QTableWidget(parent)
{
    // this->setAcceptDrops(true);
    // this->setDropIndicatorShown(true);
    // this->viewport()->setAcceptDrops(true);
    // this->setSelectionMode(QAbstractItemView:: SingleSelection);
    // setDragDropMode(QAbstractItemView::InternalMove);

    // setDragDropOverwriteMode(false);

    this->setSelectionMode(QAbstractItemView::SingleSelection); //seleccionamos en una linea todas las columnas
    //this->horizontalHeader()->setMovable(false);  //evita cambiar las columnas
    this->horizontalHeader()->setResizeMode(QHeaderView::Fixed); //evita redimensionar las columnas
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->setStyleSheet(QString::fromUtf8("QTableView {\n"
                                          "background-color: rgb(46, 54, 103);\n"
                                          "\n"
                                          "}\n"
                                          "\n"
                                          "QHeaderView::section\n"
                                          "{\n"
                                          "spacing: 10px;\n"
                                          "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                                          " stop: 0 #5aa3fd, stop: 1 #1f406a);\n"
                                          "color: white;\n"
                                          "border: 1px solid black;\n"
                                          "margin: 1px;\n"
                                          "text-align: right;\n"
                                          "font-family: arial;\n"
                                          "font-size:12px;\n"
                                          "}\n"
                                          ""));
}

void ListaGrupos::resizeEvent( QResizeEvent *event)
{
    //damos forma a las columnas
    int ancho = this->width();

    this->setColumnWidth(0,20);
    this->setColumnWidth(2,100);
    this->setColumnWidth(1,ancho- 140);


    this->setColumnHidden(3, true);  //ocultamos columnas segundos
    this->setColumnHidden(4, true);  //ocultamos columnas fichero

    this->setColumnHidden(5, true);  //ocultamos columnas segundos pisador
    this->setColumnHidden(6, true);  //ocultamos columnas url pisador in
    this->setColumnHidden(7, true);  //ocultamos columnas url pisador out

    /// refrescar la lista es un parche************************************
    /// genera un cuelge si no se pone vscroll en el qtable
    insertRow( 0 );
    removeRow( 0 );
}
