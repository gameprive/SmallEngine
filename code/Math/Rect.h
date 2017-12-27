#pragma once

struct Rect
{
	explicit Rect(float x = 0, float y = 0, float width = 0, float height = 0) :
		x(x),
		y(y),
		width(width),
		height(height)
	{
	}

	void Set(float nx, float ny, float nwidth, float nheight)
	{
		x = nx;
		y = ny;
		width = nwidth;
		height = nheight;
	}

	bool operator ==(const Rect &r) const;
	bool operator !=(const Rect &r) const;

	float x;
	float y;
	float width;
	float height;
};