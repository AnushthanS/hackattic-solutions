#pragma once
#include <glaze/glaze.hpp>
#include <string>

/*
 * Data structures for glaze parsing
 */
struct Credentials {
    std::string user;
    std::string password;
};

struct Problem {
    Credentials credential;
    std::string ignition_key;
    std::string trigger_token;
};

/**
 * Glaze metadata to auto parse into the struct
 * */

template <> struct glz::meta<Credentials> {
    using T = Credentials;

    static constexpr auto value =
        object("user", &T::user, "password", &T::password);
};

template <> struct glz::meta<Problem> {
    using T = Problem;
    static constexpr auto value =
        object("credentials", &T::credential, "ignition_key", &T::ignition_key,
               "trigger_token", &T::trigger_token);
};

struct Solution {
    std::string secret;
};

template <> struct glz::meta<Solution> {
    using T = Solution;

    static constexpr auto value = object("secret", &T::secret);
};
