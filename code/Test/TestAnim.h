#pragma once

#include "Renderer/Resource/VertexFormat.h"
#include "Renderer/Resource/ShaderProgram.h"
#include "Renderer/Resource/SamplerState.h"
#include "Renderer/Resource/Texture2D.h"

class TestAnim final : public IApp
{
public:
	bool Init() final;

	void Update(float dt) final;
	void Render() final;

	void Close() final;
};