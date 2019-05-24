#include "point.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void Point::move(double dx, double dy){
	x = x + dx;
	y = y + dy;
}

//calculate the distance between two points
double Point::distanceFrom(const Point & p){
	return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
}

double Point::getX() const{
	return x;
}

double Point::getY() const{
	return y;
}