#ifndef POWER_H
#define POWER_H
//
#include "param.h"
#include "selectmovesimpl.h"

using namespace std;
//
class Power : public QObject
{
Q_OBJECT
public:
	Power();
	inline void setParam(Param * p) {param=p;}
	void config();
	void runTest(int burnin,int additional,int thinin);
protected:
	Param * param;
	Param * infer;
	double * toChange;
	bool stop;
	double simuAndMetro(int burnin,int additional,int thinin,int f);
public slots:
	void slotProgress(int i);
	void cancelled();
signals:
	void signalProgress(int i);
	void updateMain();
};
#endif





