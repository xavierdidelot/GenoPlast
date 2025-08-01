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
#include "moveposcp.h"

MovePosCP::MovePosCP(Param * p,bool rp)
        : Move(p) {
    isrp=rp;
}

void MovePosCP::move() {
    ChangePointsOnTree * r;
    if (isrp) r=param->getRp();
    else r=param->getRm();
    double oldmarglh=param->getMarglh();
        if (r->getTotcp()==0) {
        if (verbose) cout<<"Updating pos impossible"<<endl;
        return;
    }
    unsigned int w=(unsigned int)floor(rand() / (RAND_MAX + 1.0) * (r->getTotcp()))+1;
    vector<double> * locvec;
    vector<double> * valvec;
    int i=0;
    r->getCompoBranch(i,&locvec,&valvec);
    while (locvec->size()-1<w) {
        w-=locvec->size()-1;
        i++;
        r->getCompoBranch(i,&locvec,&valvec);
    }
    w--;
    //toChange=&(locvec->at(w));
    double oldval=locvec->at(w);
    locvec->at(w)=rand()/(RAND_MAX+1.0)*param->getTree()->getNode(i)->getDist();
    while (w>0&&locvec->at(w)<locvec->at(w-1)) {swap(locvec,w-1,w);swap(valvec,w-1,w);w--;}
    while (w<locvec->size()-1 && (locvec->at(w))>(locvec->at(w+1)))
    {swap(locvec,w,w+1);swap(valvec,w,w+1);w++;}
    r->propagate();
    param->computeMarglh(false);
    double u=rand() / (RAND_MAX + 1.0);
    if (log(u)>param->getMarglh()-oldmarglh) {
        if (verbose) cout<<"Updating pos rejected "<<oldmarglh<<" "<<param->getMarglh()<<endl;
        locvec->at(w)=oldval;
        while (w>0&&locvec->at(w)<locvec->at(w-1)) {swap(locvec,w-1,w);swap(valvec,w-1,w);w--;}
        while (w<locvec->size()-1 && (locvec->at(w))>(locvec->at(w+1)))
        {swap(locvec,w,w+1);swap(valvec,w,w+1);w++;}
        r->propagate();
        param->setMarglh(oldmarglh);
    } else {
        if (verbose) cout<< "Updating pos accepted "<<oldmarglh<<" "<<param->getMarglh()<<endl;
    }
}

void MovePosCP::swap(vector<double>*v1,int ind1,int ind2)
{
double tmp;
tmp=v1->at(ind1);
v1->at(ind1)=v1->at(ind2);
v1->at(ind2)=tmp;
}

MovePosCP::~MovePosCP() {}
