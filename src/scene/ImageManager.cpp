// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "scene/ImageManager.hpp"

std::unordered_map<std::string, Ref<Image>> ImageManager::m_images;

Ref<Image> ImageManager::get(const std::string &path)
{
    auto i = m_images.find(path);

    if(i == m_images.end()){
        i = ImageManager::m_images.insert(std::pair<std::string,Ref<Image>>(path,nullptr)).first;
    }

    if(i->second == nullptr){
        i->second = read_image(path);
        i->second->m_path = path;
    }

    return i->second;
}

void ImageManager::register_image(const std::string &path)
{
    ImageManager::m_images.insert(std::pair<std::string,Ref<Image>>(path,nullptr));
}
