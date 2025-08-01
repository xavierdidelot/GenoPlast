#include "movevalueboth.h"
//
MoveValueBoth::MoveValueBoth( Param * p ) : Move(p) {}

void MoveValueBoth::move() {
    ChangePointsOnTree * rp=param->getRp();
    ChangePointsOnTree * rm=param->getRm();
    double oldmarglh=param->getMarglh();
    double * toChangeRp=rp->getTopvalPtr();
    double * toChangeRm=rm->getTopvalPtr();
    double oldvalRp=*toChangeRp;
    double oldvalRm=*toChangeRm;
    double u=rand()/(RAND_MAX+1.0);
    if (u<0.5) {//Additive move
        u=(rand()/ (RAND_MAX + 1.0)-0.5)/10.0*(param->getPrior()->getMaxVal()-param->getPrior()->getMinVal());
        *toChangeRp+=u;
        *toChangeRm+=u;
    } else {//Independent move
            u=param->getPrior()->drawRate();
            *toChangeRp=u;
            u=param->getPrior()->drawRate();
            *toChangeRm=u;
    }
    if (*toChangeRp<param->getPrior()->getMinVal() || *toChangeRp>param->getPrior()->getMaxVal()
    ||  *toChangeRm<param->getPrior()->getMinVal() || *toChangeRm>param->getPrior()->getMaxVal()) {
        *toChangeRp=oldvalRp;
        *toChangeRm=oldvalRm;
        if (verbose) cout<<"Updating val rejected because out of range"<<endl;
        return;
    }
    rp->propagate();rm->propagate();
    param->computeMarglh(false);
    u=rand() / (RAND_MAX + 1.0);
    if (log(u)>param->getMarglh()-oldmarglh) {
        if (verbose) cout<<"Updating val rejected "<<oldmarglh<<" "<<param->getMarglh()<<endl;
        *toChangeRp=oldvalRp;
        *toChangeRm=oldvalRm;
        rp->propagate();
        rm->propagate();
        param->setMarglh(oldmarglh);
    } else {
        if (verbose) cout<< "Updating val accepted "<<oldmarglh<<" "<<param->getMarglh()<<endl;
    }
}

MoveValueBoth::~MoveValueBoth() {}
