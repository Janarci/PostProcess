#pragma once
#include "Camera.h"
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Cube.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Matrix4x4.h"


class FrameBufferDemo : public Window, public InputListener
{
public:
	FrameBufferDemo();

	void update();

	~FrameBufferDemo();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;

	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;

	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

private:
	SwapChainPtr m_swap_chain;
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	ConstantBufferPtr m_cb;
	TexturePtr m_wood_tex;
	TexturePtr screen_tex;

	TexturePtr m_render_target;
	TexturePtr m_depth_stencil;

	MeshPtr m_quad_mesh;
	MeshPtr m_mesh;

	MaterialPtr m_mat;
	MaterialPtr m_post_process_mat;

	std::vector<MaterialPtr> m_list_materials;

	MeshPtr m_monitor_mesh;
	MaterialPtr m_monitor_mat;
	MaterialPtr m_screen_mat;


private:
	long m_old_delta;
	long m_new_delta;
	float m_delta_time;

	float m_delta_pos;
	float m_delta_scale;
	float m_delta_rot;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float m_scale_cube = 1;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	Matrix4x4 m_world_cam;

	vector<Cube*> cubeList;

	Matrix4x4* camPtr;

	Camera cam_1;
	Camera cam_2;
	Camera* cam_Ptr;


};

