// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Rect.h"
#include "Mathf.h"

bool Rect::operator ==(const Rect& r) const
{
	return Mathf::FloatEqual(x, r.x) &&
		Mathf::FloatEqual(y, r.y) &&
		Mathf::FloatEqual(width, r.width) &&
		Mathf::FloatEqual(height, r.height);
}

bool Rect::operator !=(const Rect& r) const
{
	return !(*this == r);
}