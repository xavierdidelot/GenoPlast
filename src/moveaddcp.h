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
#ifndef MOVEADDCP_H
#define MOVEADDCP_H

#include "move.h"
#include "changepointsontree.h"

/**
        @brief This move attempts to add a changepoint
	@author Xavier Didelot <X.Didelot@warwick.ac.uk>
*/
class MoveAddCP : public Move
{
public:
    MoveAddCP(Param * p,bool rp);
    Move * clone() {return new MoveAddCP(*this);};
    void move();
    ~MoveAddCP();
    inline string desc() {if (isrp) return "Adding CP on r+"; else return "Adding CP on r-";}
protected:
    bool isrp;

};

#endif
