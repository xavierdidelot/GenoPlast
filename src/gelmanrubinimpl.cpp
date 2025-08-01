#include "gelmanrubinimpl.h"
//
GelmanRubinImpl::GelmanRubinImpl( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
}
//

void GelmanRubinImpl::compute(Tree*tree,QDomDocument *first,QStringList*others)
{
	//Build a m*n matrix of Param
	vector< vector<Param*> > v;
	int m=1+others->size();
    QDomNodeList list=first->documentElement().elementsByTagName("Iteration");
	int n=list.size();
	int iso=tree->getN();
	v.push_back(vector<Param*>());
	for (int j=0;j<n;j++) {
		QDomElement qde=list.at(j).toElement();
		v[0].push_back(new Param(tree,NULL,&qde));
	}
	for (int i=0;i<others->size();i++) {
    QFile file(others->at(i));
    QDomDocument domDoc=QDomDocument();
	file.open(QIODevice::ReadOnly);
	domDoc.setContent(&file);
	file.close();
	v.push_back(vector<Param*>());
	list=domDoc.documentElement().elementsByTagName("Iteration");
	for (int j=0;j<n;j++) {
		QDomElement qde=list.at(j).toElement();
		v[i+1].push_back(new Param(tree,NULL,&qde));
	}
	}
	
	//Table headers
	table->setRowCount(1+2+(iso+iso-2)*2);
	table->setColumnWidth(0,table->columnWidth(0)*2);
	table->setItem(0,1,new QTableWidgetItem(QString("r-")));
	table->setItem(0,2,new QTableWidgetItem(QString("r+")));
	table->setItem(1,0,new QTableWidgetItem(QString("NumCP")));
	table->setItem(2,0,new QTableWidgetItem(QString("TopVal")));
	for (int i=0;i<iso+iso-2;i++) {
		table->setItem(3+i,0,new QTableWidgetItem(QString("Val at top of branch ")+QString::number(i)));
		table->setItem(3+i+iso+iso-2,0,new QTableWidgetItem(QString("Val at mid of branch ")+QString::number(i)));	
	}

	//Table
	for (int whichR=0;whichR<=1;whichR++) 
	for (int whichP=0;whichP<2+(iso+iso-2)*2;whichP++) {
	vector< vector<double> > data;
	for (int i=0;i<m;i++) {
		data.push_back(vector<double>());
	    for (int j=0;j<n;j++) {
			if (whichP==0) {data[i].push_back(v[i][j]->getR(whichR)->getTotcp());continue;}
	    	if (whichP==1) {data[i].push_back(v[i][j]->getR(whichR)->getTopval());continue;}
	    	vector<double> * locs;
			vector<double> * vals;
	    	if (whichP<2+iso+iso-2) {v[i][j]->getR(whichR)->getCompoBranch(whichP-2,&locs,&vals);data[i].push_back(vals->at(0));continue;}
	    	v[i][j]->getR(whichR)->getCompoBranch(whichP-2-iso-iso+2,&locs,&vals);
	    	int ind=0;
	    	while (locs->at(ind)<locs->back()/2.0) ind++;
	    	data[i].push_back(vals->at(ind));
    	}
    	}
	double R=test(&data);
	table->setItem(whichP+1,1+whichR,new QTableWidgetItem(QString::number(R)));
}
}

double GelmanRubinImpl::test(vector< vector<double> >*data)
{
	vector<double> means;
	double mean;
	unsigned int m=data->size();
	unsigned int n=data->at(0).size();
	for (unsigned int i=0;i<m;i++) {
		mean=0.0;
	    for (unsigned int j=0;j<n;j++) mean+=data->at(i).at(j);
	    mean/=n;
	    means.push_back(mean);
	}
	mean=0.0;
	for (unsigned int j=0;j<m;j++) mean+=means[j];
	mean/=m;
	double B=0.0;
	for (unsigned int j=0;j<m;j++) B+=pow((means[j]-mean),2.0);
	B=B*n/(m-1.0);
	double W=0.0;
	for (unsigned int j=0;j<m;j++) for (unsigned int t=0;t<n;t++) W+=pow((data->at(j).at(t)-means[j]),2.0);
	W=W/m/(n-1.0);
	double V=(n-1.0)/n*W+B/n;
	return V/W;
}

void GelmanRubinImpl::on_buttonBox_accepted()
{
	close();
}

void GelmanRubinImpl::on_buttonBox_rejected()
{
	close();
}

void GelmanRubinImpl::on_exportButton_clicked()
{
	QString qstr = QFileDialog::getSaveFileName(this, tr("Save output file"),".","CSV files (*.csv);;All files (*)");
	if (qstr==NULL) return;
	QFile file(qstr);
	if( !file.open(QIODevice::WriteOnly)) return;
	QTextStream ts( &file );
	for (int i=0;i<table->rowCount();i++) 
		for (int j=0;j<table->columnCount();j++) {
			if (table->item(i,j)!=NULL) ts<<table->item(i,j)->text();
			if (j<table->columnCount()-1) ts<<","; else ts<<endl;
		}
	file.close();
}

