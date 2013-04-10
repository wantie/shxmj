#pragma once
#include <windows.h>
namespace mj
{

template <class T>
T abs(T &x) 
{
	return x > 0 ? x : (0-x);
}

class point
{
public:
	point(int x =0, int y = 0) : x(x), y(y) {}
	int x;
	int y;
};

class shape
{
protected:
	HDC _hdc;
public:
	void set_hdc(HDC h) { _hdc = h; }
	virtual void draw() = 0;
};

class line : public shape
{
	point _pt_start;
	point _pt_end;
	COLORREF _color;
public:
	line(const point &ps = point() , const point &pe = point())
		: _pt_start(ps), _pt_end(pe), _color(RGB(0,0,0)) {};
	void set_start_point(const point &p) {_pt_start = p;};
	void set_end_point(const point &p) { _pt_end = p;}
	void draw()
	{/*
		// decide which pixel should be chosen which maps to (x,y)
		double dx = _pt_end.x - _pt_start.x;
		double dy = _pt_end.y - _pt_start.y;
		if (dx == 0) {
			for (double y = min(_pt_start.y, _pt_end.y); y < max(_pt_start.y, _pt_end.y); y++)
				SetPixel(_hdc, _pt_start.x, y, _color);
			return;
		}
		if (dy == 0) {
			for (double x = min(_pt_start.x, _pt_end.x); x < max(_pt_start.x, _pt_end.x); x++)
				SetPixel(_hdc, x, _pt_start.y, _color);
			return;
		}

		if (abs(dx) > abs(dy)) {
			for (double x = _pt_start.x; x != _pt_end.x; x += dx / abs(dx)) {
				//x - sx / y - sy = dx / dy , y = sy + (s-sx)*dy / dx
				double y = _pt_start.y + (x - _pt_start.x) * dy / dx;
				SetPixel(_hdc, x, y, _color);
			}
			
		} else {
			for (double y = _pt_start.y; y != _pt_end.y; y += dy / abs(dy)) {
				//x - sx / y - sy = dx / dy , y = sy + (s-sx)*dy / dx
				double x = _pt_start.x + (y - _pt_start.y) * dx / dy;
				SetPixel(_hdc, x, y, _color);
			}
		} 
		SetPixel(_hdc, _pt_end.x, _pt_end.y, _color);
		*/
	}
};

class rectangle : public shape
{
	point _pt_left_top;
	point _pt_right_bottom;
public:
	rectangle(const point &lt = point(), const point &rb = point())
		: _pt_left_top(lt), _pt_right_bottom(rb) {}
	void set_left_top_point(const point &p) { _pt_left_top = p; }
	void set_left_top_point(int x, int y) { set_left_top_point(point(x, y)); }
	void set_right_bottom_point(const point &p) { _pt_right_bottom = p; }
	void set_right_bottom_point(int x, int y) { set_right_bottom_point(point(x,y)); }
	void draw()
	{
		line left(_pt_left_top, point(_pt_left_top.x, _pt_right_bottom.y));
		line right(_pt_right_bottom, point(_pt_right_bottom.x, _pt_left_top.y));
		line bottom(point(_pt_left_top.x, _pt_right_bottom.y), _pt_right_bottom);
		line top(_pt_left_top, point(_pt_right_bottom.x, _pt_left_top.y));
		left.set_hdc(_hdc);
		right.set_hdc(_hdc);
		bottom.set_hdc(_hdc);
		top.set_hdc(_hdc);
		left.draw();
		right.draw();
		bottom.draw();
		top.draw();
	}
};

class bezier : public shape
{
	// when???
};

}
