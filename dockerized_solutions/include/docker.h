#pragma once

#include <string>

std::string exec(const std::string &cmd);

int dockerLogin(const std::string &host, const std::string &user,
                const std::string &password);

void dockerPull(const std::string &image);

std::string dockerRun(const std::string &image,
                      const std::string &ignition_key);
