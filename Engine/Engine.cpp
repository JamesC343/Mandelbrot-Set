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
		cReal = wnd.mouse.GetPosX() * 0.01 - 2;
		cImaginary = wnd.mouse.GetPosY() * 0.01 - 2;
	}
	else if( iterations < 1000)
	{
		iterations *= 2;
	}
}

void Engine::ComposeFrame()
{
	gfx->DrawSpriteNonChroma(0, 0, *test);
	
	Vei2 point(0, 0);

	for (int i = 0; i < windowSize.x; i++)
	{
		for (int j = 0; j < windowSize.y; j++)
		{
			point.x = i;
			point.y = j;

			int result = window(point);

			if (result > -1)
				putPixel(point, result);
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

int Engine::window(Vei2 screenCoordinate)
{
	double real = graphXMin + (screenCoordinate.x * pixelZoom);
	double imaginary = graphYMin + (screenCoordinate.y * pixelZoom);
	
	return mandelbrotFunction(real, imaginary);
	return juliaFunction(real, imaginary, cReal, cImaginary);
}

int Engine::mandelbrotFunction(double zX, double zY, double cX, double cY, int n)
{
	double cXIterating = cX * cX - cY * cY + zX;
	double cYIterating = cX * cY * 2 + zY;
	
	if (cXIterating * cXIterating + cYIterating * cYIterating > 4)
		return 16581375 - (10000000 / (n + 1));

	if (n > iterations)
		return 0;

	return mandelbrotFunction(zX, zY, cXIterating, cYIterating, n + 1);
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
