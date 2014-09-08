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

#include <vector>
#include <cstdint>

#include "lanelet_point.hpp"
#include "LineStrip.hpp"
#include "BoundingBox.hpp"
#include "RegulatoryElement.hpp"

namespace LLet
{

enum SIDE
{
    LEFT = 0,
    RIGHT = 1
};

class LaneletBase;

typedef std::pair< double, double > coord_t;
typedef std::shared_ptr< LaneletBase > lanelet_base_ptr_t;

class LaneletBase
{
public:
    LaneletBase();

    /// returns the (lat, lon) pair at the specified index. Throws if index is out of range. If index is negative, it will return
    /// the -nth element from the back. n=0 refers to the first, n=-1 refers to the last element.
    virtual const point_with_id_t& node_at( SIDE bound, int32_t n ) const;

    /// returns the vector of points describing the left or right bound.
    virtual const std::vector< point_with_id_t >& nodes( SIDE bound ) const;

    /// returns the left and right line strip.
    virtual const std::tuple< strip_ptr_t, strip_ptr_t >& bounds() const = 0;

    virtual BoundingBox bb() const;
    virtual const std::vector< regulatory_element_ptr_t >& regulatory_elements() const = 0;

    bool fits_before( const lanelet_base_ptr_t& other ) const;
    double length() const;
};

}
