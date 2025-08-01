#ifndef GELMANRUBINIMPL_H
#define GELMANRUBINIMPL_H
//
#include "ui_gelmanrubin.h"
#include <QtXml>
#include <iostream>
#include <vector>
#include <math.h>
#include "param.h"
#include "tree.h"
#include "changepointsontree.h"
//

using namespace std;

class GelmanRubinImpl : public QDialog, public Ui::GelmanRubin
{
Q_OBJECT
public:
	GelmanRubinImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	void compute(Tree*,QDomDocument *,QStringList*);
protected:
	double test(vector< vector<double> >*data);
private slots:
	void on_exportButton_clicked();
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
};
#endif






