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

#include <iostream>

#include <MRT/libLanelet/llet_xml.hpp>
#include <MRT/libLanelet/Lanelet.hpp>
#include <MRT/libLanelet/LaneletMap.hpp>
#include <MRT/libLanelet/BoundingBox.hpp>

#include <boost/foreach.hpp>

#include <vector>
#include <tuple>
#include <memory>

using namespace LLet;

int main (int argc, char* argv[]) {
    std::string source = argv[1];
    LaneletMap the_map( source );

    BoundingBox world( std::make_tuple(-180, -180, 180, 180) );
    auto query_result = the_map.query(world);

    // print all attributes of each lanelet
    for( const lanelet_ptr_t& lanelet: query_result )
    {
        const AttributeMap& attributes = lanelet->attributes();
        for( const auto& a: attributes )
        {
            std::cout << a.first << ": " << a.second.as_string() << std::endl;
        }
    }

    // exploiting the lanelet graph directly
    const Graph& G = the_map.graph();

    BOOST_FOREACH(const auto& edge, boost::edges(G))
    {
        auto src_vtx = boost::source(edge, G);
        auto dest_vtx = boost::target(edge, G);

        std::cout << G[src_vtx].lanelet << " --> " << G[dest_vtx].lanelet << std::endl;
    }
}
