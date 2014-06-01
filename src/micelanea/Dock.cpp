#include<QCloseEvent>
#include"Dock.h"

Dock::Dock(QWidget *parent) :
           QDockWidget(parent)
{
    setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
    setAllowedAreas(Qt::LeftDockWidgetArea |Qt::RightDockWidgetArea);

    setStyleSheet(QString::fromUtf8("/* background-color: rgb(33, 48, 58);*/\n"
    "\n"
    " QDockWidget {\n"
    "	color: rgb(255, 255, 255);\n"
    "     \n"
    " }\n"
    "\n"
    " QDockWidget::title {\n"
    "   \n"
    "    \n"
    "	background-color: rgb(33, 135, 207);\n"
    "	\n"
    "	\n"
    "   \n"
    " }\n"
    "\n"
    ""));
}

void Dock::closeEvent(QCloseEvent *e)
{
    e->ignore();
    //e->accept();
}
