#include <QHeaderView>
#include <QUrl>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QFileDialog>
#include<QMessageBox>

#include "PlayerPisadores.h"
#include "engine/StreamMath.h"
#include "engine/StreamFile.h"
#include "micelanea/ListaEstado.h"
#include "Lista.h"
#include "micelanea/IconoItem.h"
#include "iolista/IoLista.h"
#include "AddRadioInternet.h"

Lista::Lista(QWidget *parent): QTableWidget(parent)    
{   
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->viewport()->setAcceptDrops(true);
    //this->setSelectionMode(QAbstractItemView:: SingleSelection);
    setDragDropMode(QAbstractItemView::InternalMove);

    setDragDropOverwriteMode(false);

    this->setSelectionMode(QAbstractItemView::SingleSelection); //seleccionamos en una linea todas las columnas
    this->horizontalHeader()->setMovable(false);  //evita cambiar las columnas
    this->horizontalHeader()->setResizeMode(QHeaderView::Fixed); //evita redimensionar las columnas
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->setStyleSheet(QString::fromUtf8("QTableView {\n"
                                          "background-color: rgb(46, 54, 103);\n"
                                          "\n"
                                          "}\n"
                                          "\n"
                                          "QHeaderView::section\n"
                                          "{\n"
                                          "spacing: 10px;\n"
                                          "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                                          "                                       stop: 0 #5aa3fd, stop: 1 #1f406a);\n"
                                          "color: white;\n"
                                          "border: 1px solid black;\n"
                                          "margin: 1px;\n"
                                          "text-align: right;\n"
                                          "font-family: arial;\n"
                                          "font-size:12px;\n"
                                          "}\n"
                                          ""));
    TotalTiempo=0;
}

void Lista::resizeEvent( QResizeEvent *event)
{

    //damos forma a las columnas
    int ancho = this->width();

    this->setColumnWidth(0,20);
    this->setColumnWidth(2,100);
    this->setColumnWidth(1,ancho- 140);


    this->setColumnHidden(3, true);  //ocultamos columnas segundos
    this->setColumnHidden(4, true);  //ocultamos columnas fichero

    this->setColumnHidden(5, true);  //ocultamos columnas segundos pisador
    this->setColumnHidden(6, true);  //ocultamos columnas url pisador in
    this->setColumnHidden(7, true);  //ocultamos columnas url pisador out


    /// refrescar la lista es un parche************************************
    /// genera un cuelge si no se pone vscroll en el qtable
    insertRow( 0 );
    removeRow( 0 );
}

/**
 * Drop exent
 * @brief Lista::dropEvent
 * @param event
 */
