#include "scene/MeshManager.hpp"

std::unordered_map<std::string, Ref<Mesh>> MeshManager::m_meshs;

Ref<Mesh> MeshManager::get(const std::string &name)
{
    auto i = m_meshs.find(name);

    if (i == m_meshs.end())
    {
        throw std::runtime_error("Mesh Named " + name + " Don't Exist");
    }

    return i->second;
}

void MeshManager::register_mesh(const std::string &name, Ref<Mesh> mesh)
{
    MeshManager::m_meshs.insert(std::pair<std::string,Ref<Mesh>>(name, mesh));
}
