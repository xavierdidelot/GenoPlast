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
#include "param.h"
#include "move.h"
#include "moveaddcp.h"
#include "changepointsontree.h"

Param::Param() {
  tree=NULL;
  bindata=NULL;
  prior=new Prior();
  rm=NULL;
  rp=NULL;
  rateScale=1.0;
  exportC=true;
  set=false;
}

Param::Param(Tree * tree,BinData * bindata) {
  this->tree=tree;
  this->bindata=bindata;
  set=false;
  prior=new Prior();
  rm=new ChangePointsOnTree(this);
  rp=new ChangePointsOnTree(this);
  rateScale=1.0;
  exportC=false;
}

Param::Param(Tree * tree,BinData * bindata,QDomElement * dom) {
  rateScale=1.0;
  exportC=false;
  set=false;
  this->tree=tree;
  this->bindata=bindata;
  prior=new Prior();
  QDomElement e=dom->elementsByTagName("rp").at(0).toElement();
  rp=new ChangePointsOnTree(this,&e);
  e=dom->elementsByTagName("rm").at(0).toElement();
  rm=new ChangePointsOnTree(this,&e);
}

Param::~Param() {
  delete(rm);
  delete(rp);
  delete(prior);
}

void Param::computeMarglh(bool updateC) {
  if (bindata==NULL) {marglh=0.0;return;};
  int n=bindata->getN();
  int f=bindata->getF();
  marglh=0.0;
  vector<double> f0=vector<double>(2*n-1,0.0);
  vector<double> f1=vector<double>(2*n-1,0.0);
  vector<double> lcoef=vector<double>(2*n-1,0.0);
  for (int feat=0;feat<f;feat++) {
//Step1
      for (int i=0;i<n;i++) if (bindata->get(i,feat)) {
            f1[i]=1.0;
            f0[i]=0.0;
            lcoef[i]=0.0;
          }
        else {
            f0[i]=1.0;
            f1[i]=0.0;
            lcoef[i]=0.0;
          }
//Step2
      for (int i=n;i<2*n-1;i++) {
          int y=tree->getNode(i)->getLeft()->getId();
          int z=tree->getNode(i)->getRight()->getId();
          double h00y=h(0,0,y);double h00z=h(0,0,z);
          double h01y=h(0,1,y);double h01z=h(0,1,z);
          f0[i]=(f0[y]*h00y+f1[y]*(1.0-h00y))*(f0[z]*h00z+f1[z]*(1.0-h00z));
          f1[i]=(f0[y]*h01y+f1[y]*(1.0-h01y))*(f0[z]*h01z+f1[z]*(1.0-h01z));
          lcoef[i]=lcoef[y]+lcoef[z];
          if (i==2*n-2) {
              //double pi[]={0.015,0.005,0.007,0.452,0.007,0.012,0.054,0.054,0.01,0.011,0.075,0.539,0.071,0.875,0.515,0.106,0.891,0.069,0.469,0.992,0.479,0.144,0.919,0.473,0.989,0.543,0.497,0.99,0.568,0.992,0.994,0.992};
              //f1[i]*=pi[feat];
              //f0[i]*=1.0-pi[feat];
              
              f1[i]*=bindata->getPi();
              f0[i]*=1.0-bindata->getPi();
            }
          double coef=f0[i]+f1[i];
          lcoef[i]=lcoef[i]+log(coef);
          f0[i]=f0[i]/coef;
          f1[i]=f1[i]/coef;
        }
//Step3
      marglh+=bindata->getLen(feat)*(min(1.0*bindata->getTotlen(),1000.0))/bindata->getTotlen()*(log(f0[2*n-2]+f1[2*n-2])+lcoef[2*n-2]);
      if (updateC) {
//Step4
          double u=rand()/ (RAND_MAX + 1.0);
          bool b=u<f1[2*n-2]/(f0[2*n-2]+f1[2*n-2]);
          bindata->set(2*n-2,feat,b);
//Step5
          for (int i=2*n-3;i>=n;i--) {
              u=rand()/ (RAND_MAX + 1.0);
              bool ctxj=bindata->get(tree->getNode(i)->getFather()->getId(),feat);
              double h0=h(0,ctxj,i);
              b=u<f1[i]*(1.0-h0)/(f0[i]*h0+f1[i]*(1.0-h0));
              bindata->set(i,feat,b);
            }
        }
    }
  //marglh+=-100.0*(rp->integral()+rm->integral());
}

