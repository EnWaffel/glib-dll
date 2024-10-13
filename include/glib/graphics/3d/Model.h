#pragma once

#include "../../DLLDefs.h"
#include "Mesh.h"

#include <vector>
#include <string>

namespace glib
{
	class Model
	{
	private:
		std::vector<Mesh*> m_Meshes;
	public:
		GLIB_API Model(const std::vector<Mesh*>& meshes);
		GLIB_API ~Model();

		GLIB_API static Model* LoadModel(const std::string& path, bool pixelart = false);

	friend class Camera3DRenderer;
	};
}