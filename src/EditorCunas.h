/**
 * EditorCunas
 * -----------------------------------------
 *
 * - Control with views
 * - This class work with jingles.
 *
 * @author Victor Algaba
 */
#ifndef EDITORCUNAS_H
#define EDITORCUNAS_H

#include <QSqlDatabase>
#include "ui_FrmEditCunas.h"


class EditorCunas : public QDialog, public Ui::FrmEditCunas
{
    Q_OBJECT

    private:
        void ModificarItem(QString url);
        QSqlDatabase db;

    public:
        explicit EditorCunas(QSqlDatabase w_db, QWidget *parent = 0);
        virtual ~EditorCunas(){;}

    public slots:

    private slots://Botonera
        void ClickAceptar();
        void ClickEliminar();
        void ClickModificar();
        void ClickGuardar();
        void Listar(int in);

    signals:
};
#endif // EDITORCUNAS_H
