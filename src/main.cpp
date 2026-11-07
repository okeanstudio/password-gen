// Demian Vasilev (Okean Studio), 08.11.2026, password-gen
#include "password.h"

#include <iostream>
#include <string>

namespace {

void printHelp(const char* prog) {
    std::cout <<
        "Usage: " << prog << " [options]\n"
        "Options:\n"
        "  -l, --length N     длина пароля (по умолчанию 16)\n"
        "  -L, --lower        использовать строчные (вкл. по умолчанию)\n"
        "  -U, --upper        использовать заглавные (вкл. по умолчанию)\n"
        "  -D, --digits       использовать цифры (вкл. по умолчанию)\n"
        "  -S, --symbols      использовать символы\n"
        "  -a, --avoid        исключить неоднозначные символы (l,1,I,O,0)\n"
        "  -h, --help         показать помощь\n";
}

} // namespace

int main(int argc, char** argv) {
    PasswordOptions opts;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-l" || arg == "--length") && i + 1 < argc) {
            try {
                opts.length = std::stoi(argv[++i]);
            } catch (...) {
                std::cerr << "Ошибка: некорректная длина\n";
                return 1;
            }
        } else if (arg == "-S" || arg == "--symbols") {
            opts.useSymbols = true;
        } else if (arg == "-a" || arg == "--avoid") {
            opts.avoidAmbiguous = true;
        } else if (arg == "-h" || arg == "--help") {
            printHelp(argv[0]);
            return 0;
        } else {
            std::cerr << "Неизвестный аргумент: " << arg << "\n";
            printHelp(argv[0]);
            return 1;
        }
    }

    if (opts.length <= 0) {
        std::cerr << "Ошибка: длина должна быть положительной\n";
        return 1;
    }

    const std::string pwd = generatePassword(opts);
    if (pwd.empty()) {
        std::cerr << "Ошибка: не выбран ни один набор символов\n";
        return 1;
    }

    const double entropy = estimateEntropy(pwd);
    const bool weak = isWeakPassword(pwd);

    std::cout << "Пароль:  " << pwd << "\n";
    std::cout << "Энтропия: " << entropy << " бит\n";
    std::cout << "Оценка:  " << (weak ? "слабый (есть повторы/последовательности)" : "ок") << "\n";

    return 0;
}