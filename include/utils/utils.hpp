#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>


namespace std
{
    string to_string(const glm::vec2 &val);
    string to_string(const glm::vec3 &val);
    string to_string(const glm::vec4 &val);
}

std::string read_from_file(const std::string& path);