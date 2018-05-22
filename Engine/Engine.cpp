#include "MainWindow.h"
#include "Engine.h"
#include "String.h"
#include "Colors.h"
#include "Math.h"


Engine::Engine( MainWindow& wnd ) :	wnd( wnd )
{
	gfx = new Graphics(wnd);

	test = new Surface("images/background.bmp");
	
	pixelZoom = graphScale / windowSize.x;
}

Engine::~Engine()
{
}

void Engine::Go()
{
	gfx->BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx->EndFrame();
}

void Engine::UpdateModel()
{
	if( wnd.kbd.KeyIsPressed( VK_LEFT ) )
	{
		iterations -= 10;
	}
	if( wnd.kbd.KeyIsPressed( VK_RIGHT ) )
	{
		iterations += 10;
	}
	if( wnd.kbd.KeyIsPressed( 'A' ) )
	{
		cLock = true;
		drawColor = Colors::Black;
	}
	if( wnd.kbd.KeyIsPressed( 'S' ) )
	{
		cLock = false;
		drawColor = Colors::Red;
		iterations = 100;
	}
	if( wnd.kbd.KeyIsPressed( 'Z' ) )
	{
		//zoom in
		zoomCamera(1.1);
	}
	if( wnd.kbd.KeyIsPressed( 'X' ) )
	{
		//zoom out
		zoomCamera(1/1.1);
	}

	if (!cLock)
	{
		cReal = wnd.mouse.GetPosX() * pixelZoom + graphXMin;
		cImaginary = -(wnd.mouse.GetPosY() * pixelZoom + graphYMin);
	}
	else if( iterations < 1000)
	{
		iterations *= 2;
	}
}

void Engine::ComposeFrame()
{
	gfx->DrawSpriteNonChroma(0, 0, *test);

	drawSets();
	//drawCircles();
}

void Engine::drawSets()
{
	Vei2 point(0, 0);

	for (int i = 0; i < windowSize.x; i++)
	{
		for (int j = 0; j < windowSize.y; j++)
		{
			point.x = i;
			point.y = j;

			double real = getPixelXValue(point.x);
			double imaginary = getPixelYValue(point.y);

			int result = mandelbrotFunction(real, imaginary);
			//int result = mandelbrotFunctionX2(real, imaginary);
			//int result = juliaFunction(real, imaginary, cReal, cImaginary);

			if (result > -1)
				putPixel(point, result);
		}
	}
}

void Engine::drawCircles()
{
	currentJuliaCircles.clear();
	currentJuliaCircles.push_back(JuliaCircle(0, 0, sqrt((cReal * cReal) + (cImaginary * cImaginary)) / 2));
	juliaCircleFunction(cReal, cImaginary);

	for (int i = 0; i < currentJuliaCircles.size(); i++)
		drawCircle(currentJuliaCircles.at(i));
}

void Engine::drawCircle(JuliaCircle circle)
{
	for (int i = 0; i < windowSize.x; i++)
	{
		for (int j = 0; j < windowSize.y; j++)
		{
			if (circle.contains(getPixelXValue(i), getPixelYValue(j)))
				putPixel({ i,j }, 6581375);
		}
	}
}

void Engine::putPixel(Vei2 pixel, int colorOption)
{
	if (pixel.x < 0)
		pixel.x = 0;
	else if (pixel.x >= 400)
		pixel.x = 399;

	if (pixel.y < 1)
		pixel.y = 1;
	else if (pixel.y > 400)
		pixel.y = 400;

	//static constexpr Color White = MakeRGB( 255u,255u,255u );
	//static constexpr Color Black = MakeRGB( 0u,0u,0u );

	//int a = (float)255 / iterations * colorOption;
	int a = colorOption;
	
	Color asasdfasdf = ( 255u,255u,255u );
	//Color drawColor = ( a + 'u',255u,255u );

	gfx->PutPixel(pixel.x, windowSize.y - pixel.y, Color(colorOption));
}

