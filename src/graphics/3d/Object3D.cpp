#include "glib/graphics/3d/Object3D.h"
#include "glib/graphics/3d/Mesh.h"

glib::Object3D::Object3D() : pos(Vec3(0.0f, 0.0f, 0.0f)), scale(Vec3(1.0f, 1.0f, 1.0f)), color({ 1.0f, 1.0f, 1.0f, 1.0f }), model(nullptr), uvSize(glib::Vec2(1.0f, 1.0f))
{
    visible = true;
}

glib::Object3D::~Object3D()
{
}

void glib::Object3D::Draw()
{
}

void glib::Object3D::Update(float delta)
{
}

void glib::Object3D::MakeCube(Texture* texture)
{
	std::vector<Vertex> vertices;
    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2,    2, 3, 0,

        // Back face
        4, 5, 6,    6, 7, 4,

        // Left face
        8, 9, 10,   10, 11, 8,

        // Right face
        12, 13, 14, 14, 15, 12,

        // Top face
        16, 17, 18, 18, 19, 16,

        // Bottom face
        20, 21, 22, 22, 23, 20
    };

    float verts[] = {
        // Front face
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  // Bottom-left
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f,  // Bottom-right
         1.0f,  1.0f,  1.0f,  1.0f, 1.0f,  // Top-right
        -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,  // Top-left

        // Back face
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  // Bottom-left
         1.0f, -1.0f, -1.0f,  1.0f, 0.0f,  // Bottom-right
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f,  // Top-right
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,  // Top-left

        // Left face
        -1.0f,  1.0f,  1.0f,  1.0f, 0.0f,  // Top-right
        -1.0f,  1.0f, -1.0f,  1.0f, 1.0f,  // Top-left
        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,  // Bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  // Bottom-right

        // Right face
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f,  // Top-left
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f,  // Top-right
         1.0f, -1.0f, -1.0f,  0.0f, 1.0f,  // Bottom-right
         1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  // Bottom-left

         // Top face
         -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,  // Top-left
          1.0f,  1.0f, -1.0f,  1.0f, 1.0f,  // Top-right
          1.0f,  1.0f,  1.0f,  1.0f, 0.0f,  // Bottom-right
         -1.0f,  1.0f,  1.0f,  0.0f, 0.0f,  // Bottom-left

         // Bottom face
         -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,  // Top-right
          1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  // Top-left
          1.0f, -1.0f,  1.0f,  1.0f, 0.0f,  // Bottom-left
         -1.0f, -1.0f,  1.0f,  0.0f, 0.0f   // Bottom-right
    };

    for (int i = 0; i < sizeof(verts) / sizeof(float); i += 5)
    {
        Vertex v;
        v.pos = Vec3(verts[i], verts[i + 1], verts[i + 2]);
        v.uvCoords = Vec2(verts[i + 3], verts[i + 4]);
        vertices.push_back(v);
    }
    
    MeshTexture tex;
    tex.id = texture->GetID();
    tex.type = GLIB_MESH_TEX_TYPE_DIFFUSE;

	Mesh* mesh = new Mesh(vertices, indices, { tex });
    model = new Model({ mesh });
}

void glib::Object3D::MakeQuad(Texture* texture, const Vec2& size)
{
    scale = Vec3(size.x, size.y, 1.0f);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };

    float verts[] = {
        // Positions        // Texture Coords
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // Bottom-left
         1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // Bottom-right
         1.0f,  1.0f, 0.0f, 0.0f, 0.0f,  // Top-right
        -1.0f,  1.0f, 0.0f, 1.0f, 0.0f   // Top-left
    };

    for (int i = 0; i < sizeof(verts) / sizeof(float); i += 5)
    {
        Vertex v;
        v.pos = Vec3(verts[i], verts[i + 1], verts[i + 2]);
        v.uvCoords = Vec2(verts[i + 3], verts[i + 4]);
        vertices.push_back(v);
    }

    MeshTexture tex;
    tex.id = texture->GetID();
    tex.type = GLIB_MESH_TEX_TYPE_DIFFUSE;

    Mesh* mesh = new Mesh(vertices, indices, { tex });
    model = new Model({ mesh });
}
