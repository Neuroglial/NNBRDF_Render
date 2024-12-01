#pragma once

#include <unordered_map>
#include <vector>
#include <stb_image.h>
#include "utils/utils.hpp"


class ImageManager
{
public:
    static Ref<Image> get(const std::string &path);

    static void register_image(const std::string &path);

private:
    static std::unordered_map<std::string,Ref<Image>> m_images;
};