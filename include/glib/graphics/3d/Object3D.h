#pragma once

#include "../../DLLDefs.h"
#include "../Drawable.h"
#include "glib/math/Vec3.h"
#include "glib/utils/Color.h"
#include "Model.h"
#include "glib/graphics/Texture.h"

namespace glib
{
	class Object3D : public Drawable
	{
	public:
		Vec3 pos;
		Vec3 scale;
		Vec3 rotation;
		Vec3 pivotPoint;
		Color color;
		Model* model;
		Vec2 uvPos; // ! Only for quads
		Vec2 uvSize; // ! Only for quads
	public:
		GLIB_API Object3D();
		GLIB_API ~Object3D();

		GLIB_API void Draw() override;
		GLIB_API void Update(float delta) override;

		GLIB_API void MakeCube(Texture* texture);
		GLIB_API void MakeQuad(Texture* texture, const Vec2& size);
	};
}