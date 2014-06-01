#include <QHeaderView>
#include "ListaEventos.h"

ListaEventos::ListaEventos(QWidget *parent):
    QTableWidget(parent)
{    
    this->setAcceptDrops(false);
    this->setDragEnabled(false);
    //this->setDropIndicatorShown(true);
    //this->viewport()->setAcceptDrops(true);
    this->setSelectionMode(QAbstractItemView:: SingleSelection);
    // setDragDropMode(QAbstractItemView::InternalMove);

    setDragDropOverwriteMode(false);

    this->setSelectionMode(QAbstractItemView::SingleSelection); //seleccionamos en una linea todas las columnas
    this->horizontalHeader()->setMovable(false);  //evita cambiar las columnas
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
                                          "                                       stop: 0 #5aa3fd, stop: 1 #1f406a);\n"
                                          "color: white;\n"
                                          "border: 1px solid black;\n"
                                          "margin: 1px;\n"
                                          "text-align: right;\n"
                                          "font-family: arial;\n"
                                          "font-size:12px;\n"
                                          "}\n"
                                          ""));
}


void ListaEventos::resizeEvent( QResizeEvent *event)
{
    //set width of column
    int ancho = this->width();

    this->setColumnWidth(0,20);//ico
    this->setColumnWidth(1,80);//hora
    this->setColumnWidth(2,80);//Comienzo
    this->setColumnWidth(3,ancho - 280);//titulo
    this->setColumnWidth(4,80);//duracion

    this->setColumnHidden(5, true);//hide column seconds
    this->setColumnHidden(6, true);//hide column file
    this->setColumnHidden(7, true);//hide column cod
    this->setColumnHidden(8, true);//hide column date/time
}
