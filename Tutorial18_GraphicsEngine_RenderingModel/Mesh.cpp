/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2022, PardCode

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <locale>
#include <codecvt>
#include <iostream>

#include "GraphicsEngine.h"

Mesh::Mesh(const wchar_t* full_path): Resource(full_path)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	std::string inputfile = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(full_path);

	std::string mtldir = inputfile.substr(0, inputfile.find_last_of("\\/"));

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str(), mtldir.c_str());

	if (!err.empty()) throw std::exception("Mesh not created successfully");

	if (!res) throw std::exception("Mesh not created successfully");

	//if (shapes.size() > 1) throw std::exception("Mesh not created successfully");


	std::vector<VertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;

	size_t size_vertex_index_lists = 0;

	for (size_t s = 0; s< shapes.size(); s++)
	{
		size_vertex_index_lists += shapes[s].mesh.indices.size();
	}

	list_vertices.reserve(size_vertex_index_lists);
	list_indices.reserve(size_vertex_index_lists);

	m_material_slots.resize(materials.size());

	std::cout << materials.size() << std::endl;
	size_t index_global_offset = 0;

	for (size_t m = 0; m < materials.size(); m++)
	{
		m_material_slots[m].start_index = index_global_offset;
		m_material_slots[m].material_id = m;

		for (size_t s = 0; s < shapes.size(); s++)
		{
			size_t index_offset = 0;
			//list_vertices.reserve(shapes[s].mesh.indices.size());
			//list_indices.reserve(shapes[s].mesh.indices.size());

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

				if (shapes[s].mesh.material_ids[f] != m)
				{
					index_offset += num_face_verts;
					continue;
				}

				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
					tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

					tinyobj::real_t nx = attribs.normals[index.normal_index * 3 + 0];
					tinyobj::real_t ny = attribs.normals[index.normal_index * 3 + 1];
					tinyobj::real_t nz = attribs.normals[index.normal_index * 3 + 2];

					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz));

					//VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty));
					list_vertices.push_back(vertex);

					//list_indices.push_back((unsigned int)index_offset + v);
					list_indices.push_back((unsigned int)index_global_offset + v);
				}

				index_offset += num_face_verts;
				index_global_offset += num_face_verts;
			}
		}
		m_material_slots[m].num_indices = index_global_offset - m_material_slots[m].start_index;
	}


	//for (size_t s = 0; s < shapes.size(); s++)
	//{
	//	size_t index_offset = 0;
	//	list_vertices.reserve(shapes[s].mesh.indices.size());
	//	list_indices.reserve(shapes[s].mesh.indices.size());

	//	for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
	//	{
	//		unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

	//		for (unsigned char v = 0; v < num_face_verts; v++)
	//		{
	//			tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

	//			tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
	//			tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
	//			tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

	//			tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
	//			tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

	//			tinyobj::real_t nx = attribs.normals[index.normal_index * 3 + 0];
	//			tinyobj::real_t ny = attribs.normals[index.normal_index * 3 + 1];
	//			tinyobj::real_t nz = attribs.normals[index.normal_index * 3 + 2];

	//		//	VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty));
	//			VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz));
	//			list_vertices.push_back(vertex);

	//			list_indices.push_back((unsigned int)index_offset + v);
	//		}

	//		index_offset += num_face_verts;
	//	}
	//}


	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
	m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&list_vertices[0], sizeof(VertexMesh),
		(UINT)list_vertices.size(), shader_byte_code, (UINT)size_shader);
	m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());

}

Mesh::Mesh(VertexMesh* vertex_list_data, unsigned vertex_list_size, unsigned* index_list_data, unsigned index_list_size,
	MaterialSlot* material_slot_list, unsigned material_slot_list_size) : Resource(L"")
{


	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
	m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list_data, sizeof(VertexMesh),
		(UINT)vertex_list_size, shader_byte_code, (UINT)size_shader);
	m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list_data, (UINT)index_list_size);

	m_material_slots.resize(material_slot_list_size);
	for (unsigned int i = 0; i<material_slot_list_size; i++)
	{
		m_material_slots[i] = material_slot_list[i];
	}

}


Mesh::~Mesh()
{
}

const VertexBufferPtr & Mesh::getVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr & Mesh::getIndexBuffer()
{
	return m_index_buffer;
}

const MaterialSlot& Mesh::getMaterialSlot(unsigned int slot)
{
	if (slot >= m_material_slots.size())
	{
		return MaterialSlot();
	}
	return m_material_slots[slot];
}

size_t Mesh::getNumMaterialSlots()
{
	return m_material_slots.size();
}
