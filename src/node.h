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
#ifndef NODE_H
#define NODE_H
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

/**
        @brief A node of the tree
	@author Xavier Didelot <X.Didelot@warwick.ac.uk>
*/
class Node {
protected:
    Node * left;///<Left daughter node
    Node * right;///<Right daughter node
    Node * father;///<Father node
    double dist;///<Distance to the current node to its father
    int id;///<Index of the node
public:
    Node();
    Node(string * newick,Node * father,vector<Node*> * nodes);
    ~Node();
    string newick();///<Convert the node to a Newick string
    inline int getId() {
        return id;
    }///<Returns the index of the node
    inline void setId(int i) {
        id=i;
    }///<Sets the index of the node
    inline double getDist() {
        return dist;
    }///<Returns the distance of the node to its father
    inline void setDist(double d) {
        dist=d;
    }///<Sets the distance of the node to its father
    inline Node * getLeft() {
        return left;
    }///<Returns the left daughter node
    inline Node * getRight() {
        return right;
    }///<Returns the right daughter node
    inline Node * getFather() {
        return father;
    }///<Returns the father node
    inline void setFather(Node * f) {father=f;}
    inline void setLeft(Node * l) {left=l;}
    inline void setRight(Node * r) {right=r;}

};

#endif
