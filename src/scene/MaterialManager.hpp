// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once

#include <unordered_map>
#include <vector>
#include "utils/utils.hpp"
#include "core/render/Material.hpp"

class MaterialManager
{
public:
    static Ref<Material> get(const std::string &path);
    static bool storage(Ref<Material> mat);

private:
    static std::unordered_map<std::string, Ref<Material>> m_mats;
};