#include <polygon_collider/system/math.h>
#include <math.h>

bool math_is_equal(double left, double right)
{
    const double epsilon = 0.0000001;

    if(left == right)
        return true;
    return fabs(left - right) / (left + right) < epsilon;
}

bool math_is_less(double left, double right)
{
    return !math_is_equal(left, right) && left < right;
}
