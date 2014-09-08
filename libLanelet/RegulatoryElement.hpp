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
#include <boost/variant.hpp>
#include <vector>
#include <string>
#include <memory>

#include "LaneletFwd.hpp"
#include "lanelet_point.hpp"
#include "LineStrip.hpp"
#include "Attribute.hpp"

namespace LLet
{

typedef boost::variant< lanelet_ptr_t, strip_ptr_t, point_with_id_t > member_variant_t;
typedef std::pair< std::string, member_variant_t > regulatory_element_member_t;

class RegulatoryElement;

typedef std::shared_ptr< RegulatoryElement > regulatory_element_ptr_t;

class RegulatoryElement : public HasAttributes
{
public:
    RegulatoryElement( int32_t id );
    int32_t id() const;
    const std::vector< regulatory_element_member_t >& members() const;
    std::vector< member_variant_t > members(std::string role) const;

    std::vector< regulatory_element_member_t >& members();

private:
    std::vector< regulatory_element_member_t > _members;
    const int32_t _id;
    AttributeMap _attributes;
};

}
