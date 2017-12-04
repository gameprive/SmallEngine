#pragma once

struct ClearFlags
{
	enum
	{
		Color = (1 << 0),
		Depth = (1 << 1),
		Stencil = (1 << 2),  
		
		ColorDepth = (Color | Depth),
		DepthStencil = (Depth | Stencil),
		All = (Color | Depth | Stencil),
	};
};
