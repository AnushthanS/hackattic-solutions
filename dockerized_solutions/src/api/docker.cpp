#include <array>
#include <docker.h>
#include <memory>

std::string exec(const std::string &cmd) {
    std::array<char, 128> buffer;

    std::string result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"),
                                                  pclose);

    while (fgets(buffer.data(), buffer.size(), pipe.get())) {
        result += buffer.data();
    }
    return result;
}

int dockerLogin(const std::string &host, const std::string &user,
                const std::string &password) {
    exec("podman login" + host + " -u" + user + " -p" + password);

    return 1;
}

void dockerPull(const std::string &image) { exec("podman pull" + image); }

std::string dockerRun(const std::string &image,
                      const std::string &ignitionKey) {
    return exec("podman run --rm "
                "-e IGNITION_KEY=" +
                ignitionKey + " " + image);
}
