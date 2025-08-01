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
#include "moveremovecp.h"

MoveRemoveCP::MoveRemoveCP(Param * p,bool rp)
        : Move(p) {
    isrp=rp;
}

void MoveRemoveCP::move() {
    ChangePointsOnTree * r;
    if (isrp) r=param->getRp();
    else r=param->getRm();
    double oldmarglh=param->getMarglh();
    if (r->getTotcp()==0) {
        if (verbose) cout<<"Removing impossible"<<endl;
        return;
    }
    unsigned int w=(unsigned int)floor(rand()/(RAND_MAX+1.0)*r->getTotcp())+1;
    int i=0;
    vector<double> * locvec;
    vector<double> * valvec;
    r->getCompoBranch(i,&locvec,&valvec);
    while (locvec->size()-1<w) {
        w-=locvec->size()-1;
        i++;
        r->getCompoBranch(i,&locvec,&valvec);
    }
    w--;
    double loc=locvec->at(w);
    double val=valvec->at(w);
    locvec->erase(locvec->begin()+w);
    valvec->erase(valvec->begin()+w);
    r->decrTotcp();
    r->propagate();
    param->computeMarglh(false);
    double u=rand()/(RAND_MAX+1.0);
    if (log(u)>log(1.0+r->getTotcp())-log(param->getPrior()->getFreqCP())+param->getMarglh()-oldmarglh) {
        if (verbose) cout<<"Removing rejected "<<param->getMarglh()<<" "<<oldmarglh<<endl;
        locvec->insert(locvec->begin()+w,loc);
        valvec->insert(valvec->begin()+w,val);
        r->incrTotcp();
        r->propagate();
        param->setMarglh(oldmarglh);
    } else {
        if (verbose) cout<< "Removing accepted "<<oldmarglh<<" "<<param->getMarglh()<<endl;
    }
}

MoveRemoveCP::~MoveRemoveCP() {}


