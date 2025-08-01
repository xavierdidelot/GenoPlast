#include "plotimpl.h"
//
PlotImpl::PlotImpl( QWidget * parent, Qt::WindowFlags f)
    : QDialog(parent, f) {
  mode=0;
  setupUi(this);
  values=NULL;
}

PlotImpl::~PlotImpl() {
  delete(values);
}

void PlotImpl::paintEvent(QPaintEvent*) {
  if (mode==0) display_traj(this);
  if (mode==1) display_hist(this);
}

void PlotImpl::display_traj(QPaintDevice*qpd) {
  if (values==NULL) return;
  double minY=values->at(0);
  double maxY=values->at(0);
  for (unsigned int i=1;i<values->size();i++) {
      if (values->at(i)>maxY) maxY=values->at(i);
      if (values->at(i)<minY) minY=values->at(i);
    }
  double minX=0.0;
  double maxX=values->size()-1;
  QPainter painter(qpd);
  painter.drawText(QRectF(width()*0.0,height()*0.1,100,100),QString::number(maxY));
  painter.drawText(QRectF(width()*0.0,height()*0.9,100,100),QString::number(minY));
  painter.drawText(QRectF(width()*0.1,height()*0.95,100,100),QString::number(minX+1));
  painter.drawText(QRectF(width()*0.9,height()*0.95,100,100),QString::number(maxX+1));
  
  QPointF * points=(QPointF*)calloc(values->size(),sizeof(QPointF));
  for (unsigned int i=0;i<values->size();i++) {
      double x,y;
      x=i;
      y=values->at(i);
      x=(x-minX)/(maxX-minX);
      y=(y-minY)/(maxY-minY);
      points[i]=QPointF(x,1.0-y);
    }
  painter.translate(width()/10.0,height()/10.0);
  painter.scale(width()*0.8,height()*0.8);
  painter.drawPolyline(points,values->size());
  painter.drawRect(QRectF(-0.05,-0.05,1.1,1.1));
  free(points);
}

void PlotImpl::display_hist(QPaintDevice*qpd)
{
int minX=round(values->at(0));
int maxX=minX;
for (unsigned int i=1;i<values->size();i++) {
	if (round(values->at(i))<minX) minX=round(values->at(i));
	if (round(values->at(i))>maxX) maxX=round(values->at(i));
}
vector<int> hist=vector<int>(maxX-minX+1,0);
for (unsigned int i=0;i<values->size();i++) hist[round(values->at(i))-minX]++;
int maxY=0;
for (unsigned int i=0;i<hist.size();i++) if (hist[i]>maxY) maxY=hist[i];
QPainter painter(qpd);
painter.drawText(QRectF(width()*0.0,height()*0.1,100,100),QString::number(maxY));
painter.drawText(QRectF(width()*0.0,height()*0.9,100,100),QString::number(0));
painter.drawText(QRectF(width()*0.1,height()*0.95,100,100),QString::number(minX));
painter.drawText(QRectF(width()*0.9,height()*0.95,100,100),QString::number(maxX));
painter.translate(width()/10.0,height()/10.0);
painter.scale(width()*0.8,height()*0.8);
for (unsigned int i=0;i<hist.size();i++)
	painter.drawRect(QRectF(1.0*i/(1.0+maxX-minX),1.0-1.0*hist[i]/maxY,1.0/(1.0+maxX-minX),1.0*hist[i]/maxY));
painter.drawRect(QRectF(-0.05,-0.05,1.1,1.1));
}

void PlotImpl::extractValuesNumCP(QDomDocument * domDoc,bool isrp) {
  QString str;
  QDomNodeList list=domDoc->elementsByTagName("Iteration");
  if (isrp) str="rp";else str="rm";
  values=new vector<double>();
  int maxIteration=list.size();
  double mean=0.0;
  for (int i=0;i<maxIteration;i++) {
      values->push_back(list.at(i).toElement().elementsByTagName(str).at(0).toElement().elementsByTagName("ChangePoint").size());
      mean+=values->at(i);
    }
  mean/=values->size();
  vector<double> vals=vector<double>(*values);
  sort(vals.begin(),vals.end());
  double low= vals[(int)floor(0.025*vals.size())];
  double high=vals[(int)floor(0.975*vals.size())];
  setWindowTitle(QString::number(mean)+" ["+QString::number(low)+","+QString::number(high)+"]");
}

void PlotImpl::extractValuesTop(QDomDocument * domDoc,bool isrp) {
  QString str;
  QDomNodeList list=domDoc->elementsByTagName("Iteration");
  if (isrp) str="rp";else str="rm";
  values=new vector<double>();
  int maxIteration=list.size();
  for (int i=0;i<maxIteration;i++)
    values->push_back(list.at(i).toElement().elementsByTagName(str).at(0).toElement().elementsByTagName("TopVal").at(0).firstChild().toText().data().toFloat());
}

void PlotImpl::extractValuesLlhood(QDomDocument * domDoc) {
  QDomNodeList list=domDoc->elementsByTagName("Iteration");
  values=new vector<double>();
  int maxIteration=list.size();
  for (int i=0;i<maxIteration;i++)
    values->push_back(list.at(i).toElement().elementsByTagName("llhood").at(0).firstChild().toText().data().toFloat());
}

void PlotImpl::on_exportButton_clicked() {
  QString qstr = QFileDialog::getSaveFileName(this, tr("Save picture file"),".",tr("Joint Photographic Experts Group (*.jpg *.jpeg);;Windows Bitmap (*.bmp);;Portable Network Graphics (*.png);;Portable Pixmap (*.ppm);;X11 Bitmap (*.xbm *.xpm)"));
  if (qstr.isNull()) return;
  QImage image(width(),height(),QImage::Format_RGB32);
  image.invertPixels();//Fill image in white
  if (mode==0) display_traj(&image);
  if (mode==1) display_hist(&image);
  image.save(qstr);
}

