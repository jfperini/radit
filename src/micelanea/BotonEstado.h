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
#ifndef BOTONESTADO_H
#define BOTONESTADO_H

#include <QPushButton>
#include <QTableWidget>


class BotonEstado
{
    private:
        QPushButton *Boton;

    protected:

    public:
        BotonEstado();
        BotonEstado(QPushButton *w_Boton);
        virtual ~BotonEstado(){;}
        void SetActivo();
        void SetInactivo();
        void SetButton(QPushButton *w_Boton);
};
#endif // BOTONESTADO_H
