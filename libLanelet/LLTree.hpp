/*
 *  © 2014 by Philipp Bender <pbender@fzi.de>
 * 
 *  This file is part of libLanelet.
 *
 *  libLanelet is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libLanelet is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libLanelet.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "RTree.h"
#include "BoundingBox.hpp"
#include "Lanelet.hpp"

#include <vector>
#include <tuple>

/* Wraps the RTree and provides some smarter access methods. */

namespace LLet
{

using std::get;

struct LLTree
{
    typedef RTree< int32_t, double, 2 > tree_t;
    void insert(const lanelet_ptr_t& obj);
    std::vector< lanelet_ptr_t > query( const BoundingBox& bb);
private:
    tree_t _tree;
    std::vector< lanelet_ptr_t > _lanelets;

};

}
