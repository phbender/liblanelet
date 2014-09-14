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

#include <boost/math/special_functions.hpp>
#include <cmath>
#include <MRT/Commons/LocalGeographicCS.hpp>
#include <MRT/Commons/normalize_angle.hpp>

namespace LLet
{

enum LATLON_COORDS
{
    LAT = 0,
    LON = 1,
    ID = 2
};

enum XY_COORDS
{
    X = 0,
    Y = 1
};

typedef std::tuple< double, double, int32_t > point_with_id_t;

inline
std::pair< double, double > vec( const point_with_id_t& a, const point_with_id_t& b )
{
    using std::get;

    LocalGeographicCS cs(get<LAT>(a), get<LON>(a));

    double ax, ay, bx, by;
    std::tie(ax, ay) = cs.ll2xy(get<LAT>(a), get<LON>(a));
    std::tie(bx, by) = cs.ll2xy(get<LAT>(b), get<LON>(b));

    double dx = bx -  ax;
    double dy = by -  ay;

    return std::make_pair(dx, dy);
}

inline
double abs( const std::pair< double, double > v )
{
    using std::get;
    using boost::math::hypot;
    return hypot( get<X>(v), get<Y>(v) );
}

inline
double dist( const point_with_id_t& a, const point_with_id_t& b )
{
    return abs(vec(a, b));
}

inline
double scalar_product( const std::pair< double, double >& a, const std::pair< double, double >& b )
{
    using std::get;
    return get<X>(a) * get<X>(b) + get<Y>(a) * get<Y>(b);
}

inline
double angle( const std::pair< double, double >& a, const std::pair< double, double >& b )
{
    using std::get;

    double sp = scalar_product(a, b);
    double cos_phi = sp / (abs(a) * abs(b));

    // sign for angle: test cross product
    double crossp_z = get<X>(a) * get<Y>(b) - get<Y>(a) * get<X>(b);
    double signum = boost::math::sign(crossp_z);
    double phi = normalize_angle(signum * std::acos(cos_phi));
    return phi;
}


template< typename T1, typename T2 >
inline
bool inrange(const T1& val, const T2& lo, const T2& hi)
{
    return val >= lo && val <= hi;
}

}
