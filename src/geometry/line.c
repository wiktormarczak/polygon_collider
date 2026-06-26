/* Copyright 2026 Wiktor Marczak
 * This file is part of Polygon Collider. */

/* Polygon Collider is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version. */

/* Polygon Collider is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. */

/* You should have received a copy of the GNU General Public License
 * along with Polygon Collider. If not, see <https://www.gnu.org/licenses/>. */

#include <polygon_collider/geometry/line.h>
#include <polygon_collider/geometry/vector.h>

Line line_get(double slope, double intercept)
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

double line_get_segment_angular_mass(Line line, double left, double right)
{
    double parameter[5];
    parameter[0] = 0.0;
    parameter[1] = line.intercept * line.intercept * line.intercept / 3.0;
    parameter[2] = line.slope * line.intercept * line.intercept;
    parameter[3] = line.slope * line.slope * line.intercept + line.intercept;
    parameter[4] = line.slope * line.slope * line.slope / 3.0 + line.slope;

    double left_power = 1.0;
    double right_power = 1.0;
    double angular_mass = 0.0;
    for(unsigned int i = 1; i <= 4; i++)
    {
        left_power *= left;
        right_power *= right;

        double delta = parameter[i] * (right_power - left_power) / i;
        angular_mass += delta;
    }

    return angular_mass;
}
