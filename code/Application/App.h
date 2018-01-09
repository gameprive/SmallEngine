#pragma once

class IApp : Noncopyable
{
public:
	virtual ~IApp() = default;

	virtual bool Init() = 0;

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

	virtual void Close() = 0;
};