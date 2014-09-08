// Author:         Andreas Geiger <geiger@kit.edu>

#if !defined(CONVERT_COORDINATES_HPP)
#define CONVERT_COORDINATES_HPP

#include <cmath>

/*!
  * \file convert_coordinates.hpp
  *
  * \brief provides functions to convert global lat/long into local cartesian x/y coordinates
  *
  * the following functions map lat/long coordinates to the euclidean mercator coordinate system mx/my
  * lat/long are always specified in degrees (DEG)
  * mx is the distance from the central meridian towards EAST
  * my is the distance from the equator towards NORTH
  * mercator cordinates mx/my correspond to metric coordinates x/y ONLY at the equator (scale=1)
  * at other latitudes a scale factor can be used to get metric relations
  * for more details see GCDC/docs/coordinate_systems.pdf
  * \note that in GCDC x is towards NORTH, y towards WEST!!!
  */

namespace convert_coordinates {

  const double EARTH_RADIUS_EQUA = 6378137.0;// earth radius at equator [m]

  // inlined functions to avoid multiple definitions

  /*! \brief convert latitude to scale, which is needed by mercator transformations
   *  \param lat latitude in degrees (DEG)
   *  \return scale factor
   *  \note when converting from lat/lon -> mercator and back again,
   *        or vice versa, use the same scale in both transformations!
   */
  inline double lat_to_scale (double lat) {
    return cos(lat * M_PI / 180.0);
  }

  /*! \brief converts lat/lon/scale to mx/my (mx/my in meters if correct scale is given)
	 */
  template<class float_type>
  inline void latlon_to_mercator (double lat, double lon, double scale, float_type &mx, float_type &my) {
    mx = scale * lon * M_PI * EARTH_RADIUS_EQUA / 180.0;
    my = scale * EARTH_RADIUS_EQUA * log( tan((90.0+lat) * M_PI / 360.0) );
  }

  /*! \brief convenience function, uses lat0 to calculate appropriate scale
	 */
  inline void latlon_to_scaled_mercator (double lat, double lon, double lat0, double &mx, double &my) {
    double scale = lat_to_scale( lat0 );
    mx = scale * lon * M_PI * EARTH_RADIUS_EQUA / 180.0;
    my = scale * EARTH_RADIUS_EQUA * log( tan((90.0+lat) * M_PI / 360.0) );
  }

  /*! \brief converts mx/my/scale to lat/lon (mx/my in meters if correct scale is given)
	 */
  inline void mercator_to_latlon (double mx, double my, double scale, double &lat, double &lon) {
    lon = mx * 180.0 / (M_PI * EARTH_RADIUS_EQUA * scale);
    lat = 360.0 * atan( exp(my/(EARTH_RADIUS_EQUA * scale)) ) / M_PI - 90.0;
	}
	
  /*! \brief convenience function, uses lat0 to calculate appropriate scale
   */
  inline void scaled_mercator_to_latlon (double mx, double my, double lat0, double &lat, double &lon) {
    double scale = lat_to_scale( lat0 );
    lon = mx * 180.0 / (M_PI * EARTH_RADIUS_EQUA * scale);
    lat = 360.0 * atan( exp(my/(EARTH_RADIUS_EQUA * scale)) ) / M_PI - 90.0;
  }

  /*! \brief adds meters dx/dy to given lat/lon and returns new lat/lon
   */
  inline void latlon_add_meters (double lat_start, double lon_start, double dx, double dy, double &lat_end, double &lon_end) {
    double scale = lat_to_scale (lat_start);
    double mx,my;
    latlon_to_mercator (lat_start, lon_start, scale, mx, my);
    mx += dx;
    my += dy;
    mercator_to_latlon (mx, my, scale, lat_end, lon_end);
  }

  /*! \brief given two lat/lon coordinates, returns their difference in meters dx/dy
   */
  inline void latlon_diff_to_meters (double lat_start, double lon_start, double lat_end, double lon_end, double &dx, double &dy) {
    double scale = lat_to_scale (lat_start);
    double mx1,my1, mx2, my2;
    latlon_to_mercator (lat_start, lon_start, scale, mx1, my1);
    latlon_to_mercator (lat_end, lon_end, scale, mx2, my2);
    dx = mx2-mx1;
    dy = my2-my1;
  }

};

#endif

