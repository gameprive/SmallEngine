#pragma once

class Test2
{
public:
	Test2(Window *window);

	void Update(float dt);
	void Render();
	void Resize(int width, int height);

	bool IsGameDone();
};