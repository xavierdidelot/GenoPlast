#include "paramcons.h"
//
ParamCons::ParamCons(Tree * tree,BinData * bindata) 
:Param(tree,bindata)
{
rmmin=new ChangePointsOnTree(this);
rpmin=new ChangePointsOnTree(this);
rmmax=new ChangePointsOnTree(this);
rpmax=new ChangePointsOnTree(this);
int n=tree->getN();
storerpval=vector< vector< vector<double> > >(n+n-2,vector< vector<double> >());
storerploc=vector< vector< vector<double> > >(n+n-2,vector< vector<double> >());
storermval=vector< vector< vector<double> > >(n+n-2,vector< vector<double> >());
storermloc=vector< vector< vector<double> > >(n+n-2,vector< vector<double> >());
}

ParamCons::~ParamCons()
{
delete(rpmin);
delete(rmmin);
delete(rpmax);
delete(rmmax);
}

void ParamCons::add(Param * param)
{
int n=tree->getN();
vector<double> * locvec;
vector<double> * valvec;
for (int i=0;i<n+n-2;i++)
{
param->getRp()->getCompoBranch(i,&locvec,&valvec);
storerpval[i].push_back(vector<double>(*valvec));
storerploc[i].push_back(vector<double>(*locvec));
param->getRm()->getCompoBranch(i,&locvec,&valvec);
storermval[i].push_back(vector<double>(*valvec));
storermloc[i].push_back(vector<double>(*locvec));
}
}

void ParamCons::compute()
{
    double precision=0.001;
	vector<double> * locvec,*locvecmin,*locvecmax;
    vector<double> * valvec,*valvecmin,*valvecmax;
    double mean,min,max;
    int n=tree->getN();
    ChangePointsOnTree * r, *rmin, *rmax;
    for (int rep=0;rep<2;rep++) {
    if (rep==0) {r=rp;rmin=rpmin;rmax=rpmax;} else {r=rm;rmin=rmmin;rmax=rmmax;}
    for (int br=0;br<n+n-2;br++) {
    double dist=tree->getDist(br);
    r   ->getCompoBranch(br,&locvec,&valvec);
    rmin->getCompoBranch(br,&locvecmin,&valvecmin);
    rmax->getCompoBranch(br,&locvecmax,&valvecmax);
    locvec->pop_back();valvec->pop_back();
    locvecmin->pop_back();valvecmin->pop_back();
    locvecmax->pop_back();valvecmax->pop_back();
    double d=precision;
    while (d<dist) {
    locvec->push_back(d);locvecmin->push_back(d);locvecmax->push_back(d);
    computeValue(br,d,rep,&mean,&min,&max);
    valvec->push_back(mean);valvecmin->push_back(min);valvecmax->push_back(max);
    d+=precision;
    }
    locvec->push_back(dist);valvec->push_back(0.0);
    locvecmin->push_back(dist);valvecmin->push_back(0.0);
    locvecmax->push_back(dist);valvecmax->push_back(0.0);
    }
    }
}

void ParamCons::computeValue(int br,double d,int whichR,double * mean,double *min,double *max)
{
	vector< vector< vector<double> > > *storeval;
	vector< vector< vector<double> > > *storeloc;
	vector<double> vals;
	if (whichR==0) {storeval=&storerpval;storeloc=&storerploc;} else {storeval=&storermval;storeloc=&storermloc;}
	for (unsigned int i=0;i<storeval->at(br).size();i++) 
		{
		unsigned int j=0;
		while (storeloc->at(br)[i][j]<d) j++;
		vals.push_back(storeval->at(br)[i][j]);
		};
	sort(vals.begin(),vals.end());
	*mean=0.0;
	for (unsigned int i=0;i<vals.size();i++) *mean+=vals[i];
	*mean/=vals.size();
	*min=vals[(int)floor(0.025*vals.size())];
	*max=vals[(int)floor(0.975*vals.size())];
}

void ParamCons::display(QPaintDevice * qpd)
{
  if (tree==NULL) return;
  vector<double>*x,*y;
  tree->getXY(&x,&y);
  QPainter painter(qpd);
  for (int i=0;i<tree->getN();i++)
    painter.drawText(QPointF(qpd->width()/10.0+x->at(i)*qpd->width()*0.7+3,qpd->height()/5.0+y->at(i)*qpd->height()*0.7+2),tree->getName(i));
  for (int i=tree->getN();i<tree->getN()*2-1;i++)
    painter.drawText(x->at(i)*qpd->width()*0.698,y->at(i)*qpd->height()*0.7+2,qpd->width()/10.0,qpd->height()/5.0,Qt::AlignRight,tree->getName(i));
  painter.translate(qpd->width()/10.0,qpd->height()/5.0);
  painter.scale(qpd->width()*0.7,qpd->height()*0.7);
  tree->display(&painter,x,y);
  if (rp!=NULL) rp->display(&painter,x,y, 1,rateScale);
  if (rm!=NULL) rm->display(&painter,x,y,-1,rateScale);
    rpmin->display(&painter,x,y, 2,rateScale);
    rmmin->display(&painter,x,y,-2,rateScale);
    rpmax->display(&painter,x,y, 2,rateScale);
    rmmax->display(&painter,x,y,-2,rateScale);
    delete(x);
    delete(y);
}
