// Demian Vasilev (Okean Studio), 08.11.2026, password-gen
#include "password.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

namespace {

std::string buildAlphabet(const PasswordOptions& opts) {
    std::string alphabet;

    if (opts.useLower)   alphabet += "abcdefghijklmnopqrstuvwxyz";
    if (opts.useUpper)   alphabet += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (opts.useDigits)  alphabet += "0123456789";
    if (opts.useSymbols) alphabet += "!@#$%^&*()-_=+[]{};:,.<>?";

    if (opts.avoidAmbiguous) {
        const std::string ambiguous = "l1IO0";
        alphabet.erase(
            std::remove_if(alphabet.begin(), alphabet.end(),
                [&](char c) { return ambiguous.find(c) != std::string::npos; }),
            alphabet.end());
    }
    return alphabet;
}

} // namespace

std::string generatePassword(const PasswordOptions& opts) {
    if (opts.length <= 0) return {};

    const std::string alphabet = buildAlphabet(opts);
    if (alphabet.empty()) return {};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dist(0, alphabet.size() - 1);

    std::string password;
    password.reserve(opts.length);
    for (int i = 0; i < opts.length; ++i) {
        password += alphabet[dist(gen)];
    }
    return password;
}

double estimateEntropy(const std::string& password) {
    if (password.empty()) return 0.0;

    bool hasLower = false, hasUpper = false, hasDigit = false, hasSymbol = false;
    for (char c : password) {
        if (std::islower(static_cast<unsigned char>(c)))      hasLower = true;
        else if (std::isupper(static_cast<unsigned char>(c))) hasUpper = true;
        else if (std::isdigit(static_cast<unsigned char>(c))) hasDigit = true;
        else                                                   hasSymbol = true;
    }

    int alphabetSize = 0;
    if (hasLower)  alphabetSize += 26;
    if (hasUpper)  alphabetSize += 26;
    if (hasDigit)  alphabetSize += 10;
    if (hasSymbol) alphabetSize += 32;

    if (alphabetSize == 0) return 0.0;
    return password.size() * std::log2(static_cast<double>(alphabetSize));
}

bool isWeakPassword(const std::string& password) {
    if (password.size() < 8) return true;

    int run = 1;
    for (std::size_t i = 1; i < password.size(); ++i) {
        if (password[i] == password[i - 1]) {
            if (++run >= 3) return true;
        } else {
            run = 1;
        }
    }

    int seq = 1;
    for (std::size_t i = 1; i < password.size(); ++i) {
        if (password[i] == password[i - 1] + 1) {
            if (++seq >= 3) return true;
        } else {
            seq = 1;
        }
    }
    return false;
}