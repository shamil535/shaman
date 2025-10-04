#pragma once
#include "includes.h"

#define LOG_ERROR() \
	logger::log("[!] failed at in %s:%d, (0x%lX)\n", __FILE__, __LINE__, GetLastError())

namespace logger
{
    template <typename ... T>
    __forceinline void log(const char* format, T const& ... args)
    {
        printf(format, args ...);
    }

    // Fun��o para verificar se a linha j� existe no arquivo
    inline bool lineExists(const std::string& filename, const std::string& line) {
        std::ifstream file(filename);
        std::string fileLine;
        while (std::getline(file, fileLine)) {
            if (fileLine == line) {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    // Fun��o para adicionar uma nova linha ao arquivo, se ela n�o existir
    inline void ExtLog(const std::string& filename, const std::string& escrever) {
        // Verificar se a linha j� existe no arquivo
        if (lineExists(filename, escrever)) {
            // Se j� existir, n�o escrever e retornar
            return;
        }

        // Abrir o arquivo em modo de escrita, com append
        std::ofstream outputFile(filename, std::ios::app);

        // Escrever a string no arquivo
        outputFile << escrever << std::endl;

        // Fechar o arquivo ap�s a escrita
        outputFile.close();
    }
}

