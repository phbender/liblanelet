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

#include "LaneletBase.hpp"
#include "LineStrip.hpp"

#include <tuple>

namespace LLet
{

class CompoundLanelet : public LaneletBase
{
public:
    CompoundLanelet( const std::vector< lanelet_base_ptr_t >& lanelets );
    virtual const std::vector< point_with_id_t >& nodes(SIDE bound) const;
    virtual const std::vector< regulatory_element_ptr_t >& regulatory_elements() const;
private:
    const std::vector< lanelet_base_ptr_t > _lanelets;
    std::tuple< strip_ptr_t, strip_ptr_t > bounds;
    std::vector< regulatory_element_ptr_t > _regulatory_elements;


};

}

