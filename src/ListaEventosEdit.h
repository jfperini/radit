#ifndef LISTAEVENTOSEDIT_H
#define LISTAEVENTOSEDIT_H

#include <QTableWidget>

class ListaEventosEdit : public QTableWidget
{
    Q_OBJECT

    private:

    public:
        ListaEventosEdit( QWidget *parent = 0);
        virtual ~ListaEventosEdit(){;}

    protected:
        void resizeEvent( QResizeEvent *event) ;

    private slots:
};
#endif // LISTAEVENTOSEDIT_H
