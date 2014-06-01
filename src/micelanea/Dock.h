#ifndef DOCK_H
#define DOCK_H

#include <QDockWidget>

class Dock: public QDockWidget
{
     Q_OBJECT
    public:
        explicit Dock(QWidget *parent = 0);
        virtual~Dock(){;}

    private:

    protected:
        void closeEvent(QCloseEvent *e);
};
#endif // DOCK_H