void Lista::dropEvent(QDropEvent *event)
{            
    PisadorTiempo = "";
    PisadorUrlIn  = "";
    PisadorUrlOut = "";

    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        QStandardItemModel *model= new QStandardItemModel;  //cramos modelo
        model->dropMimeData(event->mimeData(), Qt::CopyAction, 0,0, QModelIndex()) ; //añadimo el elemento al modelo

        QTableWidget *tableWidget = new QTableWidget(1, 8, this);

        if (event->source() != this)// es el otro reproductor
        {
            int pos;
            if(!IsItem(event))// esta fuera es el ultimo añadimos al ultimo
            {
                pos= this->rowCount();
                this->setRowCount(pos + 1);
            }
            else//añadimos en su posicion
            {
                pos= itemAt(event->pos())->row();   //posicion nuevo iten
                this->insertRow(pos);
            }

            InsItem(model->item(0,0)->text(),pos);
            event->setDropAction(Qt::CopyAction);
            delete tableWidget;
            delete model;

            return;
        }
        else//es este reproductor reubica los item
        {
            tableWidget->setItem(0,0,new QTableWidgetItem(this->item(this->currentRow(),0)->text()));
            tableWidget->setItem(0,1,new QTableWidgetItem(this->item(this->currentRow(),1)->text()));
            tableWidget->setItem(0,2,new QTableWidgetItem(this->item(this->currentRow(),2)->text()));
            tableWidget->setItem(0,3,new QTableWidgetItem(this->item(this->currentRow(),3)->text()));
            tableWidget->setItem(0,4,new QTableWidgetItem(this->item(this->currentRow(),4)->text()));
            tableWidget->setItem(0,5,new QTableWidgetItem(this->item(this->currentRow(),5)->text()));
            tableWidget->setItem(0,6,new QTableWidgetItem(this->item(this->currentRow(),6)->text()));
            tableWidget->setItem(0,7,new QTableWidgetItem(this->item(this->currentRow(),7)->text()));

            QIcon icono(this->item(this->currentRow(),0)->icon());
            tableWidget->item(0,0)->setIcon(icono);

            int pos;
            if(!IsItem(event))//esta fuera es el ultimo
            {
                pos= this->rowCount();
                this->setRowCount(pos + 1);
            }
            else
            {
                pos= itemAt(event->pos())->row();   //posicion nuevo iten
                this->insertRow(pos);
            }

            MoveItem(tableWidget,pos);
            event->setDropAction(Qt::MoveAction);

            #ifdef Q_OS_UNIX
                removeRow(this->currentRow());  //version linux ####borramos una vez isertado,evita duplicados
            #endif
        }

        delete tableWidget;
        delete model;

        return;
    }

    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        QList<QUrl> urls = event->mimeData()->urls();

        QString fileName;
        //configuramos la barra
        progressBar->setMinimum(0);
        progressBar->setMaximum(urls.size());
        progressBar->setVisible(true);

        for (int i = 0; i < urls.size();  ++i)
        {
            /*qDebug() << "Original: " << fileName; //c:/
               qDebug() << "As URL:   " << QUrl::fromLocalFile(fileName);
               qDebug() << "Result:   " << QDir::toNativeSeparators(fileName); //c:\ */
            fileName = urls.at(i).toLocalFile();
            //AddItem(fileName);
            const int currentRow = this->rowCount();
            this->setRowCount(currentRow + 1);

            InsItem(fileName,currentRow);
            progressBar->setValue(i);

            //  BarraStado->showMessage(fileName, 2000);
        }
    }

    //this->setFocus();
    // quitamos la barra
    progressBar->setValue(0);
    progressBar->setVisible(false);
    ShowTotalTiempo();

    //ponemos un verde si no hay
    ListaEstado *w_ListaEstado = new ListaEstado(this);

    if(w_ListaEstado->Verde()==-1)
    {
        w_ListaEstado->SetItem(0);
        w_ListaEstado->SetVerde();
    }

    delete w_ListaEstado ;
}

void Lista::dragEnterEvent(QDragEnterEvent *event)
{      
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
        return;
    }

    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        event->acceptProposedAction();
        return;
    }
}

bool Lista::IsItem(QDropEvent *event)
{
    return(itemAt(event->pos()));
}

void Lista::InsItem(QString url , int pos)
{

    BASS_SetDevice(1); // primer device se usa para los tiempos etc

    #ifdef Q_OS_UNIX
        BASS_SetDevice(2); // primer device se usa para los tiempos etc
    #endif

   StreamMath *w_StreamMath = new StreamMath(StreamFile(url).stream);
    QFileInfo NombreCorto(url);

    this->setItem(pos, 0, new QTableWidgetItem(url));
    this->setItem(pos, 1, new QTableWidgetItem(NombreCorto.completeBaseName())); //nombre de la cancion
    this->setItem(pos, 2, new QTableWidgetItem(w_StreamMath->SegundoToFormato()));// duracion con formato
    this->item(pos, 2 )->setTextAlignment(Qt::AlignRight); // justificamos a la derecha  el tiempo

    this->setItem(pos, 3, new QTableWidgetItem(url));// duracion en segundos******* carbiarlo
    this->setItem(pos, 4, new QTableWidgetItem(url));// direccion paht del fichero

    this->setItem(pos, 5, new QTableWidgetItem(PisadorTiempo));// pisador tiempo de in
    this->setItem(pos, 6, new QTableWidgetItem(PisadorUrlIn));//  pisador in url
    this->setItem(pos, 7, new QTableWidgetItem(PisadorUrlOut));//  pisador out url

    IconoItem *w_IconoItem = new IconoItem(item(pos, 0 ),url); //ponemos el icono para decorar
    delete w_IconoItem;

    //TotalTiempo=TotalTiempo+w_StreamMath->Duracion();

    BASS_StreamFree(w_StreamMath->stream); //free stream
    delete w_StreamMath;
}

