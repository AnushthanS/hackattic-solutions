#include <registry.h>

#include <cpr/cpr.h>
#include <glaze/glaze.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <models/repo-catalog.h>
#include <models/tags.h>

Catalogs fetchCatalogs(const std::string &host) {
    cpr::Response response =
        cpr::Get(cpr::Url{"https://" + host + "/v2/_catalog"});

    Catalogs catalogs{};

    auto error = glz::read_json(catalogs, response.text);

    if (error) {
        std::cerr << "Error when parsing catalogs response" << std::endl;
        return {};
    }

    return catalogs;
}

Tags fetchTags(const std::string &host, const std::string &repo) {
    cpr::Response response =
        cpr::Get(cpr::Url{"https://" + host + "/v2/" + repo + "/tags/list"});

    Tags tags{};

    auto error = glz::read_json(tags, response.text);

    if (error) {
        std::cerr << "Failed to parse tags" << std::endl;
        return {};
    }
    return tags;
}
