#pragma once

#include "Renderer/Resource/RenderTarget2D.h"
#include "Renderer/Resource/VertexBuffer.h"
#include "Renderer/Resource/IndexBuffer.h"
#include "Renderer/Resource/ConstantBuffer.h"

class RenderSystem : public Singleton<RenderSystem>
{
public:
	void SetRenderTarget2D(std::shared_ptr<RenderTarget2D> rt);
	void SetViewport(int x, int y, int w, int h);

	void Clear(const glm::vec4 &color, float depth = 1.0f, uint8_t stencil = 0);

private:
	std::shared_ptr<RenderTarget2D> m_rt;
};