#ifndef PARAMCONS_H
#define PARAMCONS_H
#include "param.h"
#include <algorithm>
#include "changepointsontree.h"
//
using namespace std;
//
class ParamCons:public Param
{

public:
	ParamCons(Tree*tree,BinData*bindata);
	virtual ~ParamCons();
	void add(Param * param);
	void display(QPaintDevice * qpd);
	void compute();
	void computeValue(int br,double d,int whichR,double * mean,double *min,double *max);
protected:
	vector< vector< vector<double> > > storerpval;
	vector< vector< vector<double> > > storermval;
	vector< vector< vector<double> > > storerploc;
	vector< vector< vector<double> > > storermloc;
	ChangePointsOnTree * rpmin;
    ChangePointsOnTree * rmmin;
    ChangePointsOnTree * rpmax;
    ChangePointsOnTree * rmmax;
};
#endif
