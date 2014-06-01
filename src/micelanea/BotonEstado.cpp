/**
 * Aleatorio
 * -----------------------------------------
 *
 * - Changes the state of the buttons on/off
 * - Create/remove red item that is playing
 * - get item red
 *
 * @author Victor Algaba
 */
#include <QDebug>
#include "BotonEstado.h"

BotonEstado::BotonEstado()
{

}

BotonEstado::BotonEstado(QPushButton *w_Boton)
{

    SetButton(w_Boton);
}

void BotonEstado::SetButton(QPushButton *w_Boton)
{
    Boton=w_Boton;
}

void BotonEstado::SetActivo()
{
    Boton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
                                           "border: 1px solid #000000;\n"
                                           "border-radius:4px;\n"
                                           "\n"
                                           "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                                           "                                       stop: 0 #f68f29, stop: 1 #e5fae2);\n"
                                           "     min-width: 30px;\n"
                                           "font: 75 11pt \"MS Shell Dlg 2\";\n"
                                           " }\n"
                                           "\n"
                                           ""));
}

void BotonEstado::SetInactivo()
{
    Boton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
                                           "border: 1px solid #000000;\n"
                                           "border-radius:4px;\n"
                                           "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                                           "stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
                                           "min-width: 30px;\n"
                                           "font: 75 11pt \"MS Shell Dlg 2\";\n"
                                           " }\n"
                                           "\n"
                                           " QPushButton:pressed {\n"
                                           "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                                           " stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
                                           " }\n"
                                           "\n"
                                           " QPushButton:flat {\n"
                                           " border: none; /* no border for a flat push button */\n"
                                           " }\n"
                                           "\n"
                                           "QPushButton:hover {\n"
                                           " background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                                           "  stop: 0 #f5b14e, stop: 1 #fff9ef);\n"
                                           " }\n"
                                           "\n"
                                           " QPushButton:default {\n"
                                           " border-color: navy; /* make the default button prominent */\n"
                                           " }\n"
                                           "    "));
}
