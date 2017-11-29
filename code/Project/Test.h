#pragma once

class Camera;
class TestModel;

class Test
{
public:
	Test(Window *window);
	~Test();

	void Update(float dt);
	void Render();
	void Resize(int width, int height);

	bool IsGameDone();

	Camera *m_camera = nullptr;
	TestModel *m_sign = nullptr;
};