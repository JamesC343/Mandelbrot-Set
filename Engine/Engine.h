#pragma once

#include <vector>
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Colors.h"

class Engine
{
public:
	Engine( class MainWindow& wnd );
	~Engine();
	Engine( const Engine& ) = delete;
	Engine& operator=( const Engine& ) = delete;
	void Go();

private:
	void ComposeFrame();
	void UpdateModel();
	void putPixel(Vei2,int);
	int window(Vei2);
	int mandelbrotFunction(double,double,double=0,double=0,int=0);
	int juliaFunction(double,double,double,double,int=0);
	void zoomCamera(float);

private:
	MainWindow& wnd;
	Graphics* gfx;
	Surface* test;
	
	const Vei2 windowSize = { 400, 400 };
	
	double graphXMin = -2;
	double graphYMin = -2;
	double graphScale = 4;

	double pixelZoom = 0;
	double cameraZoom = 1;

	double cReal = 0;
	double cImaginary = 0;

	bool cLock = false;
	Color drawColor = Colors::Red;

	int iterations = 100;

};

