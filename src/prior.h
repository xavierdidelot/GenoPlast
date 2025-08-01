#ifndef PRIOR_H
#define PRIOR_H
#include <cmath>
#include <cstdlib>
//

//
class Prior  
{
public:
	Prior();
	inline double raternd() //{return (rand()/(RAND_MAX+1.0))*10.0;}
	{return -log(rand()/(RAND_MAX+1.0))/rateparam;}
	inline double ratepdf(double val) //{if (val>0.0 && val<10.0) return 0.1; else return 0.0;}
	{if (val>0.0) return rateparam*exp(-rateparam*val); else return 0.0;}
	inline double getrateparam() {return rateparam;}
	inline double getFreqCP() {return freqCP;}
	int drawNumCP();
protected:
	double rateparam;
	double freqCP;
friend class EditPriorImpl;
};

#endif
