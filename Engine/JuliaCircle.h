#pragma once
class JuliaCircle
{
public:
	JuliaCircle(double, double, double);
	~JuliaCircle();
	bool contains(double, double);
	double getRadiusDistance(double, double);
	//double getRadiusDistanceSquared(double, double);
	double getDistance(double, double);
	//double getDistanceSquared(double, double);
private:
	double real;
	double imaginary;
	double radius;
	//double radiusSquared;
};

