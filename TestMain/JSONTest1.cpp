#include "nlohmann/json.hpp"
#include "utils/utils.hpp"
#include <fstream>
#include <iostream>
using namespace std;
using json = nlohmann::json;
int main() {
	json j;			// 创建 json 对象
	ifstream jfile(Path("resource/JSON/JSONTest1.json"));
	jfile >> j;		// 以文件流形式读取 json 文件
	float pi = j.at("pi");
	bool happy = j.at("happy");

	
	return 0;
}