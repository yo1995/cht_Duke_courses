#include "point.h"
#ifndef __CIRCLE_H__
#define __CIRCLE_H__

class Circle {
	private:
		Point center;
		const double radius;
	public:
		Circle (Point p_center, double r_radius);
		void move (double dx, double dy);
		double intersectionArea(const Circle & otherCircle);
};

#endif