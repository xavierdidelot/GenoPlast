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
#ifndef TREE_H
#define TREE_H
#include "node.h"
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <QtGui>

using namespace std;

/**
        @brief Genealogy of the isolates under study
	@author Xavier Didelot <X.Didelot@warwick.ac.uk>
*/
class Tree {
protected:
    Node * root;///<Root node of the genealogy
    vector<Node*> nodes;///<Vector of all the nodes in the genealogy
    int n;///<Number of isolates
    double totbralen;///<Total branch length
    void makeYforLeaves(vector<double> * v,Node * cur,int *x);
    vector<QString> names;
public:
    Tree(string newick,bool isFilename=true);///<Creates a tree from a Newick file
    Tree(int n);
    ~Tree();
    string newick();///<Returns a Newick description of the tree
    inline int getN() {
        return n;
    }///<Returns the number of isolates
    inline Node* getNode(int i) {
        return nodes[i];
    }///<Returns a node given its index
    inline double getDist(int i) {return nodes[i]->getDist();}
    inline double getTotbralen() {return totbralen;}
    void getXY(vector<double> **x,vector<double> **y);
    void display(QPainter *painter,vector<double> *x,vector<double> *y);
    void loadNames(QString qstr);
    inline QString getName(int i) {return names.at(i);};
};
#endif
