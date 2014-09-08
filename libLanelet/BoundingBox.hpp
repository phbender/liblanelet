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

#include <tuple>
#include <iostream>

#include "lanelet_point.hpp"

namespace LLet
{

struct BoundingBox
{
    enum COORDS
    {
        /// min lat
        SOUTH = 0,
        /// min lon
        WEST = 1,
        /// max lat
        NORTH = 2,
        /// max lon
        EAST = 3
    };

    std::tuple< double, double, double, double > bb;

    template< int index >
    const double& get() const
    {
        return std::get<index>(bb);
    }

    BoundingBox( const point_with_id_t& pt ) : bb(std::make_tuple( std::get<LAT>(pt), std::get<LON>(pt), std::get<LAT>(pt), std::get<LON>(pt) ) )
    {

    }

    BoundingBox (const std::tuple< double, double, double, double > _bb) : bb(_bb)
    {

    }

    void extend_point( const point_with_id_t& pt )
    {
        std::get<NORTH>(bb) = std::max( std::get<NORTH>(bb), std::get<LAT>(pt) );
        std::get<SOUTH>(bb) = std::min( std::get<SOUTH>(bb), std::get<LAT>(pt) );
        std::get<EAST>(bb) = std::max( std::get<EAST>(bb), std::get<LON>(pt) );
        std::get<WEST>(bb) = std::min( std::get<WEST>(bb), std::get<LON>(pt) );
    }

    void extend_box( const BoundingBox& other )
    {
        std::get<NORTH>(bb) = std::max( std::get<NORTH>(bb), std::get<NORTH>(other.bb) );
        std::get<SOUTH>(bb) = std::min( std::get<SOUTH>(bb), std::get<SOUTH>(other.bb) );
        std::get<EAST>(bb) = std::max( std::get<EAST>(bb), std::get<EAST>(other.bb) );
        std::get<WEST>(bb) = std::min( std::get<WEST>(bb), std::get<WEST>(other.bb) );
    }
};
}

inline
std::ostream& operator<<(std::ostream& out, const LLet::BoundingBox& box )
{
    out << "foo";
    return out;
}


