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
#ifndef PARAM_H
#define PARAM_H
#include "bindata.h"
#include "tree.h"
#include "prior.h"
#include <vector>
#include <cmath>
#include <cassert>
#include <QtXml>
#include <QtGui>
#include <QApplication>
#include "exponential.h"

/**
    @brief Set of parameters of the model
 @author Xavier Didelot <X.Didelot@warwick.ac.uk>
*/

class Move;
class MoveAddCP;
class ChangePointsOnTree;
extern bool verbose;
extern double exponential(double);

class Param:public QObject {
      Q_OBJECT
    public:
      Param();
      Param(Tree*tree,BinData*bindata);
      Param(Tree*tree,BinData*bindata,QDomElement * dom);
      virtual ~Param();
      void computeMarglh(bool updateC);///<Recomputes the marginal likelihood, and reassign the value of the ancestral features if updateC=true
      double h(bool u,bool v,int id);///<Computes function h=P(u at bottom|v at top)
      double g(bool u,bool v,double rp,double rm,double d);///<Computes function g=P(u at bottom|v at top)
      void metropolis(vector<Move*> mall,int,int,int,string,bool);///<Performs the Metropolis-Hastings algorithm
      inline Tree * getTree() {return tree;}
      void setTree(Tree * t);
      inline BinData * getBinData() {return bindata;}
      inline void setBinData(BinData*bd) {delete(bindata);bindata=bd;}
      inline ChangePointsOnTree * getRp() {return rp;}
      inline ChangePointsOnTree * getRm() {return rm;}
      inline ChangePointsOnTree * getR(int which) {if (which==0) return rm;else return rp;};
      inline double getMarglh() {return marglh;}
      inline void setMarglh(double m) {marglh=m;}
      void exportXMLbegin(ostream*out);
      void exportXMLend(ostream*out);
      void exportXMLiter(ostream*out);
      virtual void display(QPaintDevice *);
      void setValue(int cprp,int cprm,double topvalrp,double topvalrm);
      inline bool isSet() {return set;}
      inline void unSet() {set=false;}
      void simulateData(int f);
      bool exportC;
      bool stop;
      inline Prior * getPrior() {return prior;}
      inline void incrRateScale() {rateScale+=0.1;};
      inline void decrRateScale() {rateScale-=0.1;if (rateScale<0.1) rateScale=0.1;};
      inline double getRateScale() {return rateScale;}
    public slots:
      void cancelled() {stop=true;}
      void echoProgress(int i) {printf("\b\b\b\b%3d%%",i);fflush(0);}
      void setExportC(bool c) {exportC=c;}
    signals:
      void progress(int val);
    protected:
      Tree * tree;///<Tree on which the parameters apply
      BinData * bindata;///<Data and ancestral states of the ancestral nodes
      ChangePointsOnTree * rp;///<Rate of feature gain
      ChangePointsOnTree * rm;///<Rate of feature loss
      double marglh;///<Marginal likelihood of the parameters
      bool set;
      Prior * prior;
      double rateScale;
  };
  
#endif
