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
#ifndef MOVE_H
#define MOVE_H
#include <cmath>
#include "param.h"

/**
        @brief This virtual class represents a move of the MCMC
	@author Xavier Didelot <X.Didelot@warwick.ac.uk>
*/
class Move {
public:
    Move(Param*p);
    virtual Move * clone()=0;
    virtual ~Move()=0;
    virtual void move()=0;///<Performs the move once
    inline void setParam(Param * param) {this->param=param;}
    virtual inline string desc() {return "Any move";}
protected:
    Param*param;///<Parameter set on which the move is performed
};

#endif
