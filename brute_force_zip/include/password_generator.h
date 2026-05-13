#pragma once
#include <array>
#include <string>
#include <vector>

class PasswordConverter {
  private:
    std::string CHARSET;
    std::array<int, 256> lookup;

    void initializeLookup();

  public:
    PasswordConverter(const std::string charset);

    void setCharset(const std::string charset) { CHARSET = charset; }

    std::string numToPassword(long val, int length) const;
    long passwordToNum(const std::string &password) const;

    void incrementPassword(std::string &password);

    int getIndex(char c) const { return lookup[static_cast<unsigned char>(c)]; }
};

class PasswordFileManager {
  public:
    static bool dumpToFile(const std::vector<std::string> &passwords,
                           const std::string &filename, bool append = true);
};
