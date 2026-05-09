#include <docker.h>
#include <json-get.h>
#include <models/problem.h>
#include <registry.h>
#include <trigger-push.h>

#include <cpr/cpr.h>
#include <cstdlib>
#include <glaze/glaze.hpp>
#include <iostream>
#include <string>

int main() {

    Problem problem{};

    TriggerGet(problem);

    TriggerPush(problem);

    std::string host = std::getenv("REG_HOST");

    auto catalog = fetchCatalogs(host);

    if (catalog.repositories.empty()) {
        std::cerr << "No repositories found\n";
        return 1;
    }

    auto tags = fetchTags(host, catalog.repositories[0]);

    dockerLogin(host, problem.credential.user, problem.credential.password);

    std::string secret;
    for (const auto &tag : tags.tags) {

        std::string image = host + "/" + catalog.repositories[0] + ":" + tag;

        dockerPull(image);

        auto output = dockerRun(image, problem.ignition_key);

        if (output.find("oops, wrong image!") != std::string::npos) {
            continue;
        }

        secret = output;
        break;
    }

    if (!secret.empty()) {
        secret.erase(secret.find_last_not_of(" \n\r\t") + 1);
        std::cout << "Secret found:" << secret << std::endl;

        Solution solution{.secret = secret};
        std::string body{};

        auto error = glz::write_json(solution, body);

        std::string access_token = std::getenv("ACCESS_TOKEN");

        std::string solution_url = "https://hackattic.com"
                                   "/challenges/dockerized_solutions/"
                                   "solve?access_token=" +
                                   access_token;

        cpr::Response response = cpr::Post(
            cpr::Url{solution_url},
            cpr::Header{{"Content-Type", "application/json"}}, cpr::Body{body});

        std::cout << "Response status: " << response.status_code << std::endl;
        std::cout << "Body text: " << response.text << std::endl;
    }
    return 0;
}
