#include <iostream>
#include <string>
#include "preprocessador.h"
#include "montador.h"
#include "simulador.h"

std::string getExtensao(const std::string& arquivo) {
    size_t pos = arquivo.rfind('.');
    if (pos == std::string::npos) return "";
    return arquivo.substr(pos); // retorna ".asm", ".pre" ou ".obj"
}

std::string getName(const std::string& arquivo) {
    size_t pos = arquivo.rfind('.');
    return arquivo.substr(0, pos); // retorna "myfile"
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: ./montador <arquivo>" << std::endl;
        return 1;
    }

    std::string arquivoEntrada = argv[1];
    std::string ext = getExtensao(arquivoEntrada);
    std::string base = getName(arquivoEntrada);

    if (ext == ".asm") {
        preprocessar(arquivoEntrada, base + ".pre");
    } else if (ext == ".pre") {
        montar(arquivoEntrada, base + ".obj", base + ".pen");
    } else if (ext == ".obj") {
        simular(arquivoEntrada);
    } else {
        std::cerr << "Extensão não reconhecida: " << ext << std::endl;
        return 1;
    }

    return 0;
}