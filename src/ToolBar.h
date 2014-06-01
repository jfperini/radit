#ifndef TOOLBAR_H
#define TOOLBAR_H
#include "ui_FrmToolBar.h"
#include <QTableWidget>
#include <QString>

class ToolBar : public QDialog, public Ui::FrmToolBar
{
    Q_OBJECT

    private:

    public:
        explicit ToolBar(QWidget *parent = 0);
        virtual ~ToolBar(){;}

    private slots:
};
#endif // TOOLBAR_H
