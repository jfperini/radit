#include "micelanea/ListaEstado.h"
#include <QDebug>

ListaEstado::ListaEstado()
{

}

ListaEstado::ListaEstado(QTableWidget *w_Lista)
{


    Lista=w_Lista;
    Item=Lista->currentRow();
}

void ListaEstado::SetLista(QTableWidget *w_Lista)
{
    Lista=w_Lista;
}

void ListaEstado::SetItem(int w_Item)
{
    Item=w_Item;
}

void ListaEstado::SetRojo()
{
    if(!Lista->rowCount()) //si esta vacia nos vamos
        return;

    // ListaInactivo(Lista);  //quitamos la que hubiese antes

    Lista->item(Item,0)->setBackground(QBrush(QColor(Qt::red)));
    Lista->item(Item,1)->setBackground(QBrush(QColor(Qt::red)));
    Lista->item(Item,2)->setBackground(QBrush(QColor(Qt::red)));

    Lista->item(Item,0)->setForeground(QBrush(QColor(Qt::white)));
    Lista->item(Item,1)->setForeground(QBrush(QColor(Qt::white)));
    Lista->item(Item,2)->setForeground(QBrush(QColor(Qt::white)));
    // ItenActivo=RowActivo(Lista);
}

void ListaEstado::SetVerde()
{
    if(!Lista->rowCount()) //si esta vacia nos vamos
         return;

     Lista->item(Item,0)->setBackground(QBrush(QColor(0,128,0)));
     Lista->item(Item,1)->setBackground(QBrush(QColor(0,128,0)));
     Lista->item(Item,2)->setBackground(QBrush(QColor(0,128,0)));

     Lista->item(Item,0)->setForeground(QBrush(QColor(Qt::white)));
     Lista->item(Item,1)->setForeground(QBrush(QColor(Qt::white)));
     Lista->item(Item,2)->setForeground(QBrush(QColor(Qt::white)));
}

int ListaEstado::Rojo()
{
    QBrush brush(QColor(Qt::red));
    int Rojo=brush.color().red();

    for(int i=0; i != Lista->rowCount(); ++i)
        if(Lista->item(i,0)->background().color().red()==Rojo)
            return(i);

    return(-1);
}

int ListaEstado::Verde()
{
    QBrush brush(QColor(0,128,0));
    int Verde=brush.color().green();

    for(int i=0; i != Lista->rowCount(); ++i)
        if(Lista->item(i,0)->background().color().green()==Verde)
            return(i);

    //return(Lista->rowCount()-1);

    return(-1);
}

void ListaEstado::Borrar()
{
    if(!Lista->rowCount()) //si esta vacia nos vamos
        return;

    if(Item==-1)
        return;

    Lista->item(Item,0)->setBackground(QBrush(QColor(32,39,47)));
    Lista->item(Item,1)->setBackground(QBrush(QColor(32,39,47)));
    Lista->item(Item,2)->setBackground(QBrush(QColor(32,39,47)));

    Lista->item(Item,0)->setForeground(QBrush(QColor(7,250,242)));
    Lista->item(Item,1)->setForeground(QBrush(QColor(7,250,242)));
    Lista->item(Item,2)->setForeground(QBrush(QColor(7,250,242)));
}
