#pragma once

#include <unordered_map>
#include <vector>
#include "utils/utils.hpp"
#include "core/platform/renderAPI/Mesh.hpp"


class MeshManager
{
public:
    static Ref<Mesh> get(const std::string &name);

    static void register_mesh(const std::string &name,Ref<Mesh> mesh);

private:
    static std::unordered_map<std::string,Ref<Mesh>> m_meshs;
};