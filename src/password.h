// Demian Vasilev (Okean Studio), 08.11.2026, password-gen
#pragma once

#include <string>

struct PasswordOptions {
    int length = 16;
    bool useLower = true;
    bool useUpper = true;
    bool useDigits = true;
    bool useSymbols = false;
    bool avoidAmbiguous = false;
};

std::string generatePassword(const PasswordOptions& opts);

double estimateEntropy(const std::string& password);

bool isWeakPassword(const std::string& password);