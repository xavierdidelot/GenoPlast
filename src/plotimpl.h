#ifndef PLOTIMPL_H
#define PLOTIMPL_H
//
#include "ui_plot.h"
#include <QtGui>
#include <QtXml>
#include <iostream>
#include <cmath>
#include <QFileDialog>
//

using namespace std;

class PlotImpl : public QDialog, public Ui::Plot {
      Q_OBJECT
    public:
      PlotImpl( QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowFlags() );
      ~PlotImpl();
      void extractValuesNumCP(QDomDocument * domDoc,bool isrp);
      void extractValuesTop(QDomDocument * domDoc,bool isrp);
      void extractValuesLlhood(QDomDocument * domDoc);
      void trajectory();
      void hist();
      inline void setMode(int m) {mode=m;}
    private slots:
      void on_exportButton_clicked();
      
    protected:
      void paintEvent(QPaintEvent*);
      void display_traj(QPaintDevice*qpd);
      void display_hist(QPaintDevice*qpd);
      vector<double> * values;
      int mode;
  };
#endif
