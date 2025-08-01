#ifndef RUNPARAMIMPL_H
#define RUNPARAMIMPL_H
//
#include "ui_runParam.h"
#include <QtGui>
#include <QFileDialog>
//
class RunParamImpl : public QDialog, public Ui::RunParam
{
Q_OBJECT
public:
	RunParamImpl( QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowFlags() );
private slots:
	void on_BrowseButton_pressed();
	void on_RunButton_clicked();
	void on_CancelButton_clicked();
signals:
	void getRunning(int,int,int,QString,bool);
	void setExportC(bool);
};
#endif





