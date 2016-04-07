//
// Created by alesapin on 07.04.16.
//

#ifndef COREFRESOLUTION_UTIL_H
#define COREFRESOLUTION_UTIL_H
#include <string>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <set>
#include "Document.h"
#include <initializer_list>

namespace util {
    void getdir(std::string dir, std::vector<std::string> &files, const std::set<std::string> &except);
    std::vector<std::string> getFiles(std::initializer_list<std::string> paths,
                                      const std::set<std::string> &except = std::set<std::string>());
}

#endif //COREFRESOLUTION_UTIL_H
