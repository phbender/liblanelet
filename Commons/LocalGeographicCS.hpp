// File:           LocalGeographicCS.hpp
// Creation Date:  Tuesday, March  6 2012
// Author:         Julius Ziegler <ziegler@mrt.uka.de>

#if !defined(LOCALGEOGRAPHICCS_HPP)
#define LOCALGEOGRAPHICCS_HPP

#include <MRT/Commons/convert_coordinates.hpp>

#include <utility>

struct LocalGeographicCS
{
  LocalGeographicCS();
  LocalGeographicCS( double lat0, double lon0 );

  void set_origin( double lat0, double lon0 );

  void ll2xy( double lat, double lon, double& x, double& y ) const;
  void xy2ll( double x, double y, double& lat, double& lon ) const; 

  std::pair<double, double> ll2xy( double lat, double lon ) const;
  std::pair<double, double> xy2ll( double x, double y ) const;

  // operate on containers
  template<class ItIn, class ItOut>
  void ll2xy( const ItIn& lat_begin, const ItIn& lat_end, const ItIn& lon_begin, const ItOut& x_begin, const ItOut& y_begin ) const;

  template<class ItIn, class ItOut>
  void xy2ll( const ItIn& x_begin, const ItIn& x_end, const ItIn& y_begin, const ItOut& lat_begin, const ItOut& lon_begin ) const;

private:
  double _scale;
  double _x0, _y0;
};

inline LocalGeographicCS::LocalGeographicCS( double lat0, double lon0 )
{
  set_origin( lat0, lon0 );
}

inline LocalGeographicCS::LocalGeographicCS()
{}

inline void LocalGeographicCS::set_origin( double lat0, double lon0 )
{
  _scale = convert_coordinates::lat_to_scale( lat0 );
  convert_coordinates::latlon_to_mercator( lat0, lon0, _scale, _x0, _y0 );
}

inline void LocalGeographicCS::ll2xy( double lat, double lon, double& x, double& y ) const
{
  convert_coordinates::latlon_to_mercator( lat, lon, _scale, x, y );
  x -= _x0;
  y -= _y0;
}

inline std::pair<double, double> LocalGeographicCS::ll2xy( double lat, double lon ) const
{
  double x, y;
  ll2xy( lat, lon, x, y );
  return std::make_pair( x, y );
}

inline void LocalGeographicCS::xy2ll( double x, double y, double& lat, double& lon ) const
{
  x += _x0;
  y += _y0;

  convert_coordinates::mercator_to_latlon( x, y, _scale, lat, lon );
}

inline std::pair<double, double> LocalGeographicCS::xy2ll( double x, double y ) const
{
  double lat, lon;
  xy2ll( x, y, lat, lon );
  return std::make_pair( lat, lon );
}

// operate on containers
template<class ItIn, class ItOut>
void LocalGeographicCS::ll2xy( const ItIn& lat_begin, const ItIn& lat_end, const ItIn& lon_begin, const ItOut& x_begin, const ItOut& y_begin ) const
{
  ItIn lat = lat_begin;
  ItIn lon = lon_begin;
  ItOut x = x_begin;
  ItOut y = y_begin;

  for( ; lat != lat_end; lat++, lon++, x++, y++ )
    {
      ll2xy( *lat, *lon, *x, *y );
    }
}

template<class ItIn, class ItOut>
void LocalGeographicCS::xy2ll( const ItIn& x_begin, const ItIn& x_end, const ItIn& y_begin, const ItOut& lat_begin, const ItOut& lon_begin ) const
{
  ItIn x = x_begin;
  ItIn y = y_begin;

  ItOut lat = lat_begin;
  ItOut lon = lon_begin;

  for( ; x != x_end; lat++, lon++, x++, y++ )
    xy2ll( *x, *y, *lat, *lon );
}

#endif
