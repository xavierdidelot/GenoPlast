/***************************************************************************
 *   Copyright (C) 2008 by Xavier Didelot  *
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
#include "moveaddcp.h"

MoveAddCP::MoveAddCP(Param * p,bool rp)
        : Move(p) {
    isrp=rp;
}

void MoveAddCP::move() {
    ChangePointsOnTree * r;
    if (isrp) r=param->getRp();
    else r=param->getRm();
    double oldmarglh=param->getMarglh();
    double val=param->getPrior()->raternd();
    double loc=rand()/(RAND_MAX+1.0)*param->getTree()->getTotbralen();
    int br=0;
    while (loc>param->getTree()->getNode(br)->getDist()) loc-=param->getTree()->getNode(br++)->getDist();
    vector<double> * locvec;
    vector<double> * valvec;
    r->getCompoBranch(br,&locvec,&valvec);
    int i=0;
    while (locvec->at(i)<loc) {
        i++;
    }
    locvec->insert(locvec->begin()+i,loc);
    valvec->insert(valvec->begin()+i,val);
    r->incrTotcp();
    r->propagate();
    param->computeMarglh(false);
    double u=rand()/(RAND_MAX+1.0);
    if (log(u)>log(param->getPrior()->getFreqCP())-log(r->getTotcp())+param->getMarglh()-oldmarglh) {
        if (verbose) cout<<"Adding rejected "<<oldmarglh<<" "<<param->getMarglh()<<endl;
        locvec->erase(locvec->begin()+i);
        valvec->erase(valvec->begin()+i);
        r->decrTotcp();
        r->propagate();
        param->setMarglh(oldmarglh);
    } else {
        if (verbose) cout<< "Adding accepted "<<oldmarglh<<" "<<param->getMarglh()<<endl;
    }
}

MoveAddCP::~MoveAddCP() {}


