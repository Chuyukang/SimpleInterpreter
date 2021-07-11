#include "Win32Painter.h"
#include <iostream>
#include <cmath>

using namespace SimpleInterpreter;

Win32Painter::Win32Painter(HDC h)
{
	hdc = h;
	origin_x = 500;
	origin_y = 400;
	red = 0; green = 0; blue = 0;
	rot = 0;
	scale_x = 1; scale_y = 1;
}
void Win32Painter::setOrigin(double x,double y)
{
	origin_x = x; origin_y = y;
}
void Win32Painter::setScale(double x, double y)
{
	scale_x = x; scale_y = y;
}

void Win32Painter::setRot(double angle)
{
	rot = angle;
}
void Win32Painter::setColor(unsigned red, unsigned green, unsigned blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}
void Win32Painter::setColor(double red, double green, double blue)
{
	this->red = static_cast<unsigned>(red);
	this->green = static_cast<unsigned>(green);
	this->blue = static_cast<unsigned>(blue);
}

void Win32Painter::draw(double x, double y)
{
	COLORREF color = RGB(red, green, blue);
	std::cout << "Point logic:" << x << "," << y <<'\n';
	//伸缩变换
	x = x * scale_x; y = y * scale_y;
	//旋转变换
	double rot_x = x * cos(rot) - y * sin(rot);
	double rot_y = y * cos(rot) + x * sin(rot);
	std::cout << "Point after rot:" << x << "," << y <<'\n';
	//平移坐标系
	int re_x = static_cast<int>(origin_x + rot_x);
	int re_y = static_cast<int>(origin_y - rot_y);//坐标变换
	SetPixel(hdc, re_x, re_y, color);
	std::cout << "Point real:" << re_x << "," << re_y <<'\n';
	SetPixel(hdc, re_x+1, re_y, color);
	SetPixel(hdc, re_x, re_y+1, color);
	SetPixel(hdc, re_x+1, re_y+1, color);
}

