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

#include "Lanelet.hpp"

#include <boost/format.hpp>

#include <string>
#include <stdexcept>

using namespace LLet;

Lanelet::Lanelet()
{
}

Lanelet::Lanelet(int32_t id, const strip_ptr_t &left, const strip_ptr_t &right) :
    _id(id),
    _bounds(std::make_tuple(left, right))
{
}

const std::tuple<strip_ptr_t, strip_ptr_t>& Lanelet::bounds() const
{
    return _bounds;
}

const std::vector<regulatory_element_ptr_t> &Lanelet::regulatory_elements() const
{
    return _regulatory_elements;
}

std::vector<regulatory_element_ptr_t> &Lanelet::regulatory_elements()
{
    return _regulatory_elements;
}

int32_t Lanelet::id() const
{
    return _id;
}

void Lanelet::add_regulatory_element(const regulatory_element_ptr_t &elem)
{
    this->_regulatory_elements.push_back( elem );
}


std::ostream &operator<<(std::ostream &out, const lanelet_ptr_t &lanelet)
{
    boost::format fmt("<Lanelet %i>");
    out << (fmt % lanelet->id());
    return out;
}
