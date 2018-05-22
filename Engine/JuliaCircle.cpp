#include "JuliaCircle.h"
#include "Math.h"



JuliaCircle::JuliaCircle(double real, double imaginary, double radius)
	: real(real), imaginary(imaginary), radius(radius)
{
	//radiusSquared = radius * radius;
}

JuliaCircle::~JuliaCircle()
{
}

bool JuliaCircle::contains(double x, double y)
{
	if(getDistance(x, y) < radius)
		return true;

	return false;
}

double JuliaCircle::getRadiusDistance(double x, double y)
{
	if (contains(x, y))
		return 0;

	return getDistance(x, y) - radius;
}

//double JuliaCircle::getRadiusDistanceSquared(double x, double y)
//{
//	return getDistanceSquared(x, y) - radiusSquared;
//}

double JuliaCircle::getDistance(double x, double y)
{
	return sqrt((real - x) * (real - x) + (imaginary - y) * (imaginary - y));
}

//double JuliaCircle::getDistanceSquared(double x, double y)
//{
//	return (real - x) * (real - x) + (imaginary - y) * (imaginary - y);
//}
