/**
 * Aleatorio
 * -----------------------------------------
 *
 * - Implements random systems
 * - avoid repetition of audios files
 *
 * @author Victor Algaba
 */
#include <QDebug>
#include <QModelIndex>
#include <QTime>
#include <QFileInfo>
#include "Aleatorio.h"

/**
 * @brief Aleatorio::Aleatorio
 * @param url path of directory
 * @param w_Label
 */
Aleatorio::Aleatorio(const QString url, QLabel *w_Label)
{
    Path=url;
    QFile fexiste( url + "/index.dat" );//comprobamos si hay fichero indice

    if(!fexiste.exists())//si no existe  creamos el indice
    {
        this->CrearIndice(url);
        fexiste.close();
    }

   /* QFile file( url + "/index.dat");//leemos el fichero indice que contiene los paht+file
    file.open( QIODevice::ReadOnly );

    QDataStream stream( &file );
    stream.setVersion( QDataStream::Qt_4_2 );

    stream >> results;//lo cargamos en qlist para hacer la lista

    file.close();*/
    TotalFicheros=this->Count(url);


    if(TotalFicheros==1){// solo hay 1 para pobcast

        DeleteIndex();
        CrearIndice(Path);// creamos nuevo para pobcast
        TotalFicheros=this->Count(url);

    }



}

/**
 * Free in memory
 * @brief Aleatorio::~Aleatorio
 */
Aleatorio::~Aleatorio(){}


double Aleatorio::Count(const QString url){

    QFile file( url + "/index.dat");//leemos el fichero indice que contiene los paht+file
    file.open( QIODevice::ReadOnly );

    QDataStream stream( &file );
    stream.setVersion( QDataStream::Qt_4_2 );

    stream >> results;//lo cargamos en qlist para hacer la lista

    file.close();

    return(results.count());

}

void Aleatorio::DeleteIndex(){

    QFile file; //borramos los ficheros
    file.setFileName(Path + "/index.dat");
    file.remove();
    file.setFileName(Path + "/radit.txt");
    file.remove();
    file.close();

 }


/**
 * This function select ramdon file from path
 * @brief Aleatorio::FicheroPath
 * @return the path of random filename
 */
QString Aleatorio::FicheroPath()
{

    if(TotalFicheros==0){// no hay ficheros en la carpeta
         DeleteIndex();
        return("--- error en carpeta --");
    }


    int Cual;
        Cual=NumeroAleatorio(0,TotalFicheros-1);

    while (!Repetido(Cual))
        Cual=NumeroAleatorio(0,TotalFicheros-1);

    Todos();//checks if all has played
    Cancion=results[Cual];//for titles

    return(results[Cual]);
}

/**
 * This function selects a random number
 * @brief Aleatorio::NumeroAleatorio
 * @param range_starts int
 * @param range_ends int
 * @return int random number
 */
int Aleatorio::NumeroAleatorio(int range_starts, int range_ends)
{
    if (range_starts > range_ends)
    {
        range_starts = range_starts ^ range_ends;
        range_ends = range_ends ^ range_starts;
        range_starts = range_starts ^ range_ends;
    }

    if (range_starts == range_ends)
    {
        return range_starts;
    }

    return qrand() % (range_ends - range_starts + 1) + range_starts;
}

/**
 * Determines whether the random number is repeated to avoid repetition
 * @brief Aleatorio::Repetido
 * @param numero
 * @return true or false
 */
bool Aleatorio::Repetido(int numero)
{
    QFile file(Path + "/radit.txt");

    if (!file.open(QIODevice::WriteOnly| QIODevice::ReadOnly| QIODevice::Text))
        return(true);

    QTextStream out(&file);

    while(!out.atEnd())
    {
        QString line = out.readLine();

        if(numero==line.toInt())
            return(false);//esta repetido
    }

    out << numero;
    out << "\n";
    return(true);
}

/**
 * If they are all set to 0 reproducios index
 * @brief Aleatorio::Todos
 * @return void
 */
void Aleatorio::Todos()
{
    int TotalReproducidos=0;

    QFile file(Path + "/radit.txt");

    file.open(QIODevice::WriteOnly| QIODevice::ReadOnly| QIODevice::Text);


    QTextStream out(&file);
    QString line;

    while (!out.atEnd())
    {
        line = out.readLine();
        ++TotalReproducidos;
    }

    // si el total de ficheros es menos o igual que el indice borra indice
    if(TotalFicheros<=TotalReproducidos)
    {
        file.remove();
        Nuevo(line.toInt());   // marca el ultimo en el indice evita repetir el ultimo
    }
}

/**
 * Brand played the last index prevents the latter is the first
 * @brief Aleatorio::Nuevo
 * @param ultimo
 * @return void
 */
void Aleatorio::Nuevo(int ultimo)
{
    QFile file(Path + "/radit.txt");
    file.open(QIODevice::WriteOnly| QIODevice::ReadOnly| QIODevice::Text);

    QTextStream out(&file);

    out << ultimo;
    out << "\n";
}

void Aleatorio::CrearIndice(const QString url)
{
    QFile fexiste( url + "/index.dat" );

    if(fexiste.exists())
        return;

    fexiste.close();

    QList<QString> list;

    QStringList filters;
    filters << "*.mp3" << "*.mp2" << "*.mp1" << "*.ogg" << "*.wav" << "*.aif" <<"*.wma"  <<"*.flac";
    QDirIterator it(url,filters, QDir::Files|QDir::NoSymLinks, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        QString qstr = it.next();
        list << qstr ;
    }

    QFile file( url + "/index.dat" );

    if(!file.open( QIODevice::WriteOnly ) )
        return;

    QDataStream stream(&file);
    stream.setVersion( QDataStream::Qt_4_2 );

    stream << list;
    file.close();
}