double Param::h(bool u,bool v,int id) {
  vector<double> *locrp;
  vector<double> *valrp;
  vector<double> *locrm;
  vector<double> *valrm;
  rp->getCompoBranch(id,&locrp,&valrp);
  rm->getCompoBranch(id,&locrm,&valrm);
  /*assert(locrp->back()==tree->getNode(id)->getDist());
  assert(locrm->back()==tree->getNode(id)->getDist());
  if (locrp->size()>1) assert(locrp->at(0)<locrp->at(1));
  if (locrm->size()>1) assert(locrm->at(0)<locrm->at(1));
  if (locrp->size()>1) assert(locrp->at(locrp->size()-2)<locrp->at(locrp->size()-1));
  if (locrm->size()>1) assert(locrm->at(locrm->size()-2)<locrm->at(locrm->size()-1));*/
  //if (locrp->size()==1 && locrm->size()==1)
  //  return g(u,v,valrp->at(0),valrm->at(0),tree->getNode(id)->getDist());
  double h0,h1,h0next;
  if (v) {h1=1.0;h0=0.0;} else {h1=0.0;h0=1.0;}
  double rmcur,rpcur;
  unsigned int indrp=locrp->size()-1,indrm=locrm->size()-1;
  double x1;
  double x2=locrp->back();
  for (unsigned int i=0;i<locrp->size()+locrm->size()-1;i++) {
      rmcur=valrm->at(indrm);
      rpcur=valrp->at(indrp);
      x1=x2;
      if (indrp==0 && indrm==0) x2=0.0; else
        if (indrp>0 && (indrm==0 || locrp->at(indrp-1)>locrm->at(indrm-1)))
          x2=locrp->at(--indrp);else x2=locrm->at(--indrm);
      double g00=g(0,0,rpcur,rmcur,x1-x2);
      double g01=g(0,1,rpcur,rmcur,x1-x2);
      h0next=g00*h0+g01*h1;
      h1    =(1.0-g00)*h0+(1.0-g01)*h1;
      h0=h0next;
    }
  if (u) return h1; else return h0;
}

double Param::g(bool u,bool v,double rp,double rm,double d) {
  double res,res2;
  if (u) res=rp/(rm+rp);
  else res=rm/(rm+rp);
  if (v) res2=rm/(rm+rp);
  else res2=rp/(rm+rp);
  res2*=exponential(-d*(rp+rm));
  if (u==v) res+=res2;else res-=res2;
  return res;
}

void Param::metropolis(vector<Move*> mall,int burnin,int additional,int thinin,string outfile,bool randOrder) {
  stop=false;
  if (!set) setValue(0,0,0.1,0.1);//setValue(-1,-1,-1.0,-1.0);
  unSet();
  computeMarglh(false);
  ofstream output;
  output.open(outfile.data());
  exportXMLbegin(&output);
  for (int i=0;i<burnin+additional;i++) {
      if (stop) break;
      emit progress(i*100/(burnin+additional));
      if (verbose) cout<<"Iteration "<<i<<endl;
      for (unsigned int j=0;j<mall.size();j++) {
          int j2=j;
          if (randOrder) j2=(int)(floor(rand()/(RAND_MAX+1.0)*mall.size()));
          mall[j2]->move();
          /*if (verbose) {
              cout<<"WARNING! Computationally expensive assert in action..."<<endl;
              double oldmarglh=marglh;
              computeMarglh(false);
              if (marglh!=oldmarglh) cout<<marglh<<"!="<<oldmarglh<<endl;
              assert(marglh==oldmarglh);
            }*/
          QApplication::processEvents();
        }
      if (verbose) cout << "rptop=" << rp->getTopval() << " rmtop=" << rm->getTopval() << " rpcp=" << rp->getTotcp() << " rmcp=" << rm->getTotcp() << " marglh=" << marglh << endl;
      if (i>=burnin && (i-burnin)%(thinin)==0) exportXMLiter(&output);
    }
  emit progress(100);
  exportXMLend(&output);
  output.close();
  //computeMarglh(true);bindata->output(&cout);
}

