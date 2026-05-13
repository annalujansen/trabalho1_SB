#include "simulador.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

void simular(const std::string& arquivoObj) {
    std::ifstream in(arquivoObj);
    if (!in.is_open()) {
        std::cerr << "Erro ao abrir: " << arquivoObj << std::endl;
        return;
    }

    // lê o .obj e coloca na "memória" (vetor)
    std::vector<int> memory;
    int val;
    while (in >> val) memory.push_back(val);
    in.close();

    // REGISTRADORES
    int PC  = 0; // Program Counter
    int ACC = 0; // Acumulador

    // CICLO BUSCA-DECODIFICA-EXECUTA
    while (PC < (int)memory.size()) {
        int opcode = memory[PC];

        switch (opcode) {
            case 1: // ADD
                ACC += memory[memory[PC + 1]];
                PC += 2; break;
            case 2: // SUB
                ACC -= memory[memory[PC + 1]];
                PC += 2; break;
            case 3: // MULT
                ACC *= memory[memory[PC + 1]];
                PC += 2; break;
            case 4: // DIV
                if (memory[memory[PC + 1]] == 0) {
                    std::cerr << "Erro: divisão por zero" << std::endl;
                    return;
                }
                ACC /= memory[memory[PC + 1]];
                PC += 2; break;
            case 5: // JMP
                PC = memory[PC + 1]; break;
            case 6: // JMPN
                PC = (ACC < 0)  ? memory[PC + 1] : PC + 2; break;
            case 7: // JMPP
                PC = (ACC > 0)  ? memory[PC + 1] : PC + 2; break;
            case 8: // JMPZ
                PC = (ACC == 0) ? memory[PC + 1] : PC + 2; break;
            case 9: // COPY
                memory[memory[PC + 2]] = memory[memory[PC + 1]];
                PC += 3; break;
            case 10: // LOAD
                ACC = memory[memory[PC + 1]];
                PC += 2; break;
            case 11: // STORE
                memory[memory[PC + 1]] = ACC;
                PC += 2; break;
            case 12: // INPUT
                std::cin >> memory[memory[PC + 1]];
                PC += 2; break;
            case 13: // OUTPUT
                std::cout << memory[memory[PC + 1]] << std::endl;
                PC += 2; break;
            case 14: // STOP
                return;
            default:
                std::cerr << "Opcode desconhecido: " << opcode
                          << " no endereço " << PC << std::endl;
                return;
        }
    }
}