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

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/optional.hpp>

#include <boost/foreach.hpp>

#include <MRT/libLanelet/Lanelet.hpp>
#include <MRT/Commons/LocalGeographicCS.hpp>
#include <vector>
#include <vector>
#include <deque>
#include <queue>
#include <map>

#include <MRT/libLanelet//RTree.h>

namespace LLet
{

struct EdgeInfo
{
    double routing_cost;
};

struct VertexInfo
{
    lanelet_ptr_t lanelet;
};

typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::bidirectionalS, VertexInfo, EdgeInfo > Graph;

typedef Graph::vertex_descriptor node_t;
typedef Graph::edge_descriptor arc_t;

template < typename G >
class distance_heuristic : public boost::astar_heuristic< G, double>
{
public:

    distance_heuristic(G& graph, typename G::vertex_descriptor target) : graph(graph), target(target)
    {}
    double operator()(typename G::vertex_descriptor u)
    {
        return 0;
    }

private:
    G& graph;
    typename G::vertex_descriptor target;
};

struct found_goal {}; // exception for termination

// visitor that terminates when we find the goal
template <class Vertex>
class astar_goal_visitor : public boost::default_astar_visitor
{
public:
    astar_goal_visitor(Vertex goal) : m_goal(goal) {}
    template <class Graph>
    void examine_vertex(Vertex u, Graph& g) {
        if(u == m_goal)
            throw found_goal();
    }
private:
    Vertex m_goal;
};

template< typename G >
struct Dijkstra
{
    typedef typename G::vertex_descriptor vertex_t;
    G& graph;
    vertex_t source;

    std::vector< vertex_t > _predecessors;
    std::vector< double > _distances;

    void abort_if_vertex_invalid( vertex_t v )
    {
        if( v < 0 || v >= boost::num_vertices(graph) )
        {
            std::cerr << "invalid vertices in Dijkstra." << std::endl;
            std::cerr << v << " (" << boost::num_vertices(graph) << ")" << std::endl;
            abort();
        }
    }

    Dijkstra( G& graph, vertex_t source ) : graph(graph), source(source)
    {
        using namespace boost;

        abort_if_vertex_invalid( source );
        auto num_vertices = boost::num_vertices( graph );
        this->_predecessors.resize( num_vertices );
        this->_distances.resize( num_vertices );
        std::fill( _distances.begin(), _distances.end(), std::numeric_limits<double>::max() );

        boost::dijkstra_shortest_paths
                (graph, source,
                 predecessor_map(_predecessors.data()).distance_map(_distances.data()).
                 weight_map(get(&EdgeInfo::routing_cost, graph)));
    }

    std::deque< vertex_t > shortest_path( vertex_t target )
    {
        abort_if_vertex_invalid( target );
        const static std::deque< vertex_t > EMPTY_LIST;
        if( !reached(target) )
            return EMPTY_LIST;

        std::deque< vertex_t > sp;
        for( auto v = target; v != source ; v = _predecessors[v] )
        {
            sp.push_front( v );
        }

        sp.push_front( source );

        assert( sp.empty() || (sp.front() == source && sp.back() == target) && "Dijkstra returns either empty list or path from source to target." );

        return sp;
    }

    bool reached( vertex_t target )
    {
        static const double BIG_NUM = 1e9;
        return _distances[target] < BIG_NUM;
    }
};

template<typename G>
boost::optional< std::vector< typename G::vertex_descriptor > >
dijkstra_shortest_path( G& g, typename G::vertex_descriptor from, typename G::vertex_descriptor to )
{
    boost::optional< std::vector< typename G::vertex_descriptor > > result;
     auto dij = Dijkstra< G >( g, from );

     if(!dij.reached(to))
         return result;

     auto sp = dij.shortest_path( to );

     std::vector< typename G::vertex_descriptor > my_sp( sp.size() );
     std::copy(sp.begin(), sp.end(), my_sp.begin());

     assert( ( my_sp.empty() && sp.empty() ) || (my_sp.front() == sp.front() && my_sp.back() == sp.back() ) && "deque and vector are equal."  );

     result = my_sp;
     return result;
}

}

