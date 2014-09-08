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

#include "LaneletMap.hpp"
#include "llet_xml.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <MRT/libLanelet/LaneletGraph.hpp>
#include <MRT/Commons/prettyprint.hpp>

using namespace LLet;

LaneletMap::LaneletMap(std::vector<lanelet_ptr_t> lanelets) : _lanelets(lanelets)
{
    init();
}

LaneletMap::LaneletMap(std::string filename) : _lanelets(LLet::parse_xml(filename))
{
    init();
}

std::vector<lanelet_ptr_t> LaneletMap::query(const BoundingBox &box)
{
    return _lanelet_tree.query(box);
}

std::vector<lanelet_ptr_t> LaneletMap::shortest_path(const lanelet_ptr_t &start, const lanelet_ptr_t &dest) const
{
    int32_t start_index = vertex_id_by_lanelet(start);
    int32_t dest_index = vertex_id_by_lanelet(dest);

    auto sp = dijkstra_shortest_path(graph(), start_index, dest_index);

    if( !sp || (*sp).empty() )
    {
        NoPath exception;
        exception.start = start;
        exception.dest = dest;
        throw exception;
    }

    std::vector< lanelet_ptr_t > sp_ll(sp->size());

    std::transform(sp->cbegin(), sp->cend(), sp_ll.begin(), [this](int32_t index){ return graph()[index].lanelet;});

    std::cout << sp_ll << std::endl;

    return sp_ll;
}

const lanelet_ptr_t &LaneletMap::lanelet_by_id(int32_t id) const
{
    auto pos = std::find_if(_lanelets.cbegin(), _lanelets.cend(), [&id](const lanelet_ptr_t& ll) {return ll->id() == id;});
    if( pos != _lanelets.cend() )
        return *pos;
    else
    {
        boost::format fmt("trying to retieve lanelet with unknown id: %i");
        throw std::runtime_error( (fmt % id).str() );
    }
}

const Graph &LaneletMap::graph() const
{
    return _graph;
}

void LaneletMap::init()
{    
    for( int i = 0; i < _lanelets.size(); ++i )
    {
        Graph::vertex_descriptor vtx = boost::add_vertex(_graph);
        _graph[vtx].lanelet = _lanelets[i];
        // graph vertex descriptors and _lanelet indices should be in sync now.
        assert( vtx == i );
        assert( vertex_id_by_lanelet(_lanelets[i]) == i );
        _lanelet_tree.insert(_lanelets[i]);
    }

    for( const auto& src: _lanelets )
    {
        int32_t index_src = this->vertex_id_by_lanelet(src);
        double len = src->length();
        auto lls_around = this->query( src->bb() );
        for( const auto& other: lls_around )
        {
            if(src->fits_before(other))
            {
                int32_t index_dest = this->vertex_id_by_lanelet( other );
                bool inserted;
                Graph::edge_descriptor new_edge;
                std::tie(new_edge, inserted) = boost::add_edge(index_src, index_dest, this->_graph);
                assert(inserted);
                EdgeInfo info;
                info.routing_cost = len;
                _graph[new_edge] = info;
            }
        }
    }

}

int32_t LaneletMap::vertex_id_by_lanelet(const lanelet_ptr_t &lanelet) const
{
    auto pos = std::find(_lanelets.cbegin(), _lanelets.cend(), lanelet);
    if( pos == _lanelets.cend() )
        throw std::runtime_error("lanelet not found.");
    else
        return std::distance(_lanelets.cbegin(), pos);
}
