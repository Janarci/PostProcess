#pragma once
#include <d3d11.h>


class GraphicsEngine;
class DeviceContext;

class RasterizerState
{

public:
	RasterizerState();
	void release();
	~RasterizerState();

private:
	bool init(bool wireframe);

private:
	ID3D11RasterizerState* m_rs;


private:
	friend class GraphicsEngine;
	friend class DeviceContext;
};