#include "include/password_generator.h"
#include <iostream>
#include <string>

int main() {
    std::string charset = "abcdefghijklmnopqrstuvwxyz0123456789";
    PasswordConverter helper(charset);

    std::cout << "9999: " << helper.passwordToNum("9999") << std::endl;
    std::cout << "99999: " << helper.passwordToNum("99999") << std::endl;
    std::cout << "999999: " << helper.passwordToNum("999999") << std::endl;

    return 0;
}
