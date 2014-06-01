#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include "Indicadores.h"

Indicadores::Indicadores(QWidget *parent )
    :QDialog(parent)
{
    setupUi(this);
    //return;

    QDateTime dateTime = QDateTime::currentDateTime();
    QString Fecha = dateTime.toString("dd-MM-yy");

    UpdateLog(Fecha + ".txt");
}


/**
 * This function Add new line in log
 * @brief void
 */


void Indicadores::setAdd(const QString line){


   this->TLog->append(line);


}


void Indicadores::UpdateLog(QString file)
{
    QString Path=QCoreApplication::applicationDirPath().toLatin1();
    QFile text_file(Path + "/Logs/" + file ); // link the text file with the QFile object

    if (text_file.open(QIODevice::ReadOnly)) // try to open the file. Mode: Read only
    {
        QTextStream text_stream(&text_file); // set the interface for reading the text
        TLog->setText(text_stream.readAll());
    }

    text_file.close();  // close the file
}
