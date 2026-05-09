#include <cpr/cpr.h>
#include <glaze/glaze.hpp>

#include <filesystem>
#include <iostream>
#include <models/problem.h>
#include <models/trigger_request.h>
#include <string>
#include <trigger-push.h>

#include <cstdlib>

int TriggerPush(const Problem &problem) {
    TriggerRequest req{.registry_host = std::string(std::getenv("REG_HOST"))};

    std::string body{};
    auto error = glz::write_json(req, body);

    if (error) {
        std::cerr << "Cannot parse req" << std::endl;
        return 1;
    }

    std::string triggerUrl =
        "https://hackattic.com/_/push/" + problem.trigger_token;
    cpr::Response response = cpr::Post(
        cpr::Url{triggerUrl},

        cpr::Header{{"Content-Type", "application/json"}}, cpr::Body{body});

    std::cout << "Response status: " << response.status_code << std::endl;
    std::cout << "Body text: " << response.text << std::endl;
    return 0;
}