void Param::exportXMLbegin(ostream * out) {
  *out << "<?xml version = '1.0' encoding = 'UTF-8'?>" << endl;
  *out << "<outputFile>" << endl;
  *out << "<Tree>" << endl;
  *out << tree->newick() << endl;
  *out << "</Tree>" << endl;
}

void Param::exportXMLiter(ostream * out) {
  *out << "<Iteration>" << endl;
  *out << "<rp>" << endl;
  rp->exportXML(out);
  *out << "</rp>" << endl;
  *out << "<rm>" << endl;
  rm->exportXML(out);
  *out << "</rm>" << endl;
  *out << "<llhood>"<<marglh<<"</llhood>"<<endl;
  if (exportC) {
      computeMarglh(true);
      int n=bindata->getN();
      *out<<"<ancstate>"<<endl;
      for (int i=n;i<2*n-1;i++) {for (int j=0;j<bindata->getF();j++) *out<<bindata->get(i,j)<<" ";*out<<endl;}
      *out<<"</ancstate>"<<endl;
    };
  *out << "</Iteration>" << endl;
}

void Param::exportXMLend(ostream * out) {
  *out << "</outputFile>" << endl;
}

void Param::setTree(Tree * t) {
  delete(tree);
  delete(rp);
  delete(rm);
  rp=NULL;
  rm=NULL;
  tree=t;
}

void Param::display(QPaintDevice * qpd) {
  if (tree==NULL) return;
  vector<double>*x,*y;
  tree->getXY(&x,&y);
  QPainter painter(qpd);
  for (int i=0;i<tree->getN();i++)
    painter.drawText(QPointF(qpd->width()/10.0+x->at(i)*qpd->width()*0.7+3,qpd->height()/5.0+y->at(i)*qpd->height()*0.7+2),tree->getName(i));
  for (int i=tree->getN();i<tree->getN()*2-1;i++)
    painter.drawText(x->at(i)*qpd->width()*0.698,y->at(i)*qpd->height()*0.7+2,qpd->width()/10.0,qpd->height()/5.0,Qt::AlignRight+Qt::AlignBottom,tree->getName(i));
  painter.translate(qpd->width()/10.0,qpd->height()/5.0);
  painter.scale(qpd->width()*0.7,qpd->height()*0.7);
  tree->display(&painter,x,y);
  if (rp!=NULL) rp->display(&painter,x,y, 1,rateScale);
  if (rm!=NULL) rm->display(&painter,x,y,-1,rateScale);
  
  delete(x);
  delete(y);
}

void Param::setValue(int cprp,int cprm,double topvalrp,double topvalrm) {
  delete(rm);
  delete(rp);
  rm=new ChangePointsOnTree(this);
  rp=new ChangePointsOnTree(this);
  if (topvalrm==-1.0) topvalrm=prior->raternd();
  if (topvalrp==-1.0) topvalrp=prior->raternd();
  if (cprp==-1) cprp=prior->drawNumCP();
  if (cprm==-1) cprm=prior->drawNumCP();
  rm->setTopval(topvalrm);
  rp->setTopval(topvalrp);
  rm->propagate();
  rp->propagate();
  MoveAddCP * moveaddcp=new MoveAddCP(this,true);
while (rp->getTotcp()<cprp) {setMarglh(-10e10);moveaddcp->move();}
  delete(moveaddcp);
  moveaddcp=new MoveAddCP(this,false);
  while (rm->getTotcp()<cprm) {setMarglh(-10e10);moveaddcp->move();}
  delete(moveaddcp);
  set=true;
}

void Param::simulateData(int f) {
  delete(bindata);
  bindata=new BinData(getTree()->getN(),f);
  int n=bindata->getN();
  for (int f=0;f<bindata->getF();f++) {
      bindata->set(n+n-2,f,floor(rand()*2.0/(RAND_MAX+1.0)));
      for (int i=n+n-3;i>=0;i--) {
          int father=tree->getNode(i)->getFather()->getId();
          double r=rand()/(RAND_MAX+1.0);
          if (r<h(true,bindata->get(father,f),i))
            bindata->set(i,f,true); else bindata->set(i,f,false);
        }
    }
}
