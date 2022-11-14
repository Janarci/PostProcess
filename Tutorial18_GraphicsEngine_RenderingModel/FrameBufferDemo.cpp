

#include "FrameBufferDemo.h"

#include <iostream>
#include <Windows.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"
struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};


FrameBufferDemo::FrameBufferDemo()
{
}

void FrameBufferDemo::update()
{
	m_list_materials.clear();
	m_list_materials.push_back(m_mat);

	m_list_materials.clear();
	m_list_materials.push_back(m_mat);

	cam_Ptr->cameraUpdate(m_cb, this->getClientWindowRect(), m_mat, m_list_materials);

	//m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}


FrameBufferDemo::~FrameBufferDemo()
{
}

void FrameBufferDemo::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	m_wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\brick.png");
	screen_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\wood.jpg");

	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\sphere.obj");

	m_monitor_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\monitor.obj");



	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	std::cout << width << " " << height;
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	/*
	Cube* cubeObject = new Cube("Cube", shader_byte_code, size_shader, m_cb);
	cubeObject->setPosition(Vector3D(0.5, 0.5, 0));
	cubeObject->setScale(Vector3D(0.4f, 0.4f, 0.4f));
	this->cubeList.push_back(cubeObject);
	*/
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	m_mat = GraphicsEngine::get()->createMaterial(L"VertexShader.hlsl", L"PixelShader.hlsl");
	m_mat->addTexture(m_wood_tex);
	m_mat->setCullMode(CULL_MODE_BACK);

	m_monitor_mat = GraphicsEngine::get()->createMaterial(L"VertexShader.hlsl", L"PixelShader.hlsl");
	m_monitor_mat->addTexture(m_wood_tex);
	m_screen_mat = GraphicsEngine::get()->createMaterial(L"VertexShader.hlsl", L"PixelShader.hlsl");
	m_screen_mat->addTexture(screen_tex);

	m_post_process_mat = GraphicsEngine::get()->createMaterial(L"PostProcessVS.hlsl", L"Distort.hlsl");
	m_post_process_mat->setCullMode(CULL_MODE_BACK);

	VertexMesh quad_vertex_list[] = {
		VertexMesh(Vector3D(-1,-1,0), Vector2D(0,1), Vector3D(),Vector3D(),Vector3D()),
		VertexMesh(Vector3D(-1,1,0), Vector2D(0,0), Vector3D(),Vector3D(),Vector3D()),
		VertexMesh(Vector3D(1,1,0), Vector2D(1,0), Vector3D(),Vector3D(),Vector3D()),
		VertexMesh(Vector3D(1,-1,0), Vector2D(1,1), Vector3D(),Vector3D(),Vector3D())
	};

	unsigned int quad_index_list[] = {
		0,1,2,
		2,3,0
	};

	MaterialSlot quad_mat_slots[] = {
		{0,6,0}
	};

	m_quad_mesh = GraphicsEngine::get()->getMeshManager()->createMesh(quad_vertex_list, 4,
		quad_index_list, 6,
		quad_mat_slots, 1);

	m_list_materials.reserve(32);

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

	cam_1.initializeCamera(true);
	cam_2.initializeCamera(false);
	cam_Ptr = &cam_1;

	m_render_target = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(rc.right - rc.left, rc.bottom - rc.top), Texture::Type::RenderTarget);
	m_depth_stencil = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(rc.right - rc.left, rc.bottom - rc.top), Texture::Type::DepthStencil);

	m_post_process_mat->addTexture(m_render_target);

}

void FrameBufferDemo::onUpdate()//draw mesh
{
	Window::onUpdate();

	InputSystem::get()->update();

	//CLEAR THE RENDER TARGET 
	
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTarget(this->m_render_target, 
		0, 0.8f, 0.8f, 1);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearDepthStencil(this->m_depth_stencil);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setRenderTarget(this->m_render_target, this->m_depth_stencil);
	Rect view_size = m_render_target->getSize();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	
	
	m_list_materials.clear();
	m_list_materials.push_back(m_mat);
	//m_list_materials.push_back(m_monitor_mat);

	cam_Ptr->cameraUpdate(m_cb, this->getClientWindowRect(), m_mat, m_list_materials);
	
	//update();
/*
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_ps, m_wood_tex);
	*/

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_mesh->getVertexBuffer());
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_mesh->getIndexBuffer());

	
	for (size_t m = 0; m < m_mesh->getNumMaterialSlots(); m++)
	{
		if (m >= m_list_materials.size())
		{
			break;
		}
	
		MaterialSlot mat = m_mesh->getMaterialSlot(m);
	
	GraphicsEngine::get()->setMaterial(m_list_materials[m]);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mat.num_indices, 0, mat.start_index);
	}

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	m_list_materials.clear();
	m_list_materials.push_back(m_mat);

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_mesh->getVertexBuffer());
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_mesh->getIndexBuffer());

	for (size_t m = 0; m < m_mesh->getNumMaterialSlots(); m++)
	{
		if (m >= m_list_materials.size())
		{
			break;
		}

		MaterialSlot mat = m_mesh->getMaterialSlot(m);

		GraphicsEngine::get()->setMaterial(m_list_materials[m]);
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mat.num_indices, 0, mat.start_index);
	}

	

	m_list_materials.clear();
	m_list_materials.push_back(m_post_process_mat);

	//quad mesh
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_quad_mesh->getVertexBuffer());
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_quad_mesh->getIndexBuffer());

	for (size_t m = 0; m < m_quad_mesh->getNumMaterialSlots(); m++)
	{
		if (m >= m_list_materials.size())
		{
			break;
		}
		std::cout << m_list_materials.size()<< std::endl;

		MaterialSlot mat = m_quad_mesh->getMaterialSlot(m);

		GraphicsEngine::get()->setMaterial(m_list_materials[m]);
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mat.num_indices, 0, mat.start_index);
	}
	

	//GraphicsEngine::get()->setMaterial(m_mat);
	// FINALLY DRAW THE TRIANGLE

	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);


	for (int i = 0; i < this->cubeList.size(); i++) {
		this->cubeList[i]->draw(width, height, this->m_vs, this->m_ps, this->m_cb, this->cam_Ptr);
	}
	
	
	m_swap_chain->present(true);


	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void FrameBufferDemo::onDestroy()
{
	Window::onDestroy();
}

void FrameBufferDemo::onFocus()
{
	InputSystem::get()->addListener(this);
}

void FrameBufferDemo::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void FrameBufferDemo::onKeyDown(int key)
{
	if (key == 'W')
	{
		cam_Ptr->m_forward = 1.0f;

		//m_rot_x += 3.14f*m_delta_time;
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		cam_Ptr->m_forward = -1.0f;

		//m_rot_x -= 3.14f*m_delta_time;
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		cam_Ptr->m_rightward = -1.0f;

		//m_rot_y += 3.14f*m_delta_time;
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		cam_Ptr->m_rightward = 1.0f;

		//m_rot_y -= 3.14f*m_delta_time;
		m_rightward = 1.0f;
	}
}
void FrameBufferDemo::onKeyUp(int key)
{
	cam_Ptr->m_forward = 0.0f;
	cam_Ptr->m_rightward = 0.0f;
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void FrameBufferDemo::onMouseMove(const Point& mouse_pos)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);



	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time * 0.1f;

	cam_Ptr->m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
	cam_Ptr->m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time * 0.1f;


	InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));


}

void FrameBufferDemo::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 0.5f;
}

void FrameBufferDemo::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}

void FrameBufferDemo::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 2.0f;
}

void FrameBufferDemo::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}
