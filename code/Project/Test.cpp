#include "stdafx.h"
#include "Test.h"
#include "Camera.h"
#include "TestModel.h"

//-----------------------------------------------------------------------
static const glm::vec3 STARTING_PLAYER_POS(0.0, 0.5, 5.0);
//-----------------------------------------------------------------------
Test::Test(Window *window)
{
	m_camera = new Camera(window, STARTING_PLAYER_POS);
	m_camera->ResizeProjection(window->GetWidth(), window->GetHeight());
	m_sign = new TestModel(glm::vec3(0.0), 0.0f);
}
//-----------------------------------------------------------------------
Test::~Test()
{
	delete m_camera;
	delete m_sign;
}
//-----------------------------------------------------------------------
void Test::Update(float dt)
{
	m_camera->Update(dt);
}
//-----------------------------------------------------------------------
void Test::Render()
{
	m_sign->Render(m_camera->GetPerspectiveProjection(), m_camera->GetViewMatrix());
}
//-----------------------------------------------------------------------
void Test::Resize(int width, int height)
{
	m_camera->ResizeProjection(width, height);
}
//-----------------------------------------------------------------------
bool Test::IsGameDone()
{
	return false;
}
//-----------------------------------------------------------------------