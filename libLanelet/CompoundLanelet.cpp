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

#include "CompoundLanelet.hpp"

using namespace LLet;

CompoundLanelet::CompoundLanelet(const std::vector<lanelet_base_ptr_t> &lanelets) : _lanelets(lanelets)
{

    auto& left = std::get<0>(bounds);
    auto& right = std::get<1>(bounds);

    for( const auto& llet: lanelets )
    {
        const auto& reg_elem_to_insert = llet->regulatory_elements();
        _regulatory_elements.insert(_regulatory_elements.end(), reg_elem_to_insert.cbegin(), reg_elem_to_insert.cend() );
    }
}

const std::vector<point_with_id_t> &CompoundLanelet::nodes(SIDE bound) const
{
}

const std::vector<regulatory_element_ptr_t> &CompoundLanelet::regulatory_elements() const
{
    return _regulatory_elements;
}