double Engine::getPixelXValue(int screenCoordinate)
{
	return graphXMin + (screenCoordinate * pixelZoom);
}

double Engine::getPixelYValue(int screenCoordinate)
{
	return graphYMin + (screenCoordinate * pixelZoom);
}

int Engine::mandelbrotFunction(double zX, double zY, double cX, double cY, int n)
{
	double cXIterating = cX * cX - cY * cY + zX;
	double cYIterating = cX * cY * 2 + zY;
	
	if (cXIterating * cXIterating + cYIterating * cYIterating > 4)
		return 16581375 - (15000000 / iterations * n);

	if (n > iterations)
		return 0;

	return mandelbrotFunction(zX, zY, cXIterating, cYIterating, n + 1);
}

int Engine::mandelbrotFunctionX2(double x, double y, int n)
{
	double xIterating = (y*y*y*y)
		+ (x*x)
		+ (x*x*x*x)
		+ (x)
		+ (2 * x*x*x)
		- (6 * x*y*y)
		- (6 * x*x*y*y)
		- (y*y);
	double yIterating = (y)
		+ (2 * x*y)
		+ (4 * x*x*x*x*y)
		+ (6 * x*x*y)
		- (4 * x*y*y*y)
		- (2 * y*y*y);

	if (xIterating * xIterating + yIterating * yIterating > 16)
		return 16581375 - (10000000 / (n + 2));

	if (n > iterations)
		return 0;

	return mandelbrotFunctionX2(xIterating, yIterating, n + 1);
}

int Engine::juliaFunction(double zX, double zY, double cX, double cY, int n)
{
	double zXIterating = zX * zX - zY * zY + cX;
	double zYIterating = zX * zY * 2 + cY;
	
	if (zXIterating * zXIterating + zYIterating * zYIterating > 4)
		return 16581375 - (13000000 / (n * n + 1));// +(3000 / (zX * zY));

	if (n > iterations)
		return 0;

	return juliaFunction(zXIterating, zYIterating, cX, cY, n + 1);
}

int Engine::juliaCircleFunction(double cX, double cY, double zX, double zY, int n)
{
	double zXIterating = zX * zX - zY * zY + cX;
	double zYIterating = zX * zY * 2 + cY;

	//if any previous point contains this location
	//return 16581375;
	for (int i = 0; i < currentJuliaCircles.size(); i++)
		if (currentJuliaCircles.at(i).contains(zXIterating, zYIterating))
			return 16581375 - (13000000 / (n + 1));

	if (n > iterations)
		return 0;
	
	//radius = smallest(distance - radius) of the previous points.
	double radius = currentJuliaCircles.at(0).getRadiusDistance(zXIterating, zYIterating);

	for (int i = 0; i < currentJuliaCircles.size(); i++)
	{
		JuliaCircle juliaCircle = currentJuliaCircles.at(i);

		if (juliaCircle.getRadiusDistance(zXIterating, zYIterating) < radius)
			radius = juliaCircle.getRadiusDistance(zXIterating, zYIterating) - pixelZoom;
	}

	currentJuliaCircles.push_back(JuliaCircle(zXIterating, zYIterating, radius));

	return juliaCircleFunction(cX, cY, zXIterating, zYIterating, n + 1);
}

void Engine::zoomCamera(float zoom)
{
	iterations = 50;

	double lockXPercent = (double)wnd.mouse.GetPosX() / windowSize.x;
	double lockYPercent = 1 - (double)wnd.mouse.GetPosY() / windowSize.y;

	double lockX = lockXPercent * graphScale + graphXMin;
	double lockY = lockYPercent * graphScale + graphYMin;

	cameraZoom *= zoom;
	
	graphScale = 4 / cameraZoom;
	
	graphXMin = lockX - graphScale * lockXPercent;
	graphYMin = lockY - graphScale * lockYPercent;
	
	pixelZoom = graphScale / windowSize.x;
}
