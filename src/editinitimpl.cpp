#include "editinitimpl.h"
//
EditInitImpl::EditInitImpl( QWidget * parent, Qt::WindowFlags f)
    : QDialog(parent, f) {
  setupUi(this);
  simu=false;
}
//

void EditInitImpl::on_buttonBox_accepted() {
  int cprp,cprm;
  double topvalrp,topvalrm;
  if (numCPrp->text().size()==0) cprp=-1; else cprp=numCPrp->text().toInt();
  if (numCPrm->text().size()==0) cprm=-1; else cprm=numCPrm->text().toInt();
  if (topValrp->text().size()==0) topvalrp=-1; else topvalrp=topValrp->text().toDouble();
  if (topValrm->text().size()==0) topvalrm=-1; else topvalrm=topValrm->text().toDouble();
  param->setValue(cprp,cprm,topvalrp,topvalrm);
  if (parentWidget()!=NULL) parentWidget()->update();
  if (simu) {
      bool ok;
      int f=QInputDialog::getInt(this,"Enter f","Enter number of regions:",100,0,10000,1,&ok);
      if (!ok) return;
      param->unSet();
      param->simulateData(f);
    }
  close();
}

void EditInitImpl::on_buttonBox_rejected() {
  close();
}

void EditInitImpl::setParam(Param * p) {
  param=p;
  if (!param->isSet()) return;
  numCPrp->setText(QString::number(param->getRp()->getTotcp()));
  numCPrm->setText(QString::number(param->getRm()->getTotcp()));
  topValrp->setText(QString::number(param->getRp()->getTopval()));
  topValrm->setText(QString::number(param->getRm()->getTopval()));
}