void Lista::MoveItem(QTableWidget *w_model, int pos)
{
    this->setItem(pos, 0, new QTableWidgetItem(w_model->item(0,0)->text()));
    this->setItem(pos, 1, new QTableWidgetItem(w_model->item(0,1)->text())); //nombre de la cancion
    this->setItem(pos, 2, new QTableWidgetItem(w_model->item(0,2)->text()));// duracion con formato
    this->item(pos, 2 )->setTextAlignment(Qt::AlignRight); // justificamos a la derecha  el tiempo

    this->setItem(pos, 3, new QTableWidgetItem(w_model->item(0,3)->text()));// duracion en segundos******* carbiarlo
    this->setItem(pos, 4, new QTableWidgetItem(w_model->item(0,4)->text()));// direccion paht del fichero

    this->setItem(pos, 5, new QTableWidgetItem(w_model->item(0,5)->text()));// pisador tiempo de in
    this->setItem(pos, 6, new QTableWidgetItem(w_model->item(0,6)->text()));//  pisador in url
    this->setItem(pos, 7, new QTableWidgetItem(w_model->item(0,7)->text()));//  pisador out url


    QIcon icono(w_model->item(0,0)->icon());
    this->item(pos,0)->setIcon(icono);
}

void Lista::ShowTotalTiempo()
{
    StreamMath *w_StreamMath = new StreamMath();

    LTotalTiempo->setText( w_StreamMath->SegundoToFormato(TotalTiempo,"hh:mm:ss"));
    delete w_StreamMath;
}


//menu editar
void Lista::ClickCortar()
{
    if(!rowCount()) //si esta vacia nos vamos
        return;

    if(currentRow()==-1)// si no hay ninguno marcado
        return;

    QClipboard *cb = QApplication::clipboard();
    QMimeData *mimeData = new  QMimeData;
    QList<QTableWidgetItem *> selec(selectedItems());
    QList<QUrl> urls;

    for (int i = 0; i < selec.size();  ++i)
    {
        if(selec.at(i)->column()==1)//evita que se repitan puesto que guarda toda la columna
        {
            urls << QUrl::fromLocalFile(item(selec.at(i)->row(),4)->text());
            removeRow(selec.at(i)->row()); //borramos el item
        }
    }

    // urls << QUrl::fromLocalFile(item(currentRow(),4)->text());
    //   urls << QUrl::fromLocalFile(item(selec.r,4)->text());

    mimeData->setUrls(urls);

    // qDebug() <<  selectedItems();

    cb->setMimeData(mimeData, QClipboard::Clipboard );
    //  removeRow(currentRow()); //borramos el item
    // delete mimeData;
}

void Lista::ClickCopiar()
{
    if(!rowCount()) //si esta vacia nos vamos
        return;

    if(currentRow()==-1)// si no hay ninguno marcado
        return;

    QClipboard *cb = QApplication::clipboard();
    QMimeData *mimeData = new  QMimeData;
    QList<QTableWidgetItem *> selec(selectedItems());
    QList<QUrl> urls;

    for (int i = 0; i < selec.size();  ++i)
    {
        if(selec.at(i)->column()==1)//evita que se repitan puesto que guarda toda la columna
        {
            urls << QUrl::fromLocalFile(item(selec.at(i)->row(),4)->text());
        }
    }

    mimeData->setUrls(urls);
    cb->setMimeData(mimeData, QClipboard::Clipboard );

    // delete mimeData;
}

void Lista::ClickPegar()
{
    const QClipboard *cb = QApplication::clipboard();
    const QMimeData *mimeData = cb->mimeData();

    QList<QUrl> urls = mimeData->urls();
    QString fileName;

    for (int i = 0; i < urls.size();  ++i)
    {
        fileName = urls.at(i).toLocalFile();

        const int currentRow = this->rowCount();
        this->setRowCount(currentRow + 1);
        InsItem(fileName,currentRow);
        progressBar->setValue(i);
    }
}

///////////////////////////////// añadir pista ///////////////////////////
void Lista::AddPista()
{
    QSettings RaditIni("Radit.ini", QSettings::IniFormat);
    QString Dir=QDir::toNativeSeparators(RaditIni.value("General/Path").toString());

    QStringList urls = QFileDialog::getOpenFileNames(0,
                                                     QObject::tr("Abrir"),
                                                     Dir,
                                                     QObject::tr("Todos (*.wav *.mp2 *.mp3 *.ogg *.flac *.wma *.rot *.seq *.tad *lst);;"
                                                                 "Sound file (*.wav *.mp2 *.mp3 *.ogg *.flac *.wma);;"
                                                                 "Rotacion  (*.rot);;"
                                                                 "Secuencia (*.seq);;"
                                                                 "Tanda  (*.tad)"
                                                                 "Lista  (*.lst)"
                                                                 ));


    QString fileName;

    for (int i = 0; i < urls.size(); ++i)
    {
        fileName = urls.at(i);

        const int currentRow = this->rowCount();
        this->setRowCount(currentRow + 1);
        InsItem(fileName,currentRow);
        progressBar->setValue(i);
    }

    //Actualizamos el Path
    QFileInfo Directorio(fileName);
    if(!Directorio.isFile()){return;}
    RaditIni.setValue("General/Path",Directorio.absolutePath());
}

