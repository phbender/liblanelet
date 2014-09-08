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

#include "LineStrip.hpp"
#include <deque>
#include <algorithm>
#include <stdexcept>
#include <cassert>

#include <boost/format.hpp>

using namespace LLet;

BoundingBox LineStrip::bb() const
{
    const auto& _pts = this->pts();
    BoundingBox b(_pts.front());

    for( const auto& p: _pts )
    {
        b.extend_point(p);
    }

    return b;
}

ReversedLineStrip::ReversedLineStrip(std::shared_ptr< LineStrip > parent) : _parent(parent)
{
    const auto& ins = _parent->pts();
    _pts.insert( _pts.end(), ins.rbegin(), ins.rend() );
}

OSMLineStrip::OSMLineStrip()
{
}

const std::vector<point_with_id_t> &ReversedLineStrip::pts() const
{
    return this->_pts;
}

const std::vector<point_with_id_t> &OSMLineStrip::pts() const
{
    return this->_pts;
}

const std::vector<point_with_id_t> &CompoundLineStrip::pts() const
{
    return this->_pts;
}

CompoundLineStrip::CompoundLineStrip(const std::vector<std::shared_ptr<LineStrip> > &strips)
{
    assert( strips.size() );

    std::deque< std::shared_ptr<LineStrip> > open(strips.cbegin(), strips.cend());

    const auto& ins = open.front()->pts();

    _pts.insert(_pts.end(), ins.cbegin(), ins.cend());
    open.pop_front();

    auto insert_points = [this]( const std::shared_ptr< LineStrip > s) -> bool
    {
        const auto& ins = s->pts();
        if( std::get<ID>(this->_pts.back()) == std::get<ID>(s->pts().front()))
        {
            this->_pts.insert(this->_pts.end(), ins.cbegin(), ins.cend());
            return true;
        }
        else if( std::get<ID>(this->_pts.front()) == std::get<ID>(s->pts().back()))
        {
            this->_pts.insert(this->_pts.begin(), ins.cbegin(), ins.cend());
            return true;
        }
        else
            return false;
    };

    std::shared_ptr< LineStrip > last_nomatch;

    while( open.size() )
    {
        auto strip = open.front();
        open.pop_front();

        if( strip == last_nomatch )
        {
            throw std::runtime_error("there seems so be an error in a line strip group: cannot concatenate all.");
        }

        ReversedLineStrip* rev = new ReversedLineStrip(strip);
        std::shared_ptr< LineStrip > reversed(rev);

        if( insert_points(strip) || insert_points(  reversed ) )
            ;
        else
        {
            open.push_back(strip);
            last_nomatch = strip;
        }
    }

    auto new_end = std::unique(_pts.begin(), _pts.end());
    _pts.erase(new_end, _pts.end());
}


