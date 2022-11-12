#include "Cube.h"

#include <DirectXTex.h>
#include <iostream>

#include "Camera.h"
#include "DeviceContext.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"
#include "Mesh.h"
#include "SwapChain.h"

Cube::Cube(string name, void* shaderByteCode, size_t sizeShader, ConstantBufferPtr constantBuffer) :AGameObject(name)
{


	Vector3D position_list[] =
	{
		{ Vector3D(-0.5f,-0.5f,-0.5f)},
		{ Vector3D(-0.5f,0.5f,-0.5f) },
		{ Vector3D(0.5f,0.5f,-0.5f) },
		{ Vector3D(0.5f,-0.5f,-0.5f)},

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f) },
		{ Vector3D(0.5f,0.5f,0.5f) },
		{ Vector3D(-0.5f,0.5f,0.5f)},
		{ Vector3D(-0.5f,-0.5f,0.5f) }
	};

	Vector2D texcoord_list[] =
	{
		{ Vector2D(0.0f,0.0f) },
		{ Vector2D(0.0f,1.0f) },
		{ Vector2D(1.0f,0.0f) },
		{ Vector2D(1.0f,1.0f) }
	};

	Vertex quadList[] = {
		//X - Y - Z
		//FRONT FACE
		{ position_list[0],texcoord_list[1] },
		{ position_list[1],texcoord_list[0] },
		{ position_list[2],texcoord_list[2] },
		{ position_list[3],texcoord_list[3] },

		//back
		{ position_list[4],texcoord_list[1] },
		{ position_list[5],texcoord_list[0] },
		{ position_list[6],texcoord_list[2] },
		{ position_list[7],texcoord_list[3] },

		//top
		{ position_list[1],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[2],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[0],texcoord_list[0] },
		{ position_list[3],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[3],texcoord_list[1] },
		{ position_list[2],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[1],texcoord_list[2] },
		{ position_list[0],texcoord_list[3] }
	};

	//this->vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	//this->vertexBuffer->load(quadList, sizeof(Vertex), ARRAYSIZE(quadList), shaderByteCode, sizeShader);

	UINT size_list = ARRAYSIZE(quadList);

	this->m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(quadList, sizeof(Vertex), size_list, shaderByteCode, (UINT)sizeShader);

	unsigned int indexList[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		8,9,10,
		10,11,8,
		//BOTTOM SIDE
		12,13,14,
		14,15,12,
		//RIGHT SIDE
		16,17,18,
		18,19,16,
		//LEFT SIDE
		20,21,22,
		22,23,20
	};
	//this->indexBuffer = GraphicsEngine::get()->createIndexBuffer();
	//this->indexBuffer->load(indexList, size_index_list);

	UINT size_index_list = ARRAYSIZE(indexList);
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(indexList, size_index_list);

	Data data;
	data.time = 0;

	//constantBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&data, sizeof(data));

	m_mat = GraphicsEngine::get()->createMaterial(L"VertexShader.hlsl", L"PixelShader.hlsl");

}

Cube::~Cube()
{
	AGameObject::~AGameObject();
}

void Cube::update(float deltaTime)
{

}

void Cube::draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader, ConstantBufferPtr constantBuffer, Camera* cam)
{
	if (!m_wood_tex)
	{
		m_wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\wood.jpg");
		m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\monitor.obj");
		m_mat->addTexture(m_wood_tex);

	}



	Data cbData = {};
	//cbData.time = this->ticks * this->speed;
	this->deltaPos += this->deltaTime / 10.0f;

	if (this->deltaPos > 1.0f) {
		this->deltaPos = 0.0f;
	}



	Matrix4x4 temp;
	m_delta_rot += deltaTime / 0.55f;

	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));

	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f,1.5f, 0), m_delta_pos));

	//cc.m_world *= temp;

	cbData.worldMatrix.setIdentity();
	cbData.worldMatrix.setScale(this->getLocalScale());

	//for rotations
	temp.setIdentity();
	temp.setRotationZ(m_delta_rot);
	//cbData.worldMatrix *= temp;

	//for rotations
	temp.setIdentity();
	temp.setRotationY(m_delta_rot);
	//cbData.worldMatrix *= temp;

	//for rotations
	temp.setIdentity();
	temp.setRotationX(m_delta_rot);
	//cbData.worldMatrix *= temp;


	temp.setIdentity();
	temp.setTranslation(this->getLocalPosition());

	cbData.worldMatrix *= temp;
	//cbData.worldMatrix.setIdentity();



	cbData.viewMatrix.setIdentity();
	cbData.viewMatrix = cam->getViewMatrix();
	/*
	cbData.projMatrix.setOrthoLH
	(
		width / 300.0f,
		height / 300.0f,
		-4.0f,
		4.0f
	);
	*/
	cbData.projMatrix.setPerspectiveFovLH(((float)width / (float)height), ((float)width / (float)height), 0.1f, 100.0f);
	//cbData.projMatrix.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

	//constantBuffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cbData);
	m_mat->setData(&cbData, sizeof(Data));

	/*
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(vertexShader, constantBuffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(pixelShader, constantBuffer);
	

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(pixelShader, m_wood_tex);
	*/
	GraphicsEngine::get()->setMaterial(m_mat);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(this->m_ib);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(this->m_vb);
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_mesh->getIndexBuffer());
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_mesh->getVertexBuffer());

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(this->m_ib->getSizeIndexList(), 0, 0);
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);

	oldTime = newDelta;
	newDelta = ::GetTickCount();

	deltaTime = (oldTime) ? ((newDelta - oldTime) / 1000.0f) : 0;
}
