/****************************************************************************
 **
 ** Copyright (C) 2010 Victor Algaba .
 ** All rights reserved.
 ** Contact: (www.radit.org)
 **
 ** *************************************************************************
 **
 **
 **
 ****************************************************************************/














#ifndef ALEATORIOINDICE_H
#define ALEATORIOINDICE_H


#include <QString>



//#include <QList>


class AleatorioIndice{


private:
 QString urlx;



public:
   AleatorioIndice(const QString url);
  ~AleatorioIndice();
  QDataStream Load();




};




























#endif // ALEATORIOINDICE_H
