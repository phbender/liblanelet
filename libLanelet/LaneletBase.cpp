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

#include "LaneletBase.hpp"

#include <algorithm>

using namespace LLet;

LaneletBase::LaneletBase()
{
}

const point_with_id_t& LaneletBase::node_at(SIDE bound, int32_t n) const
{
    size_t L = nodes(bound).size();

    int32_t index = n >= 0 ? n : L + n;

    if(index >= L || index < 0)
    {
        throw std::runtime_error("node_at(): index out of range.");
    }

    else
    {
        return nodes(bound)[index];
    }
}

const std::vector< point_with_id_t >& LaneletBase::nodes(SIDE bound) const
{
    if( bound == LEFT )
    {
        return std::get<LEFT>(bounds())->pts();
    }
    else
        return std::get<RIGHT>(bounds())->pts();
}

BoundingBox LaneletBase::bb() const
{
    strip_ptr_t left, right;
    std::tie(left, right) = bounds();
    BoundingBox _bb = left->bb();
    _bb.extend_box( right->bb() );
    return _bb;
}

bool LaneletBase::fits_before(const lanelet_base_ptr_t &other) const
{
    for( auto bound : {LEFT, RIGHT} )
    {
        if( this->node_at(bound, -1) != other->node_at(bound, 0) )
            return false;
    }
    return true;
}

double LaneletBase::length() const
{
    double res = 0;
    const auto& _nodes =  nodes(RIGHT);
    for( int i = 1; i < _nodes.size(); ++i )
    {
        res += dist(_nodes[i-1], _nodes[i]);
    }

    return res;
}
