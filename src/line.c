#include <polygon_collider/line.h>
#include <polygon_collider/vector.h>

Line line_get(float slope, float intercept)
{
    Line line;
    line.slope = slope;
    line.intercept = intercept;
    return line;
}

Line line_get_from_points(Vector left, Vector right)
{
    Line line;
    line.slope = (left.y - right.y) / (left.x - right.x);
    line.intercept = left.y - line.slope * left.x;
    return line;
}
