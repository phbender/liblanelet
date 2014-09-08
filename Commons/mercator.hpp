#pragma once

#include <math.h>

/*
 * Mercator transformation
 * accounts for the fact that the earth is not a sphere, but a spheroid
 *
 * Taken from
 * http://wiki.openstreetmap.org/wiki/Mercator#C_implementation
 *
 */
#define D_R (M_PI / 180.0)
#define R_D (180.0 / M_PI)
#define R_MAJOR 6378137.0
#define R_MINOR 6356752.3142
#define RATIO (R_MINOR/R_MAJOR)
#define ECCENT (sqrt(1.0 - (RATIO * RATIO)))
#define COM (0.5 * ECCENT)

namespace mercator
{

inline
static double deg_rad (double ang) {
    return ang * D_R;
}

inline
double merc_x (double lon) {
    return R_MAJOR * deg_rad (lon);
}

inline
double merc_y (double lat) {
    lat = fmin (89.5, fmax (lat, -89.5));
    double phi = deg_rad(lat);
    double sinphi = sin(phi);
    double con = ECCENT * sinphi;
    con = pow((1.0 - con) / (1.0 + con), COM);
    double ts = tan(0.5 * (M_PI * 0.5 - phi)) / con;
    return 0 - R_MAJOR * log(ts);
}

inline
static double rad_deg (double ang) {
    return ang * R_D;
}

inline
double merc_lon (double x) {
    return rad_deg(x) / R_MAJOR;
}

inline
double merc_lat (double y) {
    double ts = exp ( -y / R_MAJOR);
    double phi = M_PI_2 - 2 * atan(ts);
    double dphi = 1.0;
    int i;
    for (i = 0; fabs(dphi) > 0.000000001 && i < 15; i++) {
        double con = ECCENT * sin (phi);
        dphi = M_PI_2 - 2 * atan (ts * pow((1.0 - con) / (1.0 + con), COM)) - phi;
        phi += dphi;
    }
    return rad_deg (phi);
}

}
