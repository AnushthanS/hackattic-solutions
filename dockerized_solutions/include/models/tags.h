#pragma once
#include <glaze/glaze.hpp>

#include <string>
#include <vector>

struct Tags {
    std::string name;
    std::vector<std::string> tags;
};

template <> struct glz::meta<Tags> {
    using T = Tags;

    static constexpr auto value = object("name", &T::name, "tags", &T::tags);
};
