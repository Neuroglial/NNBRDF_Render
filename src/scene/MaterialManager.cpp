// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "scene/MaterialManager.hpp"
#include "scene/Serialize.hpp"

std::unordered_map<std::string, Ref<Material>> MaterialManager::m_mats;

Ref<Material> MaterialManager::get(const std::string &path)
{
    auto fd = m_mats.find(path);

    if (fd == m_mats.end())
    {
        auto mat = from_file<Ref<Material>>(path);
        mat->set_path(path);

        auto f = m_mats.emplace(path, mat);
        Assert(f.second);
        fd = f.first;
    }

    return fd->second;
}

bool MaterialManager::storage(Ref<Material> mat)
{
    if (mat == nullptr)
        return false;

    Assert(mat->get_path().size() > 0);

    to_file(mat, mat->get_path());
}
