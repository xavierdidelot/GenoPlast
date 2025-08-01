#include "ancestralimpl.h"
//
AncestralImpl::AncestralImpl( QWidget * parent, Qt::WindowFlags f)
    : QMainWindow(parent, f) {
  setupUi(this);
}

void AncestralImpl::compute(QDomElement * root,Param * param) {
  n=param->getTree()->getN();
  double tmp;
  this->param=param;
  QDomNodeList list=root->elementsByTagName("Iteration");
  QString data=list.at(0).toElement().elementsByTagName("ancstate").at(0).firstChild().toText().data();
  f=(data.count("1")+data.count("0"))/(n-1);
  vector<double> v(f*(n-1),0.0);
  for (int i=0;i<list.length();i++) {
      QString data=list.at(i).toElement().elementsByTagName("ancstate").at(0).firstChild().toText().data();
      QTextStream in(&data);
      for (unsigned int j=0;j<v.size();j++) {in >> tmp;v[j]=v[j]+tmp/list.length();}
    }
  states=vector<vector<double> >(n*2-1,vector<double>());
  for (int i=0;i<n;i++) for (int j=0;j<f;j++) states[i].push_back(param->getBinData()->get(i,j));
  for (int i=0;i<n-1;i++) for (int j=0;j<f;j++) states[i+n].push_back(v[i*f+j]);
  
  table->setRowCount(2*n-1);
  table->setColumnCount(f);
  for (int i=0;i<2*n-1;i++) for (int j=0;j<f;j++) table->setItem(i,j,new QTableWidgetItem(QString::number(states[i][j])));
}

void AncestralImpl::on_actionSave_as_triggered() {
  if (actionGraph->isChecked()==false) {
      QString qstr = QFileDialog::getSaveFileName(this, tr("Save output file"),".","CSV files (*.csv);;All files (*)");
      if (qstr.isNull()) return;
      QFile file(qstr);
      if ( !file.open(QIODevice::WriteOnly)) return;
      QTextStream ts( &file );
      for (int i=0;i<table->rowCount();i++)
        for (int j=0;j<table->columnCount();j++) {
            if (table->item(i,j)!=NULL) ts<<table->item(i,j)->text();
            if (j<table->columnCount()-1) ts<<","; else ts<<"\n";
          }
      file.close();
    } else {
      QString qstr = QFileDialog::getSaveFileName(this, tr("Save picture file"),".",tr("Joint Photographic Experts Group (*.jpg *.jpeg);;Windows Bitmap (*.bmp);;Portable Network Graphics (*.png);;Portable Pixmap (*.ppm);;X11 Bitmap (*.xbm *.xpm);;PostScript Format (*.ps);;Abode PDF Format (*.pdf)"));
      if (qstr.isNull()) return;
      if (qstr.endsWith("ps") || qstr.endsWith("pdf")) {
          QPrinter qprint;
          qprint.setOutputFileName(qstr);
          qprint.setPageOrientation(QPageLayout::Landscape);
          drawGraph(&qprint);
          return;
        }
      QImage image(width(),height(),QImage::Format_RGB32);
      image.invertPixels();//Fill image in white
      drawGraph(&image);
      image.save(qstr);
    }
    
}

void AncestralImpl::on_actionQuit_triggered() {
  close();
}

void AncestralImpl::on_actionTable_triggered() {
  actionTable->setChecked(true);
  actionGraph->setChecked(false);
  table->show();
  repaint();
}

void AncestralImpl::on_actionGraph_triggered() {
  actionGraph->setChecked(true);
  actionTable->setChecked(false);
  table->hide();
  repaint();
}

void AncestralImpl::paintEvent(QPaintEvent*) {
  if (actionGraph->isChecked()) drawGraph(this);
}
void AncestralImpl::drawGraph(QPaintDevice * qpd) {
  QPainter painter(qpd);
  //Colorbar legend
  double leg=1.0;
  for (int i=0;i<11;i++) {
      if (abs(leg)<0.1) leg=0.0;
      painter.drawText(width()*0.92,height()*(0.11+0.8*i/10),QString::number(leg));
      if (actionEvents->isChecked()) leg-=0.2; else leg-=0.1;
    }
  //YAxis labels
  int r=table->rowCount();
  for (int i=0;i<r;i++) {
      painter.drawText(0,height()*(0.1+0.8*i/r),width()*0.19,height()*0.8/r,Qt::AlignRight,param->getTree()->getName(i));
    }
  painter.translate(width()/5.0,height()/10.0);
  painter.scale(width()*0.6,height()*0.8);
  int maxX;
  if (actionRepresent_lengths->isChecked()) maxX=param->getBinData()->getTotlen();else maxX=f;
  for (int i=0;i<r;i++){
  	int begX=0;
    for (int j=0;j<f;j++) {
    	double l=table->item(i,j)->text().toDouble();
        int gain=255*l*(l>0);
        int loss=255*(-l)*(-l>0);
        int len=1;
        if (actionRepresent_lengths->isChecked()) len=param->getBinData()->getLen(j);
        painter.fillRect(QRectF(1.0*begX/maxX,1.0*i/r,1.0*len/maxX,1.0/r),QColor(255-loss,255-gain-loss,255-gain));
        begX+=len;
      }
     }
  painter.drawRect(QRectF(0.0,0.0,1.0,1.0));
  //Color bar
  for (int i=-255;i<=255;i++) {
      int gain= i*(i>0);
      int loss=-i*(i<0);
      if (actionEvents->isChecked()) 
      painter.fillRect(QRectF(1.1,1.0-1.0*(255+i)/511,0.05,1.0/511),QColor(255-loss,255-gain-loss,255-gain));
      else
      painter.fillRect(QRectF(1.1,1.0-1.0*(255+i)/511,0.05,1.0/511),QColor(255,255-(i+255)/2,255-(i+255)/2));
    }
  painter.drawRect(QRectF(1.1,0.0,0.05,1.0));
}

void AncestralImpl::on_actionRepresent_lengths_triggered() {repaint();}

void AncestralImpl::on_actionStates_triggered() {
  actionStates->setChecked(true);
  actionEvents->setChecked(false);
  table->setRowCount(2*n-1);
  table->setColumnCount(f);
  for (int i=0;i<2*n-1;i++) for (int j=0;j<f;j++) table->setItem(i,j,new QTableWidgetItem(QString::number(states[i][j])));
  repaint();
}

void AncestralImpl::on_actionEvents_triggered() {
  actionEvents->setChecked(true);
  actionStates->setChecked(false);
  table->setRowCount(2*n-2);
  table->setColumnCount(f);
  for (int i=0;i<2*n-2;i++) {
  	int father=param->getTree()->getNode(i)->getFather()->getId();
  	for (int j=0;j<f;j++) table->setItem(i,j,new QTableWidgetItem(QString::number(states[i][j]-states[father][j])));
 	}
 	repaint();
}
