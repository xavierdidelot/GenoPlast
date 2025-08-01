/***************************************************************************
 *   Copyright (C) 2008 by Xavier Didelot   *
 *   X.Didelot@warwick.ac.uk   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "changepointsontree.h"

ChangePointsOnTree::ChangePointsOnTree(Param * p) {
  int n=p->getTree()->getN();
  param=p;
  positions=(vector<double>**)calloc(n+n-2,sizeof(vector<double>*));
  values=(vector<double>**)calloc(n+n-2,sizeof(vector<double>*));
  for (int i=0;i<n+n-2;i++) {
      positions[i]=new vector<double>(1,p->getTree()->getDist(i));
      values[i]=new vector<double>(1,1.0);
    }
  topval=1.0;
  totcp=0;
}

ChangePointsOnTree::ChangePointsOnTree(Param * p,QDomElement * dom) {
  int n=p->getTree()->getN();
  param=p;
  topval=dom->elementsByTagName("TopVal").at(0).firstChild().toText().data().toFloat();
  positions=(vector<double>**)calloc(n+n-2,sizeof(vector<double>*));
  values=(vector<double>**)calloc(n+n-2,sizeof(vector<double>*));
  for (int i=0;i<n+n-2;i++) {
      positions[i]=new vector<double>();
      values[i]=new vector<double>();
    }
  totcp=0;
  QDomNodeList cps=dom->elementsByTagName("ChangePoint");
  for (int i=0;i<cps.size();i++) {
      int branch  =cps.at(i).toElement().elementsByTagName("Branch").at(0).firstChild().toText().data().toInt();
      double dist =cps.at(i).toElement().elementsByTagName("Dist")  .at(0).firstChild().toText().data().toFloat();
      double value=cps.at(i).toElement().elementsByTagName("Value") .at(0).firstChild().toText().data().toFloat();
      positions[branch]->push_back(dist);
      values   [branch]->push_back(value);
      totcp++;
    }
  for (int i=0;i<n+n-2;i++) {
      positions[i]->push_back(p->getTree()->getDist(i));
      values[i]->push_back(1.0);
    }
  propagate();
}

void ChangePointsOnTree::propagate() {
  int n=param->getTree()->getN();
  values[param->getTree()->getNode(2*n-2)->getLeft ()->getId()]->back()=topval;
  values[param->getTree()->getNode(2*n-2)->getRight()->getId()]->back()=topval;
  for (int i=2*n-3;i>=n;i--) {
      int left =param->getTree()->getNode(i)->getLeft ()->getId();
      int right=param->getTree()->getNode(i)->getRight()->getId();
      values[left ]->back()=values[i]->front();
      values[right]->back()=values[i]->front();
    }
}

ChangePointsOnTree::~ChangePointsOnTree() {
  int n=param->getTree()->getN();
  for (int i=0;i<n+n-2;i++) {delete positions[i];delete values[i];}
  free(positions);
  free(values);
}

void ChangePointsOnTree::exportXML(ostream * out) {
  *out << "<TopVal>" << topval << "</TopVal>" << endl;
  int n=param->getTree()->getN();
  for (int i=0;i<n+n-2;i++)
    for (unsigned int j=0;j<positions[i]->size()-1;j++) {
        *out << "<ChangePoint>" << endl;
        *out << "<Branch>" << i << "</Branch>" << endl;
        *out << "<Dist>" << positions[i]->at(j) << "</Dist>" << endl;
        *out << "<Value>" << values[i]->at(j) << "</Value>" << endl;
        *out << "</ChangePoint>" << endl;
      }
}

void ChangePointsOnTree::display(QPainter * painter,vector<double>*x,vector<double>*y,int mode,double scale) {
  //QPainter painter(qpd);
  //painter.translate(qpd->width()/10.0,qpd->height()/5.0);
  //painter.scale(qpd->width()*0.7,qpd->height()*0.7);
  int direction=-1;
  if (mode>0) direction=1;
  int n=param->getTree()->getN();
  for (int i=0;i<n+n-2;i++) {
      vector<double> * locs;
      vector<double> * vals;
      this->getCompoBranch(i,&locs,&vals);
      QBrush brush;
      if (direction==1) brush=QBrush(Qt::red); else brush=QBrush(Qt::blue);
      if (direction==1) painter->setPen(Qt::darkRed); else painter->setPen(Qt::darkBlue);
      double beg=x->at(i);
      double nd;
      for (unsigned int j=0;j<locs->size();j++) {
          nd=x->at(i)-locs->at(j);
          if (abs(mode)==1)
            painter->fillRect(QRectF(beg,y->at(i),nd-beg,direction*vals->at(j)/100.0*scale),brush);
          else if (j>0)// && j%2==0)
            painter->drawLine(QLineF(beg,y->at(i)+direction*vals->at(j-1)/100.0*scale,nd,y->at(i)+direction*vals->at(j)/100.0*scale));
          beg=nd;
        }
    }
}



