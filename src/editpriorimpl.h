#ifndef EDITPRIORIMPL_H
#define EDITPRIORIMPL_H
//
#include "ui_editprior.h"
#include "prior.h"
#include <QtGui>
//
class EditPriorImpl : public QDialog, public Ui::EditPrior
{
Q_OBJECT
public:
	EditPriorImpl( QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowFlags() );
	void setPrior(Prior * p);
private slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
protected:
	Prior * prior;
};
#endif





