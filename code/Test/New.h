#pragma once

class New final : public IApp
{
public:
	bool Init() final;

	void Update(float dt) final;
	void Render() final;

	void Close() final;

private:
};