#include "preprocessador.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include "utils.h"


// ── pré-processador principal ───────────────────────────────────────

void preprocessar(const std::string& arquivoEntrada, const std::string& arquivoSaida) {
    std::ifstream in(arquivoEntrada);
    if (!in.is_open()) {
        std::cerr << "Erro ao abrir: " << arquivoEntrada << std::endl;
        return;
    }

    // lê todas as linhas, remove comentários, converte para maiúsculas
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        line = toUpper(removerComentario(line));
        line = normalizarEspacos(line);
        if (!line.empty())
            lines.push_back(line);
    }
    in.close();

    // resolve EQU — monta tabela de substituições
    std::map<std::string, std::string> equTable;
    std::vector<std::string> afterEqu;

    for (const std::string& l : lines) {
        auto tokens = tokenize(l);
        if (tokens.size() == 3 && tokens[1] == "EQU") {
            equTable[tokens[0]] = tokens[2];
        } else {
            afterEqu.push_back(l);
        }
    }

    // aplica substituições EQU e resolve IF
    std::vector<std::string> afterIf;
    for (size_t i = 0; i < afterEqu.size(); i++) {
        auto tokens = tokenize(afterEqu[i]);

        // Substitui tokens que estão na tabela EQU
        std::string newLine;
        for (auto& tok : tokens) {
            if (equTable.count(tok)) tok = equTable[tok];
            newLine += tok + " ";
        }
        if (!newLine.empty()) newLine.pop_back();

        // Verifica se é diretiva IF
        tokens = tokenize(newLine);
        if (!tokens.empty() && tokens[0] == "IF") {
            // IF VALOR: se VALOR != 0, inclui próxima linha; senão pula
            int val = std::stoi(tokens[1]);
            if (val != 0 && i + 1 < afterEqu.size()) {
                afterIf.push_back(afterEqu[i + 1]); // inclui próxima linha
            }
            i++; // pula a próxima linha em ambos os casos
        } else {
            afterIf.push_back(newLine);
        }
    }

    // trata rótulo sozinho na linha (rótulo: + ENTER)
    // Une o rótulo com a linha seguinte
    std::vector<std::string> afterLabel;
    for (size_t i = 0; i < afterIf.size(); i++) {
        std::string& l = afterIf[i];
        // Se a linha termina com ":" é um rótulo sozinho
        if (!l.empty() && l.back() == ':' && i + 1 < afterIf.size()) {
            afterLabel.push_back(l + " " + afterIf[i + 1]);
            i++; // consome a próxima linha
        } else {
            afterLabel.push_back(l);
        }
    }

    // separa seções TEXT e DATA
    std::vector<std::string> textSection, dataSection;
    bool inData = false;

    for (const std::string& l : afterLabel) {
        auto tokens = tokenize(l);
        if (tokens.empty()) continue;

        if (tokens[0] == "SECTION" && tokens.size() > 1 && tokens[1] == "DATA") {
            inData = true;
            dataSection.push_back(l);
            continue;
        }
        if (tokens[0] == "SECTION" && tokens.size() > 1 && tokens[1] == "TEXT") {
            inData = false;
            textSection.push_back(l);
            continue;
        }

        if (inData)
            dataSection.push_back(l);
        else
            textSection.push_back(l);
    }

    // escreve saída — TEXT sempre antes de DATA
    std::ofstream out(arquivoSaida);
    for (const std::string& l : textSection) out << l << "\n";
    for (const std::string& l : dataSection) out << l << "\n";
    out.close();

    std::cout << "Pré-processamento concluído: " << arquivoSaida << std::endl;
}