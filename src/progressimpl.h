#ifndef PROGRESSIMPL_H
#define PROGRESSIMPL_H
//
#include "ui_progress.h"
//
class ProgressImpl : public QDialog, public Ui::Progress
{
Q_OBJECT
public:
	ProgressImpl( QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowFlags() );
private slots:
	void on_cancelButton_pressed();
public slots:
    void updateProgress(int val);
signals:
	void cancel();
};
#endif






