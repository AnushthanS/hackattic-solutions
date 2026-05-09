#pragma once
#include <glaze/glaze.hpp>
#include <string>

// Metadata templates for glaze
struct TriggerRequest {
    std::string registry_host;
};

template <> struct glz::meta<TriggerRequest> {
    using T = TriggerRequest;

    static constexpr auto value = object("registry_host", &T::registry_host);
};
