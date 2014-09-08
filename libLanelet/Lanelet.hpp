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
#include "Attribute.hpp"
#include "LineStrip.hpp"
#include "lanelet_point.hpp"
#include "RegulatoryElement.hpp"
#include "LaneletFwd.hpp"

#include <tuple>
#include <vector>

namespace LLet
{

class Lanelet : public LaneletBase, public HasAttributes
{
public:
    Lanelet( );
    Lanelet( int32_t id, const strip_ptr_t& left, const strip_ptr_t& right );


    virtual const std::tuple< strip_ptr_t, strip_ptr_t >& bounds() const;
    virtual const std::vector< regulatory_element_ptr_t >& regulatory_elements() const;
    std::vector< regulatory_element_ptr_t >& regulatory_elements();

    int32_t id() const;

    void add_regulatory_element( const regulatory_element_ptr_t& elem );

private:    
    int32_t _id;    
    std::tuple< strip_ptr_t, strip_ptr_t > _bounds;
    std::vector< regulatory_element_ptr_t > _regulatory_elements;

};

}

std::ostream& operator<<( std::ostream& out, const LLet::lanelet_ptr_t& lanelet );
