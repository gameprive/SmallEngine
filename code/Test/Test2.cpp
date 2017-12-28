#include "stdafx.h"
#include "Test2.h"

#include "engine.h"

terrainwatersim

class Monkey : public Game
{
public:
	Monkey()
	{
		// We are going to use render, physics and input systems
		attach(new RenderSystem("monkey"));
		attach(new PhysicsSystem());
		attach(new InputSystem());

		// Set up the controllable player
		Entity *player = new Entity();
		player->attach(new Transform(0.0, 2.0, 0.0));
		player->attach(new PhysicsCharacter());
		player->attach(new InputController());
		player->attach(new Camera(CAMERA_DEFAULT));
		attach(player);

		// Set up a point light
		Entity *light = new Entity();
		light->attach(new Transform(1.0, 2.0, 0.0));
		light->attach(new Light(LIGHT_POINT, Color(255, 255, 255)));
		attach(light);

		// Set up a monkey
		Entity *monkey = new Entity();
		monkey->attach(new Transform(2.0, 1.0, 0.0));
		monkey->attach(MeshLoader::load("samples/monkey/monkey.obj", Color(200, 150, 50)));
		monkey->attach(new Physics(PHYSICS_DYNAMIC));
		monkey->attach(new Renderer());
		attach(monkey);

		// Set up the ground
		Entity *ground = new Entity();
		ground->attach(new Transform(0.0, 0.0, 0.0));
		ground->attach(ShapeGenerator::plane(Color(200, 50, 50), 50.0, 50.0));
		ground->attach(new Physics(PHYSICS_STATIC));
		ground->attach(new Renderer());
		attach(ground);
	}

	void update(double deltaTime)
	{

	}
};

//--------------------------------------------------------------------
Test2::Test2(Window *window)
{

}
//--------------------------------------------------------------------
Test2::~Test2()
{

}
//--------------------------------------------------------------------
void Test2::Update(float dt)
{

}
//--------------------------------------------------------------------
void Test2::Render()
{

}
//--------------------------------------------------------------------
void Test2::Resize(int width, int height)
{
}
//--------------------------------------------------------------------
bool Test2::IsGameDone()
{
	return false;
}
//--------------------------------------------------------------------