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

#include "RegulatoryElement.hpp"

using namespace LLet;

LLet::RegulatoryElement::RegulatoryElement(int32_t id) : _id(id)
{
}

int32_t RegulatoryElement::id() const
{
    return _id;
}

const std::vector< regulatory_element_member_t > &RegulatoryElement::members() const
{
    return _members;
}


std::vector< regulatory_element_member_t > &RegulatoryElement::members()
{
    return _members;
}

std::vector<member_variant_t> RegulatoryElement::members(std::string role) const
{
    std::vector< member_variant_t > result;
    for( const regulatory_element_member_t& m: members() )
    {
        if(m.first == role)
        {
            result.push_back( m.second );
        }
    }
    return result;
}
