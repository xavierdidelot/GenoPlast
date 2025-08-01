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
#include "node.h"

Node::Node() {
    left=NULL;
    right=NULL;
    father=NULL;
    dist=0.0;
    id=0;
}

Node::Node(string * newick,Node * father,vector<Node*> * nodes) {
    this->father=father;
//Find central comma if it exists
    int depth=0;
    int found=0;
    for (unsigned int i=0;i<newick->length();i++) {
        if (newick->at(i)=='(') {
            depth++;
            continue;
        }
        if (newick->at(i)==')') {
            depth--;
            continue;
        }
        if (newick->at(i)==',' && depth==1) {
            found=i;
            break;
        }
    }
//Find last ':' and read dist
    int found2=0;
    depth=0;
    for (unsigned int i=0;i<newick->length();i++) {
    	if (newick->at(i)=='(') depth++;
        if (newick->at(i)==':') found2=i;
        if (newick->at(i)==')') {depth--;if (depth==0) found2=0;}
    }
    if (found2>0) {
        istringstream input(newick->substr(found2+1,newick->length()));
        input >> dist;
    } else {
        dist=0.0;
        found2=newick->length();
    }

    if (found==0) {
//Leaf
        left=NULL;
        right=NULL;
        istringstream input(*newick);
        input >> id;
        nodes->push_back(this);
    } else {
//Internal node
        id=-1;
        string leftStr =newick->substr(1,found-1);
        string rightStr=newick->substr(found+1,found2-2-found);
        left=new Node(&leftStr,this,nodes);
        right=new Node(&rightStr,this,nodes);
        nodes->push_back(this);
    }
}

string Node::newick() {
    ostringstream idstream;
    ostringstream diststream;
    idstream<<id;
    diststream<<dist;
    if (left==NULL) return idstream.str()+":"+diststream.str();
    else return "("+left->newick()+","+right->newick()+")"+idstream.str()+":"+diststream.str();
}

Node::~Node() {
    if (left ) delete(left );
    if (right) delete(right);
}


