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
#include "movevaluecp.h"

MoveValueCP::MoveValueCP(Param * p,bool rp)
        : Move(p) {
    isrp=rp;
}


void MoveValueCP::move() {
    ChangePointsOnTree * r;
    if (isrp) r=param->getRp();
    else r=param->getRm();
    double oldmarglh=param->getMarglh();
    unsigned int w=(unsigned int)floor(rand() / (RAND_MAX + 1.0) * (r->getTotcp()+1))+1;
    double * toChange;
    if (w==1) toChange=r->getTopvalPtr();
    else {
        vector<double> * locvec;
        vector<double> * valvec;
        w--;
        int i=0;
        r->getCompoBranch(i,&locvec,&valvec);
        while (locvec->size()-1<w) {
            w-=locvec->size()-1;
            i++;
            r->getCompoBranch(i,&locvec,&valvec);
        }
        toChange=&(valvec->at(w));
    }
    double oldval=*toChange;
    double u=rand()/(RAND_MAX+1.0);
    if (u<1.5) {//Additive move
        u=(rand()/ (RAND_MAX + 1.0)-0.5)/10.0;
        *toChange+=u;
    } else {//Independent move
            u=param->getPrior()->raternd();
            *toChange=u;
    }
    if (*toChange<0.0) {
        *toChange=oldval;
        if (verbose) cout<<"Updating val rejected because out of range"<<endl;
        return;
    }
    r->propagate();
    param->computeMarglh(false);
    u=rand() / (RAND_MAX + 1.0);
    if (log(u)>log(param->getPrior()->ratepdf(*toChange))-log(param->getPrior()->ratepdf(oldval))+param->getMarglh()-oldmarglh) {
        if (verbose) cout<<"Updating val rejected "<<oldmarglh<<" "<<param->getMarglh()<<endl;
        *toChange=oldval;
        r->propagate();
        param->setMarglh(oldmarglh);
    } else {
        if (verbose) cout<< "Updating val accepted "<<oldmarglh<<" "<<param->getMarglh()<<endl;
    }
}

MoveValueCP::~MoveValueCP() {}


