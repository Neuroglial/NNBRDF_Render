// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once

#include <unordered_map>
#include <vector>
#include "utils/utils.hpp"
#include "core/platform/renderAPI/Mesh.hpp"


class MeshManager
{
public:
    static Ref<Mesh> get(const std::string &name);

    static void register_mesh(const std::string &name,Ref<Mesh> mesh = nullptr);

private:
    static std::unordered_map<std::string,Ref<Mesh>> m_meshs;
};