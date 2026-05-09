#pragma once
#include <glaze/glaze.hpp>
#include <string>
#include <vector>

struct Catalogs {
    std::vector<std::string> repositories;
};

template <> struct glz::meta<Catalogs> {
    using T = Catalogs;

    static constexpr auto value = object("repositories", &T::repositories);
};
