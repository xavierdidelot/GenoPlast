#ifndef ANCESTRALIMPL_H
#define ANCESTRALIMPL_H
//
#include <QtXml>
#include <QMainWindow>
#include <QPrinter>
#include <QFileDialog>
#include "param.h"
#include "ui_ancestral.h"
//
class AncestralImpl : public QMainWindow, public Ui::Ancestral
{
Q_OBJECT
public:
	AncestralImpl( QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowFlags() );
	void compute(QDomElement * root,Param * param);
private slots:
	void on_actionRepresent_lengths_activated();
	void on_actionStates_activated();
	void on_actionEvents_activated();
	void on_actionSave_as_activated();
	void on_actionQuit_activated();
	void on_actionTable_activated();
	void on_actionGraph_activated();
protected:    
        Param * param;
        int n;
        int f;
        void paintEvent(QPaintEvent*);
        void drawGraph(QPaintDevice * qpd);
        vector<vector<double> > states;
};
#endif





