#include <fstream>
#include <iostream>

#include <glad/glad.h>

std::string ReadShaderSource(const char* source) {
    std::string contents = "";

    std::ifstream file;
    file.open(source);
    std::string line;

    if(!file.is_open()) {
        std::cerr << "File " << printf("%s", source) << " could not be open." << std::endl;
        return "";
    }

    while (std::getline(file, line)) {
        contents.append(line);
        contents.append("\n");
    }
    file.close();

    return contents;
}