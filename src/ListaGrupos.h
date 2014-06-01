#ifndef LISTAGRUPOS_H
#define LISTAGRUPOS_H

#include <QTableWidget>

class ListaGrupos : public QTableWidget
{
    Q_OBJECT

    private:

    public:
        ListaGrupos( QWidget *parent = 0);
        virtual ~ListaGrupos(){;}

    protected:
        void resizeEvent( QResizeEvent *event) ;

    private slots:
};
#endif // LISTAGRUPOS_H
