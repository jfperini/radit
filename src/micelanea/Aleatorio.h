/**
 * Aleatorio
 * -----------------------------------------
 *
 * - Implements random systems
 * - avoid repetition of audios files
 *
 * @author Victor Algaba
 */
#ifndef ALEATORIO_H
#define ALEATORIO_H

#include <QDirModel>
#include <QStatusBar>
#include <QVariant>
#include <QLabel>
#include <QFileSystemModel>

class Aleatorio
{
    private:
        QList<QString> results;
        QList<int> Sacado;
        QString Path;
        int TotalFicheros;
        int NumeroAleatorio(int range_starts, int range_ends);   //get random number
        bool Repetido(int numero);//checks whether the file was repeated
        void Todos();// If they are all set to 0 reproducios index
        void Nuevo(int ultimo);//marks the last for us to be the first
        void CrearIndice(const QString url);
        void DeleteIndex();
        double Count(const QString url);

    public:
        Aleatorio(const QString url,QLabel *w_Label);
        ~Aleatorio();
        QString FicheroPath();
        QString Cancion;   //para los titulos
};
#endif // ALEATORIO_H
