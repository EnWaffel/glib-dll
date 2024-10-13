#include "glib/graphics/3d/Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <glad/glad.h>
#include <stb_image.h>
#include <filesystem>

using namespace glib;

struct LoadState
{
    const aiScene* scene;
    std::vector<Mesh*> meshes;
    std::string dir;
    std::vector<MeshTexture> textures;
    bool pixelart;
};

glib::Model::Model(const std::vector<Mesh*>& meshes) : m_Meshes(meshes)
{
}

glib::Model::~Model()
{
	for (Mesh* m : m_Meshes)
	{
		delete m;
	}
}

static unsigned int TextureFromFile(const char* path, const std::string& directory, LoadState& state)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        if (state.pixelart)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void ConvertBetweenBGRAandRGBA(unsigned char* input, int pixel_width, int pixel_hight, unsigned char* output) {

    for (int y = 0; y < pixel_hight; y++) {
        for (int x = 0; x < pixel_width; x++) {
            const unsigned char* pixel_in = &input[y * x * 4];

            unsigned char* pixel_out = &output[y * x * 4];
            pixel_out[0] = pixel_in[2];
            pixel_out[1] = pixel_in[1];
            pixel_out[2] = pixel_in[0];
            pixel_out[3] = pixel_in[3];
        }
    }
}

static unsigned int TextureFromMemory(const aiTexture* tex, LoadState& state)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = nullptr;
    GLenum format = GL_RGBA;

    if (tex->mHeight == 0)
    {
        data = stbi_load_from_memory((const stbi_uc*)tex->pcData, tex->mWidth, &width, &height, &nrComponents, 0);
        if (data)
        {
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;
        }
    }
    else
    {
        data = new unsigned char[(tex->mWidth * tex->mHeight) * sizeof(aiTexel)];
        ConvertBetweenBGRAandRGBA((unsigned char*)tex->pcData, tex->mWidth, tex->mHeight, data);
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if (state.pixelart)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    if (tex->mHeight != 0)
    {
        delete[] data;
    }
    else
    {
        stbi_image_free(data);
    }

    return textureID;
}

static std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, unsigned int typeID, LoadState& state)
{
    std::vector<MeshTexture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < state.textures.size(); j++)
        {
            if (std::strcmp(state.textures[j].filePath.data(), str.C_Str()) == 0)
            {
                textures.push_back(state.textures[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            MeshTexture texture;

            if (std::filesystem::exists(state.dir + '/' + str.C_Str()))
            {
                texture.id = TextureFromFile(str.C_Str(), state.dir, state);
            }
            else
            {
                const aiTexture* tex = state.scene->GetEmbeddedTexture(str.C_Str());
                if (tex == nullptr) continue;
                texture.id = TextureFromMemory(tex, state);
            }

            texture.type = typeID;
            texture.filePath = str.C_Str();
            textures.push_back(texture);
            state.textures.push_back(texture);
        }
    }
    return textures;
}

static Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, LoadState& state)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        Vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.pos = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            Vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.uvCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        }
        else
        {
            vertex.uvCoords = Vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, GLIB_MESH_TEX_TYPE_DIFFUSE, state);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    
    std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, GLIB_MESH_TEX_TYPE_SPECULAR, state);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    
    std::vector<MeshTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, GLIB_MESH_TEX_TYPE_NORMAL, state);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    
    std::vector<MeshTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, GLIB_MESH_TEX_TYPE_HEIGHT, state);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return new Mesh(vertices, indices, textures);
}

static void ProcessNode(aiNode* node, const aiScene* scene, LoadState& state)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        state.meshes.push_back(ProcessMesh(mesh, scene, state));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene, state);
    }

}

Model* glib::Model::LoadModel(const std::string& path, bool pixelart)
{
    LoadState state{};
    state.pixelart = pixelart;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "glib Error (assimp): " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    state.scene = scene;
    std::string dir = path.substr(0, path.find_last_of('/'));
    state.dir = dir;

    ProcessNode(scene->mRootNode, scene, state);

	return new Model(state.meshes);
}
