/**
 * IoLista
 * -----------------------------------------
 *
 * - Open and save playlist
 *
 * @author Victor Algaba
 */


#ifndef IOLISTA_H
#define IOLISTA_H
#include <QTableWidget>
#include <QIODevice>
#include <QStandardItemModel>
#include <QFile>
#include <QDir>

class IoLista
{
    private:
        QFile file;
        QTableWidget *lista;
        void saveModel(QIODevice *device, QTableWidget *model);
        void Path(const QString Cual);
        QString Dir;

    public:
        IoLista(QTableWidget *cual);
        ~IoLista();
        void Abrir();
        void Guardar();//save as
        void Guardar(const QString fichero);//save
        void loadModel(QIODevice *device, QTableWidget *model);
        QString Nombre;
};

#endif // IOLISTA_H
