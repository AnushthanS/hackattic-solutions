#include <cpr/cpr.h>
#include <glaze/glaze.hpp>

#include <filesystem>
#include <iostream>
#include <json-get.h>
#include <models/problem.h>
#include <string>

#include <cstdlib>

int TriggerGet(Problem &problem) {
    cpr::Response response = cpr::Get(
        cpr::Url{"https://hackattic.com/challenges/dockerized_solutions/"
                 "problem?access_token=" +
                 std::string(std::getenv("ACCESS_TOKEN"))});

    if (response.status_code != 200) {
        std::cerr << "HTTP request not successful, code returned: "
                  << response.status_code << std::endl;
        std::cerr << "Body: " << response.text << std::endl;
        return 1;
    }

    auto error = glz::read_json(problem, response.text);

    if (error) {
        std::cerr << "JSON parsing error: " << error << std::endl;
        return 1;
    }
    // PROJECT_ROOT declared in CMakeList
    std::filesystem::path root = PROJECT_ROOT;

    std::filesystem::path path = root / "data-dump" / "problem.json";
    std::ofstream out(path);
    out << response.text;

    std::cout << "Output saved to " << path << std::endl;
    return 0;
}
