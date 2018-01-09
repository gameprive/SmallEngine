#pragma once

class IApp
{
public:
	virtual ~IApp() = default;

	virtual bool Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
};