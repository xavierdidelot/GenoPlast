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
#ifndef CHANGEPOINTSONTREE_H
#define CHANGEPOINTSONTREE_H
#include <vector>
#include "param.h"
#include <stdlib.h>
#include <QtXml>
#include <QtGui>

using namespace std;

/**
        @brief This represents a changepoint structure on a genealogical tree
	@author Xavier Didelot <X.Didelot@warwick.ac.uk>
*/
class ChangePointsOnTree{
public:
    ChangePointsOnTree(Param * p);
    ChangePointsOnTree(Param * p,QDomElement * dom);
    ~ChangePointsOnTree();
    ///This returns the composition of a branch in terms of interval where the value is constant
    inline void getCompoBranch(int br,vector<double> ** locs,vector<double> ** vals) {
    *locs=positions[br];
    *vals=values[br];
    };
    inline int getTotcp() {return totcp;}///<Get accessor to the number of changepoints
    void propagate();///<Propagate the values in the structure such that the first value in each branch is equal to the last value in the parental branch
    inline void incrTotcp() {totcp++;}///<Increments the number of changepoints
    inline void decrTotcp() {totcp--;}///<Decreases the number of changepoints
    inline double getTopval() {return topval;}///<Get accessor to the value at the top of the tree
    inline void setTopval(double val) {topval=val;}///<Set accessor to the value at the top of the tree
    inline double * getTopvalPtr() {return &topval;}///Returns a pointer to the value at the top of the tree
    void exportXML(ostream * out);
    void display(QPainter * painter,vector<double>*x,vector<double>*y,int mode,double scale);
protected:
    double topval;///<Value at the top of the tree
    vector<double> ** positions;///<Set of positions of changepoints
    vector<double> ** values;///<Set of values associated with changepoints
    Param * param;
    int totcp;///<Number of changepoints
};

#endif
