/**
 * IoLista
 * -----------------------------------------
 *
 * - Open and save playlist
 *
 * @author Victor Algaba
 */



#include <QFileDialog>
#include <QObject>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include "iolista/IoLista.h"
#include "micelanea/IconoItem.h"

IoLista::IoLista(QTableWidget *cual)
{
    lista=cual;
    QSettings RaditIni("Radit.ini", QSettings::IniFormat);

    //player1
    Dir=QDir::toNativeSeparators(RaditIni.value("General/Path").toString());
}

/**
 * Free in memory
 * @brief IoLista::~IoLista
 */
IoLista::~IoLista(){}

/**
 * This function open filename
 * @brief IoLista::Abrir
 * @return void
 */
void IoLista::Abrir()
{
    QString fileName = QFileDialog::getOpenFileName(0,
                                                    QObject::tr("Abrir PlayList"),
                                                    Dir,
                                                    QObject::tr("Todos (*.lst *.rot *.seq *tad);;"
                                                                "Lista (*.lst);;"
                                                                "Rotacion (*.rot);;"
                                                                "Secuencia (*.seq);;"
                                                                "Tanda (*.tad)"
                                                                ));
    if(fileName.isNull())
        return;

    file.setFileName(fileName);

    if(file.open(QIODevice::ReadOnly))
        loadModel(&file, lista);

    file.close();

    Path(fileName);

    Nombre = QFileInfo(fileName).fileName();
}

/**
 * This function save as list
 * @brief IoLista::Guardar
 * @return void
 */
void IoLista::Guardar()
{
    QString fileName = QFileDialog::getSaveFileName(0, QObject::tr("Grardar PlayList"),
                                                    Dir + "/" + Nombre,
                                                    QObject::tr("Lista (*.lst);;"
                                                                "Rotacion (*.rot);;"
                                                                "Secuencia (*.seq);;"
                                                                "Tanda (*.tad)"
                                                    ));
    if (fileName.isNull())
        return;

    file.setFileName(fileName);

    if(file.open(QIODevice::WriteOnly))
        saveModel(&file, lista);
    file.close();
    Path(fileName);
    Nombre = QFileInfo(fileName).fileName();
}

/**
 * This function save list
 * @brief IoLista::Guardar
 * @param fichero name
 * @return vid
 */
void IoLista::Guardar(const QString fichero)
{
    if (fichero.isNull())
        return;

    file.setFileName(Dir + "/" + fichero);

    if(file.open(QIODevice::WriteOnly))
        saveModel(&file, lista);

    file.close();
    Nombre = QFileInfo(fichero).fileName();
}

/**
 * ???
 * @brief IoLista::loadModel
 * @param device
 * @param model
 */
void IoLista::loadModel(QIODevice *device, QTableWidget *model)
{
    QDataStream stream(device);

    int version;
    stream >> version;  //control de versiones para el fichero evita cargar erroneos
    if(version !=11)
    {
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("Fichero Incompatible."));
        msgBox.exec();

        return;
    }

    int rowCount, columnCount;
    stream >> rowCount;
    stream >> columnCount;
    lista->setRowCount(rowCount);
    lista->setColumnCount(columnCount);

    for(int row = 0; row < rowCount; row++)
    {
        for(int column = 0; column < columnCount; column++)
        {
            QString item;
            stream >> item;
            model->setItem(row, column, new QTableWidgetItem(item));
        }

        model->item(row, 2 )->setTextAlignment(Qt::AlignRight); // justificamos a la derecha  el tiempo

        QString Path;

        Path=model->item(row, 4 )->text();
        IconoItem *w_IconoItem = new IconoItem(model->item(row, 0) ,Path.toLatin1());
        delete w_IconoItem;
    }
}

/**
 * ???
 * @brief IoLista::saveModel
 * @param device
 * @param model
 */
void IoLista::saveModel(QIODevice *device, QTableWidget *model)
{
    QDataStream stream(device);
    int rowCount = model->rowCount();
    int columnCount = model->columnCount();

    stream << 11;  //controlamos una version de fichero
    stream << rowCount;
    stream << columnCount;

    for(int row = 0; row < rowCount; row++)
    {
        for(int column = 0; column < columnCount; column++)
            stream << model->item(row, column)->text();
    }
}

/**
 * This function save the path for filedialog
 * @brief IoLista::Path
 * @param Cual
 * @return void
 */
void IoLista::Path(const QString Cual)
{
    QFileInfo Directorio(Cual);
    //qDebug() <<Directorio.isFile();
    if(!Directorio.isFile())
        return;
    QSettings RaditIni("Radit.ini", QSettings::IniFormat);
    RaditIni.setValue("General/Path",Directorio.absolutePath());
}
