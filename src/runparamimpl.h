#ifndef RUNPARAMIMPL_H
#define RUNPARAMIMPL_H
//
#include "ui_runParam.h"
#include <QtGui>
//
class RunParamImpl : public QDialog, public Ui::RunParam
{
Q_OBJECT
public:
	RunParamImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
private slots:
	void on_BrowseButton_pressed();
	void on_RunButton_clicked();
	void on_CancelButton_clicked();
signals:
	void getRunning(int,int,int,QString,bool);
	void setExportC(bool);
};
#endif





