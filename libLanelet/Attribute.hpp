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

#include <map>
#include <boost/lexical_cast.hpp>
#include <string>
#include <boost/format.hpp>

namespace LLet
{
struct AttributeValue
{
    AttributeValue( const std::string& v) : value(v)
    {

    }

    AttributeValue() = default;

    std::string value;

    operator double() const
    {
        return as_double();
    }

    operator std::string() const
    {
        return value;
    }

    std::string as_string() const
    {
        return value;
    }

    double as_double() const
    {
        try
        {
            return boost::lexical_cast< double >(value);
        }
        catch(...)
        {
            // TODO: maybe catch more selectively ...
            boost::format fmt("AttributeValue '%s' not convertible to double.");
            throw std::runtime_error((fmt % value).str());
        }
    }

};

typedef std::map< std::string, AttributeValue > AttributeMap;

class HasAttributes
{
public:
    const AttributeValue& attribute( std::string key ) const;
    AttributeMap& attributes();
    const AttributeMap& attributes() const;
private:
    AttributeMap _attributes;
};

}
