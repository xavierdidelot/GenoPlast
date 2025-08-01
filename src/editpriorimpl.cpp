#include "editpriorimpl.h"
//
EditPriorImpl::EditPriorImpl( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
	prior=NULL;
}
//

void EditPriorImpl::setPrior(Prior * p)
{
	 prior=p;
	 valParam->setText(QString::number(p->getrateparam()));
	 numParam->setText(QString::number(p->getFreqCP()));
}

void EditPriorImpl::on_buttonBox_accepted()
{
	prior->rateparam=valParam->text().toDouble();
	prior->freqCP=numParam->text().toDouble();
	close();
}

void EditPriorImpl::on_buttonBox_rejected()
{
	close();
}

