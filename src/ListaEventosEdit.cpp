#include <QHeaderView>
#include "ListaEventosEdit.h"

ListaEventosEdit::ListaEventosEdit(QWidget *parent)
    : QTableWidget(parent)
{
    this->setSelectionMode(QAbstractItemView::SingleSelection); //seleccionamos en una linea todas las columnas
    // this->horizontalHeader()->setMovable(false);  //evita cambiar las columnas
    // this->horizontalHeader()->setResizeMode(QHeaderView::Fixed); //evita redimensionar las columnas
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

void ListaEventosEdit::resizeEvent( QResizeEvent *event)
{
    //damos forma a las columnas
    int ancho = this->width();

    this->setColumnWidth(0,150);  //Hora
    this->setColumnWidth(1,80);   //Comienzo
    this->setColumnWidth(2,ancho - 400);  //Fichero
    this->setColumnWidth(3,80);  //Prioridad
    this->setColumnWidth(4,80);  //Espera
    this->setColumnWidth(5,150);  //Dias de la semana
    this->setColumnWidth(6,80);  //horas
    this->setColumnWidth(7,150);  //Expiracion

    this->setColumnHidden(8, true);  //ocultamos columnas codigo

    /// refrescar la lista es un parche************************************
    /// genera un cuelge si no se pone vscroll en el qtable
    insertRow(0);
    removeRow(0);
}
