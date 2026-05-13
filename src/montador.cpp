#include "montador.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "utils.h"

// ── tabela de instruções ────────────────────────────────────────────
struct InstrInfo { int opcode; int numOperands; };

std::map<std::string, InstrInfo> instrTable = {
    {"ADD",    {1,  1}},
    {"SUB",    {2,  1}},
    {"MUL",    {3,  1}},
    {"MULT",   {3,  1}},
    {"DIV",    {4,  1}},
    {"JMP",    {5,  1}},
    {"JMPN",   {6,  1}},
    {"JMPP",   {7,  1}},
    {"JMPZ",   {8,  1}},
    {"COPY",   {9,  2}},
    {"LOAD",   {10, 1}},
    {"STORE",  {11, 1}},
    {"INPUT",  {12, 1}},
    {"OUTPUT", {13, 1}},
    {"STOP",   {14, 0}},
};

// ── PASSAGEM 1: constrói tabela de símbolos ──────────────────────────

void firstPass(const std::vector<std::string>& lines,
               std::map<std::string, int>& symbolTable) {
    int locationCounter = 0;

    for (const std::string& line : lines) {
        auto tokens = tokenize(line);
        if (tokens.empty()) continue;

        int idx = 0;
        std::string label = "";

        if (tokens[idx].back() == ':') {
            label = retirarDoisPontos(tokens[idx]);
            if (symbolTable.count(label)) {
                std::cerr << "ERRO: símbolo redefinido: " << label << std::endl;
            } else {
                symbolTable[label] = locationCounter;
            }
            idx++;
            if (idx >= (int)tokens.size()) continue;
        }

        std::string op = tokens[idx];
        if (op == "SECTION") continue;

        if (op == "SPACE") {
            int size = 1;
            if (idx + 1 < (int)tokens.size())
                size = interpretarValor(tokens[idx + 1]);
            locationCounter += size;
        } else if (op == "CONST") {
            locationCounter += 1;
        } else if (instrTable.count(op)) {
            locationCounter += 1 + instrTable[op].numOperands;
        }
    }
}

// ── PASSAGEM 2: gera código de máquina (.obj) ───────────────────────

std::vector<int> secondPass(const std::vector<std::string>& lines,
                            const std::map<std::string, int>& symbolTable) {
    std::vector<int> machineCode;

    for (const std::string& line : lines) {
        auto tokens = tokenize(line);
        if (tokens.empty()) continue;

        int idx = 0;

        // Pula rótulo — já processado na passagem 1
        if (tokens[idx].back() == ':') {
            idx++;
            if (idx >= (int)tokens.size()) continue;
        }

        std::string op = tokens[idx];
        if (op == "SECTION") continue;

        if (op == "SPACE") {
            int size = 1;
            if (idx + 1 < (int)tokens.size())
                size = interpretarValor(tokens[idx + 1]);
            for (int i = 0; i < size; i++)
                machineCode.push_back(0);
        } else if (op == "CONST") {
            machineCode.push_back(interpretarValor(tokens[idx + 1]));
        } else if (instrTable.count(op)) {
            InstrInfo info = instrTable[op];
            machineCode.push_back(info.opcode);

            if (op == "COPY") {
                std::string operands = tokens[idx + 1];
                size_t comma = operands.find(',');
                std::string op1 = operands.substr(0, comma);
                std::string op2 = operands.substr(comma + 1);
                for (const std::string& operand : {op1, op2}) {
                    if (symbolTable.count(operand)) {
                        machineCode.push_back(symbolTable.at(operand));
                    } else {
                        machineCode.push_back(-1);
                        std::cerr << "AVISO: símbolo não definido: " << operand << std::endl;
                    }
                }
            } else if (info.numOperands == 1) {
                std::string operand = tokens[idx + 1];
                if (symbolTable.count(operand)) {
                    machineCode.push_back(symbolTable.at(operand));
                } else {
                    machineCode.push_back(-1);
                    std::cerr << "AVISO: símbolo não definido: " << operand << std::endl;
                }
            }
        }
    }
    return machineCode;
}

