#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWorkspace>
#include <QMdiArea>
#include <QLabel>

#include "Player.h"
#include "Eventos.h"
#include "Indicadores.h"
#include "micelanea/Dock.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;
        QWorkspace *workspace;

        QLabel *L_Hora;

        Player *w_Player;
        Player *PlayerAux1;
        Player *PlayerAux2;
        Eventos *w_Eventos;

        QTimer *timer_Hora;

        Dock *DockPlayer;
        Dock *DockAux1;
        Dock *DockAux2;
        Dock *DockIndicadores;

    private slots:
        void ShowAux1(bool estado);
        void ShowAux2(bool estado);
        void ShowCunas(bool estado);
        void ShowLog(bool estado);
        void ShowGeneral();
        void ShowAcercaDe();
        void UpdateHora();   //Procesa la hora

    protected:

    void resizeEvent( QResizeEvent *event);
};

#endif // MAINWINDOW_H
