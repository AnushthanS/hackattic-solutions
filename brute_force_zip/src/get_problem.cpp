#include <cpr/cpr.h>
#include <glaze/glaze.hpp>
#include <iostream>
#include <filesystem>
#include <string>

struct Problem {
    std::string zip_url;
};

template <>
struct glz::meta<Problem> {
    using T = Problem;

    static constexpr auto value = object(
        "zip_url", &T::zip_url // "key2", &T::key2, "key3", &T::key3 ...
    );
};

int main()
{
    cpr::Response response = cpr::Get (
        cpr::Url{
            "https://hackattic.com/challenges/brute_force_zip/"
            "problem?access_token=" + std::string(std::getenv("ACCESS_TOKEN"))
        }
    );

    if (response.status_code != 200)
    {
        std::cerr << "HTTP request failed, code returned: "
        << response.status_code << std::endl;
        std::cerr << "Body: " << response.text << std::endl;
        return 1;
    }

    Problem problem;

    auto error = glz::read_json(problem, response.text);

    if (error)
    {
        std::cerr << "JSON parsing error: " << error << std::endl;
        return 1;
    }

    std::filesystem::path root = PROJECT_ROOT; // declared in cmakelists
    std::filesystem::path path = root / "build" / "problem.zip";
    
    std::ofstream of(path, std::ios::binary);

    response = cpr::Get(
        cpr::Url(problem.zip_url),
        cpr::WriteCallback{[&of](std::string_view data, intptr_t userdata){
            of.write(data.data(), data.size());
            return of.good();
        }}
    );

    if (response.status_code >= 200 && response.status_code < 300) {
        std::cout << "Success! Saved to " << path << std::endl;
    } else {
        std::cerr << "Download failed. HTTP Status: " << response.status_code << std::endl;
        if (response.error) std::cerr << "Error message: " << response.error.message << std::endl;
        return 1;
    }
    return 0;
} 