#include "Loaders/localloader.hpp"
#include "Loaders/loader.hpp"

namespace ModDownloader::LocalLoader {

    std::vector<LocalMod> getLocalMods() {
        auto available_mods = ModDownloader::Loader::getMods();
        auto mods = modloader_get_loaded();

        std::vector<LocalMod> localMods;
        for (int i = 0; i < mods.size; i++) {
            auto mod = mods.array[i];
            LocalMod localMod;

            // check if there is a newer version of the mod available
            for(auto& available_mod : available_mods) {
                if(available_mod.id == mod.info.id) {
                    for(auto& version : available_mod.versions) {
                        std::string available_version = version.version;
                        std::string local_version = mod.info.version;
                        available_version.erase(std::remove(available_version.begin(), available_version.end(), '.'), available_version.end());
                        local_version.erase(std::remove(local_version.begin(), local_version.end(), '.'), local_version.end());
                        if(std::stoi(available_version) > std::stoi(local_version)) {
                            localMod.hasUpdate = true;
                            localMod.updateVersion = version.version;
                        }
                    }
                }
            }

            localMod.name = mod.info.id;
            localMod.version = mod.info.version;
            localMod.path = mod.path;
            localMods.push_back(localMod);
        }
        return localMods;
        return {};
    }
}