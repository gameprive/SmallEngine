#pragma once

#include "Renderer/Resource/VertexFormat.h"
#include "Renderer/Resource/ShaderProgram.h"
#include "Renderer/Resource/SamplerState.h"
#include "Renderer/Resource/Texture2D.h"

class Test final : public IApp
{
public:
	bool Init() final;

	void Update(float dt) final;
	void Render() final;

private:
	std::shared_ptr<VertexBuffer> vertexbuffer;
	std::shared_ptr<VertexBuffer> colorbuffer;

	std::shared_ptr<ConstantBuffer> cb;

	std::shared_ptr<VertexFormat> format;

	std::shared_ptr<ShaderProgram> shaders;

	std::shared_ptr<SamplerState> sampler;
	std::shared_ptr<Texture2D> tex;

	std::shared_ptr<RenderTarget2D> rt;
};