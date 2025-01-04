// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "scene/ImageManager.hpp"

std::unordered_map<std::string, std::pair<Ref<Image>, Ref<Image>>> ImageManager::m_images;

Ref<Image> ImageManager::get(const std::string &path, bool yflip)
{
    auto i = m_images.find(path);

    if (i == m_images.end())
    {
        ImageManager::m_images.emplace(path, std::pair<Ref<Image>, Ref<Image>>(nullptr, nullptr));
        i = m_images.find(path);
    }

    if (yflip)
    {
        if (i->second.second == nullptr)
        {
            i->second.second = utils::read_image(path, true);
        }
        return i->second.second;
    }
    else
    {
        if (i->second.first == nullptr)
        {
            i->second.first = utils::read_image(path, false);
        }
        return i->second.first;
    }
}

Ref<Image> ImageManager::get_hdr(const std::string &path, bool yflip)
{
    auto i = m_images.find(path);

    if (i == m_images.end())
    {
        ImageManager::m_images.emplace(path, std::pair<Ref<Image>, Ref<Image>>(nullptr, nullptr));
        i = m_images.find(path);
    }

    if (yflip)
    {
        if (i->second.second == nullptr)
        {
            i->second.second = utils::read_image_hdr(path, true);
        }
        return i->second.second;
    }
    else
    {
        if (i->second.first == nullptr)
        {
            i->second.first = utils::read_image_hdr(path, false);
        }
        return i->second.first;
    }
}

void ImageManager::register_image(const std::string &path)
{
    ImageManager::m_images.emplace(path, std::pair<Ref<Image>, Ref<Image>>(nullptr, nullptr));
}
