#include "include/password_generator.h"
#include <fstream>
#include <string>
#include <vector>

PasswordConverter::PasswordConverter(const std::string charset) {
    setCharset(charset);
    initializeLookup();
}

void PasswordConverter::initializeLookup() {
    std::fill(lookup.begin(), lookup.end(), -1);

    for (int i = 0; i < CHARSET.size(); i++) {
        unsigned char c = static_cast<unsigned char>(CHARSET[i]);
        lookup[c] = i;
    }
}

std::string PasswordConverter::intToPassword(int val, int length) const {
    std::string buffer(length, CHARSET[0]);

    for (int i = 0; i < length; i++) {
        int idx = val % CHARSET.size();
        buffer[length - i - 1] = CHARSET[idx];
        val /= CHARSET.size();
    }

    return buffer;
}

int PasswordConverter::passwordToInt(const std::string &password) const {
    int ans = 0;

    for (char c : password) {
        ans = ans * CHARSET.size() + getIndex(c);
    }
    return ans;
}

void PasswordConverter::incrementPassword(std::string &password) {
    bool carry = false;

    int i = password.size() - 1;
    do {
        char c = password[i];
        if (c == CHARSET[CHARSET.size() - 1]) {
            carry = true;
            password[i] = CHARSET[0];
        } else {
            int idx = getIndex(c);
            password[i] = CHARSET[idx + 1], carry = false;
        }
        i--;
    } while (carry && i >= 0);

    if (carry)
        password = CHARSET[0] + password;
}

bool PasswordFileManager::dumpToFile(const std::vector<std::string> &passwords,
                                     const std::string &filename, bool append) {
    std::ofstream outFile(filename, append ? std::ios::app : std::ios::out);
    if (!outFile.is_open())
        return false;

    for (const auto &pw : passwords)
        outFile << pw << '\n';

    return true;
}
