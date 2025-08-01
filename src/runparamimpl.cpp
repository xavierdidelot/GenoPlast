#include "runparamimpl.h"
//
RunParamImpl::RunParamImpl( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
}
//

void RunParamImpl::on_BrowseButton_pressed()
{
  QString qstr = QFileDialog::getSaveFileName(this, tr("Open File"),".","");
  if (qstr==NULL) return;
  outfileText->setText(qstr);
}

void RunParamImpl::on_RunButton_clicked()
{
	emit(setExportC(recAncStates->isChecked()));
	emit(getRunning(burninText->text().toInt(),additionalText->text().toInt(),thininText->text().toInt(),outfileText->text(),randOrder->isChecked()));
	close();
}

void RunParamImpl::on_CancelButton_clicked()
{
	close();
}

