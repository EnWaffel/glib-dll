#pragma once

#include "../../DLLDefs.h"
#include "glib/math/Vec2.h"
#include "glib/math/Vec3.h"

#include <vector>
#include <string>

#define GLIB_MESH_TEX_TYPE_DIFFUSE 1
#define GLIB_MESH_TEX_TYPE_SPECULAR 2
#define GLIB_MESH_TEX_TYPE_NORMAL 3
#define GLIB_MESH_TEX_TYPE_HEIGHT 4

#define GLIB_MAX_BONE_INFLUENCE 4

namespace glib
{
	class Model;

	struct Vertex
	{
		Vec3 pos;
		Vec3 normal;
		Vec2 uvCoords;
		Vec3 tangent;
		Vec3 bitangent;
		int boneIDs[GLIB_MAX_BONE_INFLUENCE];
		float weights[GLIB_MAX_BONE_INFLUENCE];
	};

	struct MeshTexture
	{
		unsigned int id;
		unsigned int type;
		std::string filePath;
	};

	class Mesh
	{
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indices;
		std::vector<MeshTexture> m_Textures;
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_EBO;
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MeshTexture>& textures);
		~Mesh();

		friend class Camera3DRenderer;
	};
}