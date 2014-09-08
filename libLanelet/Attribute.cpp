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

#include "Attribute.hpp"

using namespace LLet;

const AttributeValue& HasAttributes::attribute(std::string key) const
{
    // TODO I think this will throw if key not present ... do we want this?
    return _attributes.at( key );
}

AttributeMap& HasAttributes::attributes()
{
    return _attributes;
}

const AttributeMap& HasAttributes::attributes() const
{
    return _attributes;
}

