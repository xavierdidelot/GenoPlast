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
#include "tree.h"

Tree::Tree(int n) {
  names=vector<QString>();
  this->n=n;
  for (int i=0;i<n;i++) names.push_back(QString::number(i));
  for (int i=0;i<n-1;i++) names.push_back("");
  nodes=vector<Node*>(n+n-1);
  Node ** toCoal=(Node **)calloc(n,sizeof(Node*));
  int k=n;
  double time=0.0;
  double age;
  Node * node;
  for (int i=0;i<n;i++) {nodes[i]=new Node();nodes[i]->setId(i);toCoal[i]=nodes[i];}
  while (k>1) {
      time-=2.0/(k*(k-1.0))*log(rand()/(RAND_MAX+1.0));
      int a=(int)floor(rand()/(RAND_MAX+1.0)*k);
      int b=a;
      while (b==a) b=(int)floor(rand()/(RAND_MAX+1.0)*k);
      nodes[n+n-k]=new Node();
      nodes[n+n-k]->setId(n+n-k);
      nodes[n+n-k]->setLeft(toCoal[a]);
      nodes[n+n-k]->setRight(toCoal[b]);
      toCoal[a]->setFather(nodes[n+n-k]);
      toCoal[b]->setFather(nodes[n+n-k]);
    age=0.0;node=toCoal[a];while (node->getLeft()!=NULL) {node=node->getLeft();age+=node->getDist();}
      toCoal[a]->setDist(time-age);
      age=0.0;node=toCoal[b];while (node->getLeft()!=NULL) {node=node->getLeft();age+=node->getDist();}
      toCoal[b]->setDist(time-age);
      toCoal[a]=nodes[n+n-k];
      toCoal[b]=toCoal[k-1];
      k--;
    }
  root=nodes[n+n-2];
  free(toCoal);
//Make TMRCA=1.0
  double tmrca=0.0;
  Node * cur=nodes[0];
  while (cur->getFather()!=NULL) {tmrca+=cur->getDist();cur=cur->getFather();}
  for (unsigned int i=0;i<nodes.size();i++) nodes[i]->setDist(nodes[i]->getDist()/tmrca);
//Calculate totbralen
  totbralen=0.0;
  for (unsigned int i=0;i<nodes.size();i++) totbralen+=nodes[i]->getDist();
}

Tree::Tree(string newickfile,bool isFilename) {
  names=vector<QString>();
  string newick;
  if (isFilename) {
      //Read Newick string from file
      ifstream newickFile;
      newickFile.open(newickfile.data());
      getline(newickFile,newick);
      newickFile.close();
    } else newick=newickfile;
//Remove final ';'
  if (newick.at(newick.length()-1)==';') newick=newick.substr(0,newick.length()-1);
  vector<Node*> all;
//Create tree
  root=new Node(&newick,NULL,&all);
  n=(all.size()/2)+1;
  vector<double> ages;
  for (unsigned int i=0;i<all.size();i++) {
      double age=0.0;
      Node * node=all[i];
      while (node->getLeft()!=NULL) {node=node->getLeft();age+=node->getDist();};
      ages.push_back(age);
    }
//Create list of nodes ordered by age
  nodes=vector<Node*>(n+n-1);
  for (unsigned int i=0;i<all.size();i++) {
      if (all.at(i)->getId()==-1) {
          int idnew=0;
          for (unsigned int j=0;j<all.size();j++) if (ages[j]<ages[i] || (ages[j]==ages[i] && j<i)) idnew++;
          all.at(i)->setId(idnew);
        }
      nodes.at(all.at(i)->getId())=all.at(i);
    }
//Make TMRCA=1.0
  double tmrca=0.0;
  Node * cur=nodes[0];
while (cur->getFather()!=NULL) {tmrca+=cur->getDist();cur=cur->getFather();}
  for (unsigned int i=0;i<all.size();i++) all[i]->setDist(all[i]->getDist()/tmrca);
//Calculate totbralen
  totbralen=0.0;
  for (unsigned int i=0;i<all.size();i++) totbralen+=all[i]->getDist();
  for (int i=0;i<n;i++) names.push_back(QString::number(i));
  for (int i=0;i<n-1;i++) names.push_back("");
}

string Tree::newick() {
  return root->newick()+";";
}

Tree::~Tree() {
  if (root) delete(root);
}

void Tree::getXY(vector<double>**xx,vector<double>**yy)
{
  vector<double> * x=new vector<double>();
  vector<double> * y=new vector<double>();
  //Calculate x coordinates
  for (int i=0;i<n+n-2;i++) x->push_back(getNode(i)->getDist());
  x->push_back(0.0);
  for (int i=n+n-3;i>=0;i--) x->at(i)=x->at(i)+x->at(getNode(i)->getFather()->getId());
  //Calculate y coordinates
  for (int i=0;i<n+n-1;i++) y->push_back(0);
  int k=0;
  makeYforLeaves(y,getNode(n+n-2),&k);
  for (int i=0;i<n;i++) y->at(i)=y->at(i)/n;
  for (int i=n;i<n+n-1;i++) y->at(i)=(y->at(getNode(i)->getLeft()->getId())+y->at(getNode(i)->getRight()->getId()))/2.0;
  *xx=x;
  *yy=y;
}

void Tree::display(QPainter *painter,vector<double>*x,vector<double>*y) {
  
  QBrush brush=QBrush(Qt::black);
  //Horizontal lines
  for (int i=0;i<n+n-2;i++) {
      QRectF rect(x->at(i),y->at(i)-0.002,  (x->at(getNode(i)->getFather()->getId())-x->at(i)), 0.004);
      painter->fillRect(rect,brush);
    }
  //Vertical lines
  for (int i=n;i<n+n-1;i++) {
      QRectF rect(x->at(i)-0.002,y->at(getNode(i)->getLeft()->getId()),
                  0.004,y->at(getNode(i)->getRight()->getId())-y->at(getNode(i)->getLeft()->getId()));
      painter->fillRect(rect,brush);
    }
}

void Tree::makeYforLeaves(vector<double> * v,Node * cur,int *x) {
  if (cur->getLeft()==NULL) v->at(cur->getId())=(*x)++;
else {makeYforLeaves(v,cur->getLeft(),x);makeYforLeaves(v,cur->getRight(),x);};
}

void Tree::loadNames(QString qstr) {
  QFile file(qstr);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
  names=vector<QString>();
  QTextStream in(&file);
  while (!in.atEnd()) names.push_back(in.readLine());
  file.close();
  while (names.size()<(unsigned int)(n*n-1)) names.push_back("");
}
