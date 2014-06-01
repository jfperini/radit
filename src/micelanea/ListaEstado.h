#ifndef LISTAESTADO_H
#define LISTAESTADO_H
#include <QPushButton>
#include <QTableWidget>

class ListaEstado
{
    private:
        QTableWidget *Lista;
        int Item;

    protected:

    public:
        ListaEstado();
        ListaEstado(QTableWidget *w_Lista);
        virtual ~ListaEstado(){;}

        void SetRojo();
        void SetVerde();

        void SetLista(QTableWidget *w_Lista);
        void SetItem(int w_Item);
        void Borrar();

        int Rojo();
        int Verde();
};

#endif // LISTAESTADO_H