// ── PASSAGEM ÚNICA: gera .pen ────────────────────────────────────────

std::vector<std::string> singlePass(const std::vector<std::string>& lines) {
    std::vector<std::string> penCode;
    std::map<std::string, int> partialTable;
    int locationCounter = 0;

    for (const std::string& line : lines) {
        auto tokens = tokenize(line);
        if (tokens.empty()) continue;

        int idx = 0;

        // Registra rótulo na tabela parcial AGORA (no momento em que é visto)
        if (tokens[idx].back() == ':') {
            std::string label = retirarDoisPontos(tokens[idx]);
            partialTable[label] = locationCounter;
            idx++;
            if (idx >= (int)tokens.size()) continue;
        }

        std::string op = tokens[idx];
        if (op == "SECTION") continue;

        if (op == "SPACE") {
            int size = 1;
            if (idx + 1 < (int)tokens.size())
                size = interpretarValor(tokens[idx + 1]);
            for (int i = 0; i < size; i++) {
                penCode.push_back("00");
                locationCounter++;
            }
        } else if (op == "CONST") {
            int val = interpretarValor(tokens[idx + 1]);
            penCode.push_back(std::to_string(val));
            locationCounter++;
        } else if (instrTable.count(op)) {
            InstrInfo info = instrTable[op];
            penCode.push_back(std::to_string(info.opcode));
            locationCounter++;

            if (op == "COPY") {
                std::string operands = tokens[idx + 1];
                size_t comma = operands.find(',');
                std::string op1 = operands.substr(0, comma);
                std::string op2 = operands.substr(comma + 1);
                for (const std::string& operand : {op1, op2}) {
                    if (partialTable.count(operand)) {
                        // conhecido — resolve agora
                        penCode.push_back(std::to_string(partialTable.at(operand)));
                    } else {
                        // desconhecido — pendência
                        penCode.push_back("?" + operand);
                    }
                    locationCounter++;
                }
            } else if (info.numOperands == 1) {
                std::string operand = tokens[idx + 1];
                if (partialTable.count(operand)) {
                    // conhecido — resolve agora
                    penCode.push_back(std::to_string(partialTable.at(operand)));
                } else {
                    // desconhecido — pendência
                    penCode.push_back("?" + operand);
                }
                locationCounter++;
            }
        }
    }
    return penCode;
}

// ── função principal do montador ────────────────────────────────────
void montar(const std::string& arquivoEntrada,
              const std::string& arquivoObj,
              const std::string& arquivoPen) {
    std::ifstream in(arquivoEntrada);
    if (!in.is_open()) {
        std::cerr << "Erro ao abrir: " << arquivoEntrada << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line))
        if (!line.empty()) lines.push_back(line);
    in.close();

    // Passagem 1: constrói tabela de símbolos completa
    std::map<std::string, int> symbolTable;
    firstPass(lines, symbolTable);

    std::cout << "=== Tabela de Símbolos ===" << std::endl;
    for (auto& [sym, addr] : symbolTable)
        std::cout << "  " << sym << " -> " << addr << std::endl;

    // Passagem 2: gera .obj com todos os endereços resolvidos
    std::vector<int> machineCode = secondPass(lines, symbolTable);

    std::ofstream obj(arquivoObj);
    for (size_t i = 0; i < machineCode.size(); i++) {
        if (i > 0) obj << " ";
        obj << machineCode[i];
    }
    obj << "\n";
    obj.close();

    // Passagem única: gera .pen com pendências não resolvidas
    std::vector<std::string> penCode = singlePass(lines);

    std::ofstream pen(arquivoPen);
    for (size_t i = 0; i < penCode.size(); i++) {
        if (i > 0) pen << " ";
        pen << penCode[i];
    }
    pen << "\n";
    pen.close();

    std::cout << "Montagem concluída: " << arquivoObj << " e " << arquivoPen << std::endl;
}