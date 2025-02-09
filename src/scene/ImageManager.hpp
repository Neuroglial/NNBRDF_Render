// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once

#include <unordered_map>
#include <vector>
#include <stb_image.h>
#include "utils/utils.hpp"

class ImageManager
{
public:
    static Ref<Image> get(const std::string &path, bool yflip = true);
    static Ref<Image> get_hdr(const std::string &path, bool yflip = true);
    static void register_image(const std::string &path);

private:
    // string 为路径，pair 前一个Image是没有y轴翻转的图片，后一个是翻转后的图片
    static std::unordered_map<std::string, std::pair<Ref<Image>, Ref<Image>>> m_images;
};