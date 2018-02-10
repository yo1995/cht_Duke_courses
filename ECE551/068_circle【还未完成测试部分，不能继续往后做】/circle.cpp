#include "circle.h"
#include <math.h>
#include <stdlib.h>


Circle::Circle(Point p_center, double r_radius):center(p_center), radius(r_radius) {}

void Circle::move (double dx, double dy){
	center.move(dx, dy);
}

//should equal to the sum of two circular segments
//also equal to the difference between sum of two Circular sectors and the diamond of two centers and the intersections
double Circle::intersectionArea(const Circle & otherCircle){
	double d_between_centers = center.distanceFrom(otherCircle.center);
	double r1 = radius; //cannot write as Circle.radius
	double r2 = otherCircle.radius;
	double d_r1_r2; //for calculate area
	double s_area;
	//no intersectionArea
	if (d_between_centers >= r1 + r2) {
		return 0; 
	}
	//Concentric circles
	else if (d_between_centers <= abs(r1 - r2)) {
		if (r1 > r2) {
			return M_PI * r2 * r2;
		}
		else {
			return M_PI * r1 * r1;
		}
	}
	//Other normal circumstances
	else {
		//calculate the area of diamond with Heron's formula, pretty fancy!
		d_r1_r2 = (d_between_centers + r1 + r2) / 2;
        s_area = 2 * sqrt(d_r1_r2 * (d_r1_r2 - d_between_centers) * (d_r1_r2 - r1) * (d_r1_r2 - r2));
		//the return answer lacks readability...just used the Law of cosines
		//and do not forget that the angle is 1/2 of the arc angle!
		return acos((r1*r1+d_between_centers*d_between_centers-r2*r2)/(2*r1*d_between_centers))*r1*r1 \
		+ acos((r2*r2+d_between_centers*d_between_centers-r1*r1)/(2*r2*d_between_centers))*r2*r2 - s_area;
	}
}