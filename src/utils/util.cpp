#include "utils/utils.hpp"


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>


namespace std
{
    string to_string(const glm::vec2 &val)
    {
        return "(" + std::to_string(val.x) + "," + std::to_string(val.y) + ")";
    }
    string to_string(const glm::vec3 &val)
    {
        return "(" + std::to_string(val.x) + "," + std::to_string(val.y) + "," + std::to_string(val.z) + ")";
    }

    string to_string(const glm::vec4 &val)
    {
        return "(" + std::to_string(val.x) + "," + std::to_string(val.y) + "," + std::to_string(val.z)+ "," + std::to_string(val.w) + ")";
    }
}

std::string read_from_file(const std::string& path){
        std::string readed;
        std::ifstream file;

        file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        file.open(path.c_str());
        std::stringstream stream;
        stream << file.rdbuf();

        file.close();
        readed = stream.str();

        return readed;
}