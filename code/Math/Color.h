#pragma once

struct Color
{
	static Color White() { return Color(1, 1, 1, 1); }
	static Color Black() { return Color(0, 0, 0, 1); }
	static Color Red() { return Color(1, 0, 0, 1); }
	static Color Green() { return Color(0, 1, 0, 1); }
	static Color Blue() { return Color(0, 0, 1, 1); }
	static Color Lerp(const Color &from, const Color &to, float t, bool clamp_01 = true);

	Color(float r = 1, float g = 1, float b = 1, float a = 1);
	bool operator ==(const Color &c) const;
	bool operator !=(const Color &c) const;
	Color operator *(const Color &c) const;
	Color &operator *=(const Color &c);
	Color operator *(float v) const;
	Color operator /(float v) const;

	float r;
	float g;
	float b;
	float a;
};