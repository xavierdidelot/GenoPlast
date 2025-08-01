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
#include "bindata.h"

BinData::BinData(int n,int f) {
  this->n=n;this->f=f;
  vector<bool> v=vector<bool>(f,false);
  for (int i=0;i<n+n-1;i++) data.push_back(v);
  len=vector<int>(f,1);
  pi=0.5;
}

BinData::BinData(string filename) {
  ifstream file;
  file.open(filename.data());
  len=vector<int>();
  string str;
  getline(file,str);
  istringstream input0(str);
  int bufI;
  totlen=0;
  while (1) {
      input0 >> bufI;
      if (input0.fail()) break;
      len.push_back(bufI);
      totlen+=bufI;
    }
  while (1) {
      getline(file,str);
      if (file.fail()) break;
      vector<bool> v;
      istringstream input(str);
      bool buf;
      while (1) {
          input >> buf;
          if (input.fail()) break;
          v.push_back(buf);
        }
      data.push_back(v);
    }
  file.close();
  f=data[1].size();
  n=data.size();
  vector<bool> v=vector<bool>(f,false);
  for (int i=0;i<n-1;i++) data.push_back(v);
  for (int i=0;i<n;i++)
    for (int j=0;j<f;j++)
      if (data[i][j]) pi+=len[j];
  pi/=n;
  pi/=totlen;
}

string BinData::output() {
  ostringstream out;
  for (unsigned int i=0;i<len.size();i++) out<<len[i]<<" ";
  out<<endl;
  for (int i=0;i<n;i++) {
      for (unsigned int j=0;j<data[i].size();j++)
        out << get(i,j) << " ";
      out << endl;
    }
  return out.str();
}

BinData::~BinData() {}


