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
#ifndef BINDATA_H
#define BINDATA_H
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

using namespace std;

/**
        @brief Binary feature data and ancestral states for the internal nodes of the topology
 @author Xavier Didelot <X.Didelot@warwick.ac.uk>
*/
class BinData {
    public:
      BinData(int n,int f);
      BinData(string filename);///<Reads in a binary matrix stored in a file, with each line representing an isolate and each column representing a binary feature
      ~BinData();
      inline bool get(int i,int j) {
          return data[i][j];
        } ///<Get accessor to the data and ancestral states
      inline void set(int i,int j,bool b) {
          data[i][j]=b;
        } ///<Set accessor to the data and ancestral states
      string output();///<Print the data to a stream
      inline int getN() {
        return n;
      }///<Returns the number of isolates
      inline int getF() {
        return f;
      }///<Returns the number of features
      inline int getLen(int i) {return len[i];}
      inline int getTotlen() {return totlen;}
      inline void addCore(int l) {
        for (int i=0;i<n+n-1;i++) data[i].push_back(true);
        len.push_back(l);
        totlen+=l;
        f++;
      }
      inline double getPi() {return pi;}
    protected:
      int n;///<Number of isolates
      int f;///<Number of features
      vector< vector<bool> > data;///<Data for the presence/absence of features for the isolates and ancestral nodes
      vector<int> len;///<Length of each feature
      int totlen;
      double pi;
  };
  
#endif
