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

#include <MRT/libLanelet/Attribute.hpp>
#include <MRT/libLanelet/lanelet_point.hpp>
#include <MRT/libLanelet/BoundingBox.hpp>
#include <vector>
#include <memory>
#include <tuple>

namespace LLet
{

class LineStrip
{

public:
    virtual const std::vector< point_with_id_t >& pts() const = 0;
    virtual BoundingBox bb() const;
};

typedef std::shared_ptr< LineStrip > strip_ptr_t;

class OSMLineStrip : public LineStrip
{
public:
    OSMLineStrip();
    AttributeMap _attributes;
    virtual const std::vector< point_with_id_t >& pts() const;
    std::vector< point_with_id_t > _pts;
private:

};

class CompoundLineStrip : public LineStrip
{
public:
    CompoundLineStrip( const std::vector< std::shared_ptr< LineStrip > > & strips );
    virtual const std::vector< point_with_id_t >& pts() const;

private:
    std::vector< point_with_id_t > _pts;

};

class ReversedLineStrip : public LineStrip
{
public:
    ReversedLineStrip( std::shared_ptr< LineStrip > parent );

    virtual const std::vector< point_with_id_t >& pts() const;

private:
    const std::shared_ptr< LineStrip > _parent;
    std::vector< point_with_id_t > _pts;


};

}
