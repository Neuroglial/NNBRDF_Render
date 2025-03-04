#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include "utils/utils.hpp"

using json = nlohmann::json;

int main() {
    json j;
    std::ifstream jfile(Path("resource/JSON/JSONTest2.json"));
    jfile >> j;

    // 打印output对象【也可以用j["output"].at("width")】
    std::cout << j.at("output").at("width") << std::endl;
    std::cout << j.at("output").at("height") << std::endl;
    std::cout << j.at("output").at("frameRate") << std::endl;
    std::cout << j.at("output").at("crf") << std::endl;
    // 打印tracks数组对象
    for(int i=0; i<j["tracks"].size(); i++) {
        std::cout << j["tracks"][i].at("name") << std::endl;

        // 打印pieces子数组对象
        json j2 = j["tracks"][i].at("pieces");
        for(int k=0; k<j2.size(); k++) {
            std::cout << j2[k].at("file") << std::endl;
            std::cout << j2[k].at("startTime") << std::endl;
            std::cout << j2[k].at("endTime") << std::endl;
        }
    }

    return 0;
}