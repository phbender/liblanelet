/*
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

#include <gtest/gtest.h>
#include <algorithm>

#include <MRT/libLanelet/llet_xml.hpp>
#include <MRT/libLanelet/Lanelet.hpp>
#include <MRT/libLanelet/LaneletMap.hpp>
#include <MRT/libLanelet/RegulatoryElement.hpp>
#include <MRT/Commons/prettyprint.hpp>

#include <boost/graph/adjacency_list.hpp>

namespace {

using namespace LLet;

class LaneletTest : public ::testing::Test {
protected:

    LaneletTest() : map_fn("libLanelet/sample.osm"), lanelets( parse_xml(map_fn) ), _id(22064)
    {

    }

    const std::string map_fn;

    std::vector< lanelet_ptr_t > lanelets;
    const int32_t _id;

};

TEST_F(LaneletTest, ConstructionOfLaneletsIsCorrect) {

    ASSERT_EQ(lanelets.size(), 6);

    auto pos = std::find_if(lanelets.cbegin(), lanelets.cend(), [this] (const lanelet_ptr_t& ll) { return ll->id() == this->_id; });
    ASSERT_NE(pos, lanelets.cend());

    lanelet_ptr_t llet = *pos;
    ASSERT_EQ(llet->id(), _id);
}

TEST_F(LaneletTest, PointGeometryFunctions)
{
    typedef std::pair< double, double > vec_t;

    vec_t a(std::make_pair(1, 0)), b(std::make_pair(0, 1));
    vec_t c(std::make_pair(0, -1));

    ASSERT_DOUBLE_EQ(abs(a), 1.0);
    ASSERT_DOUBLE_EQ(abs(b), 1.0);

    double pi_2 = M_PI/2.0;

    ASSERT_DOUBLE_EQ(angle(a, b), pi_2);
    ASSERT_DOUBLE_EQ(angle(a, c), -pi_2);
}

TEST_F(LaneletTest, MapInsertionAndRetrieval)
{
    std::shared_ptr< LaneletMap > map;
    ASSERT_NO_THROW( map = std::make_shared<LaneletMap>(lanelets) );

    lanelet_ptr_t the_ll;
    ASSERT_NO_THROW(the_ll = map->lanelet_by_id(_id));
    ASSERT_THROW(map->lanelet_by_id(-1000000), std::runtime_error);
    ASSERT_NE(the_ll, nullptr);

    BoundingBox world( std::make_tuple(-180, -180, 180, 180) );
    BoundingBox far_off( std::make_tuple(-180, -180, -170, -170) );

    ASSERT_EQ( map->query(world).size(), lanelets.size() );
    ASSERT_EQ( map->query(far_off).size(), 0 );
}

TEST_F(LaneletTest, NodeAccessFunctionsAreConsistent)
{    
    LaneletMap map( map_fn );

    lanelet_ptr_t llet = map.lanelet_by_id( _id );
    std::vector< point_with_id_t > nodes_left(llet->nodes(LEFT)), nodes_right(llet->nodes(RIGHT));

    ASSERT_EQ( nodes_left.size(), 10 );
    ASSERT_EQ( nodes_right.size(), 8 );

    ASSERT_EQ( llet->node_at(LEFT, -1), llet->nodes(LEFT).back() );
    ASSERT_EQ( llet->node_at(LEFT, -nodes_left.size()), llet->nodes(LEFT).front() );

    ASSERT_THROW( llet->node_at(LEFT, -nodes_left.size() - 1), std::runtime_error );
}

TEST_F(LaneletTest, ResortingOfWaysWorksCorrectly)
{
    LaneletMap map( map_fn );
    lanelet_ptr_t llet = map.lanelet_by_id( _id );

    std::vector< point_with_id_t > nodes_right(llet->nodes(RIGHT)), nodes_left(llet->nodes(LEFT));

    point_with_id_t first_right(nodes_right.front()), last_right(nodes_right.back());
    point_with_id_t first_left(nodes_left.front()), last_left(nodes_left.back());

    double dist_left  = dist(first_left, last_left);
    double dist_right = dist(first_right, last_right);

    ASSERT_NEAR(dist_right, 16.6, 0.1);
    ASSERT_NEAR(dist_left, 23.5, 0.1);

    ASSERT_EQ( std::get< ID >(first_left), 581367 );
    ASSERT_EQ( std::get< ID >(last_left), 581380 );

    // this one is pretty difficult to test, test may succeed incidental
    ASSERT_EQ( std::get< ID >(first_right), 581375);
    ASSERT_EQ( std::get< ID >(last_right), 581381);

    // test another one
    llet = map.lanelet_by_id(22060);
    ASSERT_EQ(std::get<ID>(llet->node_at(LEFT, 0)), 581342);
    ASSERT_EQ(std::get<ID>(llet->node_at(LEFT, -1)), 581380);

    llet = map.lanelet_by_id(22059);
    ASSERT_EQ(std::get<ID>(llet->node_at(LEFT, 0)), 581380);
    ASSERT_EQ(std::get<ID>(llet->node_at(LEFT, -1)), 581393);
}



TEST_F(LaneletTest, AttributeStuffWorks)
{
    LaneletMap map(map_fn);

    lanelet_ptr_t the_ll = map.lanelet_by_id(_id);

    ASSERT_EQ(the_ll->attributes().size(), 3);

    ASSERT_EQ(the_ll->attribute("type").as_string(), "lanelet");
    ASSERT_DOUBLE_EQ(the_ll->attribute("speedlimit").as_double(), 40);

    ASSERT_THROW(the_ll->attribute("type").as_double(), std::runtime_error);

}

TEST_F(LaneletTest, RegulatoryElementsAreParsedCorrectly)
{
    using namespace LLet;
    LaneletMap map( map_fn );
    lanelet_ptr_t the_ll = map.lanelet_by_id(22061);
    ASSERT_EQ( the_ll->regulatory_elements().size(), 1 );
    regulatory_element_ptr_t the_element = the_ll->regulatory_elements().front();

    // now: use the element and check its properties.
    ASSERT_EQ( the_element->attribute("maneuver").as_string(), "traffic_light" );

    auto all_refs = the_element->members("ref");
    ASSERT_EQ(all_refs.size(), 2); // should be two signal lights

    for( const auto& ref: all_refs )
    {
        ASSERT_NO_THROW(boost::get< point_with_id_t >(ref));
        ASSERT_ANY_THROW(boost::get< lanelet_ptr_t >(ref));
        ASSERT_ANY_THROW(boost::get< strip_ptr_t >(ref));
    }

    auto all_lines = the_element->members("stop_line");
    ASSERT_EQ(all_lines.size(), 1);

    const auto& the_line = all_lines.front();

    ASSERT_ANY_THROW(boost::get<lanelet_ptr_t>(the_line));
    ASSERT_NO_THROW(boost::get<strip_ptr_t>(the_line));

}

TEST_F(LaneletTest, GraphSetupWorks)
{
    using namespace LLet;
    LaneletMap map( map_fn );

    ASSERT_EQ(boost::num_vertices(map.graph()), 6);
    ASSERT_EQ(boost::num_edges(map.graph()), 5);

}

TEST_F(LaneletTest, RoutingWorks)
{
    using namespace LLet;
    LaneletMap map( map_fn );

    auto start = map.lanelet_by_id(22063);
    auto dest = map.lanelet_by_id(22061);

    std::vector< lanelet_ptr_t > shortest_path;
    ASSERT_NO_THROW(shortest_path = map.shortest_path(start, dest) );
    ASSERT_THROW(shortest_path = map.shortest_path(start, map.lanelet_by_id(22060)), LLet::NoPath);
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

