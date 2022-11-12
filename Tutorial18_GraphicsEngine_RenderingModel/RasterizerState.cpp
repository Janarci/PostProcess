#include "RasterizerState.h"
#include "GraphicsEngine.h"


RasterizerState::RasterizerState()
{
}

void RasterizerState::release()
{
	m_rs->Release();
	delete this;
}

bool RasterizerState::init(bool wireframe)
{
	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(D3D11_RASTERIZER_DESC));

	if (wireframe)
	{
		rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

	}
	else
	{
		rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	}
	rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;


	if (!SUCCEEDED(GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateRasterizerState(&rasterizer_desc, &m_rs)))
		return false;

	return true;

}

RasterizerState::~RasterizerState()
{
}
