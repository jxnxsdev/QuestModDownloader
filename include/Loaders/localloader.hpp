#pragma once

#include "scotland2/shared/modloader.h"
#include <string>
#include <vector>
#include <iostream>

namespace ModDownloader::LocalLoader {
    class LocalMod {
    public:
        std::string name;
        std::string version;
        std::string path;
        bool hasUpdate = false;
        std::string updateVersion;
    };

    std::vector<LocalMod> getLocalMods();
}