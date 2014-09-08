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

#include "Lanelet.hpp"
#include "LLTree.hpp"
#include "LaneletGraph.hpp"

namespace LLet
{

struct NoPath
{
    lanelet_ptr_t start;
    lanelet_ptr_t dest;
};

class LaneletMap
{
public:
    LaneletMap( std::vector< lanelet_ptr_t > lanelets );
    LaneletMap( std::string filename );

    std::vector< lanelet_ptr_t > query( const BoundingBox& box );
    std::vector< lanelet_ptr_t > shortest_path( const lanelet_ptr_t& start, const lanelet_ptr_t& dest ) const;

    const lanelet_ptr_t& lanelet_by_id( int32_t id ) const;

    const Graph& graph() const;

private:
    void init();
    LLTree _lanelet_tree;
    const std::vector< lanelet_ptr_t > _lanelets;
    LLet::Graph _graph;
    int32_t vertex_id_by_lanelet( const lanelet_ptr_t& lanelet) const;
};

}
