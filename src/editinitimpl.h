#ifndef EDITINITIMPL_H
#define EDITINITIMPL_H
//
#include <QDialog>
#include <QInputDialog>
#include "param.h"
#include "changepointsontree.h"
#include "ui_editinit.h"
//
class EditInitImpl : public QDialog, public Ui::EditInit
{
Q_OBJECT
public:
	EditInitImpl( QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowFlags() );
	void setParam(Param * p);
	inline void setSimu() {simu=true;}
private slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
protected:
	Param * param;
	bool simu;
};
#endif





