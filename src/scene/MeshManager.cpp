// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "scene/MeshManager.hpp"

std::unordered_map<std::string, Ref<Mesh>> MeshManager::m_meshs;

Ref<Mesh> MeshManager::get(const std::string &name)
{
    auto i = m_meshs.find(name);

    if (i == m_meshs.end())
    {
        std::vector<Ref<Mesh>> meshes;
        utils::loadModel(Root_Path + name, meshes);

        auto f = m_meshs.emplace(name, meshes[0]);
        Assert(f.second);
        i = f.first;

        i->second->set_path(name);
    }

    return i->second;
}

// void MeshManager::register_mesh(const std::string &name, Ref<Mesh> mesh)
// {
//     MeshManager::m_meshs.insert(std::pair<std::string, Ref<Mesh>>(name, mesh));
// }
