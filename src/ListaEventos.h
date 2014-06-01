#ifndef LISTAEVENTOS_H
#define LISTAEVENTOS_H

#include <QTableWidget>

class ListaEventos : public QTableWidget
{
    Q_OBJECT

    private:

    public:
        ListaEventos( QWidget *parent = 0);
        virtual ~ListaEventos(){;}

    protected:
        void resizeEvent( QResizeEvent *event) ;

    private slots:

};
#endif // LISTAEVENTOS_H
