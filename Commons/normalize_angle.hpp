#ifndef NORMALIZE_ANGLE_HPP
#define NORMALIZE_ANGLE_HPP

#include <cmath>

// modulu - similar to matlab's mod()
// result is always possitive. not similar to fmod()
// Mod(-3,4)= 1   fmod(-3,4)= -3
inline double float_modulo( double x, double y )
{
    if (0 == y)
        return x;

    return x - y * floor(x/y);
}

inline double normalize_angle( double a )
{
  return float_modulo( a + M_PI, 2*M_PI) - M_PI;  
}

#endif // NORMALIZE_ANGLE_HPP
