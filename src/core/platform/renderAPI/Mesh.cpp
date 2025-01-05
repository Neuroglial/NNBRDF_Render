// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "core/platform/renderAPI/Mesh.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Mesh::as_base_shape(Shape shape)
{
    switch (shape)
    {
    case Shape::Cube:
    {
        std::vector<float> vertex_data = {
            // |------Position------|  |------Normal-------|  |-Texcoord--|
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

        std::vector<unsigned int> index_data = {
            0, 1, 2, 3, 4, 5,
            6, 7, 8, 9, 10, 11,
            12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23,
            24, 25, 26, 27, 28, 29,
            30, 31, 32, 33, 34, 35};

        auto EBO = RenderAPI::creator<ArrayBuffer<unsigned int>>::crt();
        Ref<ArrayBuffer<float>> VBO(RenderAPI::creator<ArrayBuffer<float>>::crt());

        EBO->set_data(std::move(index_data));
        VBO->set_data(std::move(vertex_data));

        set_buffer(VBO, EBO, {ShaderParam_Type::Vec3, ShaderParam_Type::Vec3, ShaderParam_Type::Vec2});
        break;
    }

    case Shape::Quad:
    {
        std::vector<float> vertex_data = {
            // |------Position------|  |------Normal------|   |-Texcoord--|
            -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f};

        std::vector<unsigned int> index_data = {
            0,
            1,
            2,
            3,
            4,
            5,
        };

        Ref<ArrayBuffer<unsigned int>> EBO(RenderAPI::creator<ArrayBuffer<unsigned int>>::crt());
        Ref<ArrayBuffer<float>> VBO(RenderAPI::creator<ArrayBuffer<float>>::crt());

        EBO->set_data(std::move(index_data));
        VBO->set_data(std::move(vertex_data));

        set_buffer(VBO, EBO, {ShaderParam_Type::Vec3, ShaderParam_Type::Vec3, ShaderParam_Type::Vec2});
        break;
    }

    default:
        break;
    }
}

void Mesh::set_buffer(Ref<ArrayBuffer<float>> VBO, Ref<ArrayBuffer<unsigned int>> EBO, const std::vector<ShaderParam_Type> &layout)
{
    m_vertex_buffer = VBO;
    m_element_buffer = EBO;
    m_layout = layout;
    m_binded = false;
    m_draw_type = DRAW_ELEMENT;

    m_strike = 0;

    for (auto i : m_layout)
    {
        switch (i)
        {
        case ShaderParam_Type::Float:
            m_strike += sizeof(float);
            break;
        case ShaderParam_Type::Vec2:
            m_strike += sizeof(float) * 2;
            break;
        case ShaderParam_Type::Vec3:
            m_strike += sizeof(float) * 3;
            break;
        case ShaderParam_Type::Vec4:
            m_strike += sizeof(float) * 4;
            break;

        default:
            break;
        }
    }
}

void Mesh::set_buffer(Ref<ArrayBuffer<float>> VBO, const std::vector<ShaderParam_Type> &layout)
{
    set_buffer(VBO, nullptr, layout);
    m_draw_type = DRAW_ARRAY;
}

Ref<Mesh> Mesh::get_base_shape(Shape shape)
{
    std::vector<Ref<Mesh>> meshes;
    switch (shape)
    {
    case Cube:
        utils::loadModel(Root_Path + "resource/mesh/cube.obj", meshes);
        break;
    case Quad:
        utils::loadModel(Root_Path + "resource/mesh/quad.obj", meshes);
        break;

    default:
        break;
    }
    return meshes[0];
};

namespace utils
{
    Ref<Mesh> processMesh(aiMesh *mesh, const aiScene *scene)
    {
        std::vector<float> vertices(mesh->mNumVertices * sizeof(Vertex));

        int i = 0;

        // walk through each of the mesh's vertices
        for (unsigned int k = 0; k < mesh->mNumVertices; ++k)
        {
            auto &vert = *((Vertex *)(&vertices[i]));
            i += sizeof(Vertex) / 4;

            // vertex id
            vert.VertexID = k;

            // position
            vert.Position.x = mesh->mVertices[k].x;
            vert.Position.y = mesh->mVertices[k].y;
            vert.Position.z = mesh->mVertices[k].z;

            // normals
            if (mesh->HasNormals())
            {
                vert.Normal.x = mesh->mNormals[k].x;
                vert.Normal.y = mesh->mNormals[k].y;
                vert.Normal.z = mesh->mNormals[k].z;
            }
            else
            {
                vert.Normal.x = 0;
                vert.Normal.y = 0;
                vert.Normal.z = 1;
            }

            // tangent
            if (mesh->HasTangentsAndBitangents())
            {
                vert.Tangent.x = mesh->mTangents[k].x;
                vert.Tangent.y = mesh->mTangents[k].y;
                vert.Tangent.z = mesh->mTangents[k].z;
                vert.Tangent.w = 1;
            }
            else
            {
                vert.Tangent.x = 1;
                vert.Tangent.y = 0;
                vert.Tangent.z = 0;
                vert.Tangent.w = 1;
            }

            // Color
            if (mesh->HasVertexColors(0))
            {
                vert.Color.r = mesh->mColors[0][k].r;
                vert.Color.g = mesh->mColors[0][k].g;
                vert.Color.b = mesh->mColors[0][k].b;
                vert.Color.a = mesh->mColors[0][k].a;
            }
            else
            {
                vert.Color.r = 0;
                vert.Color.g = 0;
                vert.Color.b = 0;
                vert.Color.a = 1;
            }

            // texture coordinates
            if (mesh->mTextureCoords[0])
            {

                vert.TexCoord.x = mesh->mTextureCoords[0][k].x;
                vert.TexCoord.y = mesh->mTextureCoords[0][k].y;
            }
            else
            {
                vert.TexCoord.x = 0;
                vert.TexCoord.y = 0;
            }
            if (mesh->mTextureCoords[1])
            {

                vert.TexCoord.z = mesh->mTextureCoords[1][k].x;
                vert.TexCoord.w = mesh->mTextureCoords[1][k].y;
            }
            else
            {
                vert.TexCoord.z = 0;
                vert.TexCoord.w = 0;
            }

            // TODO Bone
            if (mesh->HasBones())
            {
            }
        }

        std::vector<unsigned int> elements(mesh->mNumFaces * 3);
        i = 0;

        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int k = 0; k < mesh->mNumFaces; ++k)
        {
            aiFace face = mesh->mFaces[k];
            // retrieve all indices of the face and store them in the indices vector

            for (unsigned int j = 0; j < 3; ++j)
                elements[i++] = face.mIndices[j];
        }

        auto vbo = RenderAPI::creator<ArrayBuffer<float>>::crt();
        vbo->set_data(std::move(vertices));
        auto ebo = RenderAPI::creator<ArrayBuffer<unsigned int>>::crt();
        ebo->set_data(std::move(elements));

        auto m_mesh = RenderAPI::creator<Mesh>::crt();
        m_mesh->set_buffer(vbo, ebo, get_vertex_layout());

        return m_mesh;
    }

    void processNode(aiNode *node, const aiScene *scene, std::vector<Ref<Mesh>> &meshes)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene.
            // the scene contains all the data, node is just to keep stuff organized (like 7urelations between nodes).
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene, meshes);
        }
    }

    void loadModel(const std::string &path, std::vector<Ref<Mesh>> &meshes)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene, meshes);
    }
}