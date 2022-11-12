#pragma once
#include "AGameObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "TextureManager.h"

class Cube : public AGameObject
{
public:
	Cube(string name, void* shaderByteCode, size_t sizeShader, ConstantBufferPtr constantBuffer);
	~Cube();

	void update(float deltaTime) override;
	void draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader, ConstantBufferPtr constantBuffer, Camera* cam) override;

private:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	TexturePtr m_wood_tex;
	MeshPtr m_mesh;
	MaterialPtr m_mat;


	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;

	SwapChainPtr m_swap_chain;
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	ConstantBufferPtr m_cb;


	//ConstantBuffer* m_cb;
	float m_delta_rot;
	long oldTime;
	long newDelta;

	float ticks = 0.0f;
	float deltaPos = 0.0f;
	float deltaTime = 0.0f;
	float speed = 10.0f;
};
