#pragma once

#include <models/repo-catalog.h>
#include <models/tags.h>

Catalogs fetchCatalogs(const std::string &host);
Tags fetchTags(const std::string &host, const std::string &repo);
