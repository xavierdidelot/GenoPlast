#include "selectmovesimpl.h"
//
SelectMovesImpl::SelectMovesImpl( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
	mall=NULL;
	QObject::connect(listWidget,SIGNAL(itemDoubleClicked (QListWidgetItem*)),this,SLOT(deleteCurrentItem()));
}
//

void SelectMovesImpl::deleteCurrentItem()
{
listWidget->takeItem(listWidget->currentRow());
}

void SelectMovesImpl::on_upButton_clicked()
{
	int cur=listWidget->currentRow();
	int max=listWidget->count();
	if (cur==0 || max==0) return;
	listWidget->insertItem(cur-1,listWidget->currentItem()->text());
	listWidget->takeItem(cur+1);
	listWidget->setCurrentRow(cur-1);
}

void SelectMovesImpl::on_downButton_clicked()
{
	int cur=listWidget->currentRow();
	int max=listWidget->count();
	if (cur==max-1 || max==0) return;
	listWidget->insertItem(cur+2,listWidget->currentItem()->text());
	listWidget->takeItem(cur);
	listWidget->setCurrentRow(cur+1);
}

void SelectMovesImpl::setMall(vector<Move*> * mall)
{
	this->mall=mall;
	while (listWidget->count()>0) listWidget->takeItem(0);
	for (unsigned int i=0;i<mall->size();i++) 
		{QString s=QString::fromStdString(mall->at(i)->desc());listWidget->insertItem(i,s);}
}
vector<Move*> * SelectMovesImpl::initMoves(Param*p)
{
  vector<Move*> * m=new vector<Move*>;
  m->push_back(new MoveValueCP(p,true));
  m->push_back(new MoveValueCP(p,false));
  m->push_back(new MovePosCP(p,true));
  m->push_back(new MovePosCP(p,false));
  m->push_back(new MoveAddCP(p,true));
  m->push_back(new MoveAddCP(p,false));
  m->push_back(new MoveRemoveCP(p,true));
  m->push_back(new MoveRemoveCP(p,false));
  //m->push_back(new MoveValueBoth(p));
  return m;
}

void SelectMovesImpl::on_x2Button_clicked()
{
	int cur=listWidget->currentRow();
	int max=listWidget->count();
	if (cur==-1 || max==0) return;
	listWidget->insertItem(cur,listWidget->currentItem()->text());
}

void SelectMovesImpl::on_resetButton_clicked()
{
	setMall(initMoves(NULL));
}

void SelectMovesImpl::on_cancelButton_clicked()
{
	close();
}

void SelectMovesImpl::on_okButton_clicked()
{
	vector<Move*> *m=new vector<Move*>;
	vector<Move*> *all=initMoves(NULL);
	for (int i=0;i<listWidget->count();i++)
	{
		int j=0;
		while (listWidget->item(i)->text().compare(QString::fromStdString(all->at(j)->desc()))!=0) j++;
		m->push_back(all->at(j)->clone());
	}
	emit done(m);
	for (unsigned int i=0;i<all->size();i++) delete(all->at(i));delete(all);
	close();
}
