// src/main.cpp
#include <iostream>
#include <string>
#include "preprocessor.h"
#include "assembler.h"
#include "simulator.h"

std::string getExtension(const std::string& filename) {
    size_t pos = filename.rfind('.');
    if (pos == std::string::npos) return "";
    return filename.substr(pos); // retorna ".asm", ".pre" ou ".obj"
}

std::string getBaseName(const std::string& filename) {
    size_t pos = filename.rfind('.');
    return filename.substr(0, pos); // retorna "myfile"
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: ./montador <arquivo>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string ext = getExtension(inputFile);
    std::string base = getBaseName(inputFile);

    if (ext == ".asm") {
        preprocess(inputFile, base + ".pre");
    } else if (ext == ".pre") {
        assemble(inputFile, base + ".obj", base + ".pen");
    } else if (ext == ".obj") {
        simulate(inputFile);
    } else {
        std::cerr << "Extensão não reconhecida: " << ext << std::endl;
        return 1;
    }

    return 0;
}