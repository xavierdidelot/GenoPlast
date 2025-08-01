#include "progressimpl.h"
//
ProgressImpl::ProgressImpl( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
}
//

void ProgressImpl::updateProgress(int val)
{
	progressBar->setValue(val);
    update();
}


void ProgressImpl::on_cancelButton_pressed()
{
	close();
	emit(cancel());
}

