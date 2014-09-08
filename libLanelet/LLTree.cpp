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

#include "LLTree.hpp"

#include <algorithm>

using namespace LLet;

void LLTree::insert(const lanelet_ptr_t& obj)
{
    int32_t index = _lanelets.size();
    _lanelets.push_back( obj );
    const auto bb = obj->bb();
    double min_data[2] = {bb.get<BoundingBox::SOUTH>(), bb.get<BoundingBox::WEST>()};
    double max_data[2] = {bb.get<BoundingBox::NORTH>(), bb.get<BoundingBox::EAST>()};
    _tree.Insert(min_data, max_data, index);
}

namespace
{

    bool callback(int32_t index, void* data )
    {
        auto indices = static_cast< std::vector< int32_t >* >(data);
        indices->push_back(index);
        return true;
    }

}

std::vector< lanelet_ptr_t > LLTree::query( const BoundingBox& bb)
{
    double min_data[2] = {bb.get<BoundingBox::SOUTH>(), bb.get<BoundingBox::WEST>()};
    double max_data[2] = {bb.get<BoundingBox::NORTH>(), bb.get<BoundingBox::EAST>()};

    std::vector< int32_t > indices;
    _tree.Search(min_data, max_data, callback, &indices);

    std::vector< lanelet_ptr_t > result(indices.size());

    std::transform(indices.begin(), indices.end(), result.begin(), [this](int32_t index){ return _lanelets[index];});

    return result;

}
