#ifndef LISTA_H
#define LISTA_H

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QTableWidget>

#include <QApplication>
#include <QClipboard>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QProgressBar>
#include <QLabel>

class Lista : public QTableWidget
{
    Q_OBJECT

    private:
        void InsItem(QString url, int pos);
        void MoveItem(QTableWidget *w_model, int pos);
        void ShowTotalTiempo();

        unsigned long TotalTiempo;

        //para los item de los pisadores

        QString PisadorTiempo;
        QString PisadorUrlIn;
        QString PisadorUrlOut;

    public:
        Lista( QWidget *parent = 0);
        virtual ~Lista(){;}
        QStatusBar *BarraStado;
        QProgressBar *progressBar;
        QLabel *LTotalTiempo;

    protected:
        void resizeEvent( QResizeEvent *event) ;
        // void mousePressEvent(QMouseEvent *event);
        //  void mouseMoveEvent(QMouseEvent *event);

        void dropEvent(QDropEvent *event);
        bool IsItem(QDropEvent *event);
        void dragEnterEvent(QDragEnterEvent *e);
        // void dragMoveEvent(QDragMoveEvent *event);

    private slots:
        //menu editar
        void ClickCortar();
        void ClickCopiar();
        void ClickPegar();
        void ClickAddPisador();


        //void ClickNuevo();

        void AddHora();
        void AddTemperatura();
        void AddHumedad();
        void AddPista();
        void AddRadio();
        void DuracionActualizar();
        void DuracionSeleccion();
        void DuracionActualizarTodas();
};
#endif // LISTA_H