/* HTH */

void Lista::AddHora()
{
    const int currentRow = this->rowCount();
    this->setRowCount(currentRow + 1);
    this->InsItem("Locucion Hora.hora",currentRow);
}

void Lista:: AddTemperatura()
{
    const int currentRow = this->rowCount();
    this->setRowCount(currentRow + 1);
    this->InsItem("Locucion Temperatura.tempe",currentRow);
}

void Lista:: AddHumedad()
{
    const int currentRow = this->rowCount();
    this->setRowCount(currentRow + 1);
    this->InsItem("Locucion Humedad.hume",currentRow);
}

void Lista::AddRadio()
{
    AddRadioInternet *w_AddRadioInternet = new AddRadioInternet(this);

    w_AddRadioInternet->exec();

    if(w_AddRadioInternet->Url.isEmpty())
    {
        delete w_AddRadioInternet;
        return;
    }

    const int pos = this->rowCount();
    this->setRowCount(pos + 1);

    this->setItem(pos, 0, new QTableWidgetItem(w_AddRadioInternet->Url));
    this->setItem(pos, 1, new QTableWidgetItem(w_AddRadioInternet->Nombre)); //nombre de la cancion
    this->setItem(pos, 2, new QTableWidgetItem(w_AddRadioInternet->Duracion));// duracion con formato
    this->item(pos, 2 )->setTextAlignment(Qt::AlignRight); // justificamos a la derecha  el tiempo

    this->setItem(pos, 3, new QTableWidgetItem(w_AddRadioInternet->Url));// duracion en segundos******* carbiarlo
    this->setItem(pos, 4, new QTableWidgetItem(w_AddRadioInternet->Url));// direccion paht del fichero

    this->setItem(pos, 5, new QTableWidgetItem(PisadorTiempo));// pisador tiempo de in
    this->setItem(pos, 6, new QTableWidgetItem(PisadorUrlIn));//  pisador in url
    this->setItem(pos, 7, new QTableWidgetItem(PisadorUrlOut));//  pisador out url

    QIcon icono;
    icono.addFile(":/ficheros/iconos/ficheros/online.png");
    this->item(pos, 0)->setIcon(icono);

    delete w_AddRadioInternet;
}

void Lista::ClickAddPisador()
{
    if(!rowCount())//if is empty return
        return;

    if(currentRow()==-1)// si no hay ninguno marcado
        return;

    PlayerPisadores *w_PlayerPisadores= new PlayerPisadores();
    w_PlayerPisadores->setLista(this);

    w_PlayerPisadores->exec();

    delete w_PlayerPisadores;


}

////////////////////////////////////////////duraciones
void Lista::DuracionActualizar()
{
}

void Lista::DuracionSeleccion()
{
    if(!rowCount())//if is empty return
        return;

    if(currentRow()==-1)// si no hay ninguno marcado
        return;

    double Total=0;

    QList<QTableWidgetItem *> selectedCells(this->selectedItems());

    StreamMath *w_StreamMath = new StreamMath;

    for (int i = 0; i < selectedCells.size();  ++i)
    {
        if(selectedCells.at(i)->column()==1)//evita que se repitan puesto que guarda toda la columna
        {
            StreamFile * w_StreamFile= new StreamFile(item(selectedCells.at(i)->row(),4)->text());

            w_StreamMath->SetStream(w_StreamFile->stream);
            Total= Total + w_StreamMath->Duracion();
            BASS_StreamFree(w_StreamMath->stream); //free stream
            delete  w_StreamFile;
        }
    }

    QMessageBox msgBox;
    msgBox.setText(tr("La duracción del bloque seleccionado es:"));
    msgBox.setInformativeText(w_StreamMath->SegundoToFormato(Total,"hh:mm:ss"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();

    BASS_StreamFree(w_StreamMath->stream); //free stream
    delete  w_StreamMath;
}

void Lista::DuracionActualizarTodas()
{
}
