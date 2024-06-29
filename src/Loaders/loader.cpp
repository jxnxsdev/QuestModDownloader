#include "Loaders/loader.hpp"

namespace ModDownloader::Loader {
    std::vector<Mod> getMods() {
        WebUtils::DownloaderUtility downloader;
        WebUtils::URLOptions options = WebUtils::URLOptions(ModDownloader::Constants::ApiURL);
        options.useSSL = false;
        auto response = downloader.GetAsync<WebUtils::JsonResponse>(options);
        if (!response.valid()) {
            return {};
        }
        auto res = response.get();
        if(res.IsSuccessful() && res.DataParsedSuccessful()) {
            auto& doc = res.responseData.value();

            if(doc.HasMember(ModDownloader::Constants::ModVersion.c_str())) {
                auto& mods = doc[ModDownloader::Constants::ModVersion.c_str()];
                std::vector<Mod> modList;
                for(auto& mod : mods.GetArray()) {
                    // check if the mod is already in the list
                    bool found = false;
                    for(auto& modListMod : modList) {
                        if(modListMod.id == mod["id"].GetString()) {
                            found = true;
                            ModVersion version;
                            version.name = mod["name"].GetString();
                            version.description = mod["description"].GetString();
                            version.id = mod["id"].GetString();
                            version.version = mod["version"].GetString();
                            version.download = mod["download"].GetString();
                            version.source = mod["source"].GetString();
                            version.author = mod["author"].GetString();
                            version.cover = mod["cover"].GetString();
                            version.modloader = mod["modloader"].GetString();
                            modListMod.versions.push_back(version);
                            break;
                        }
                    }

                    if(!found) {
                        Mod newMod;
                        newMod.name = mod["name"].GetString();
                        newMod.description = mod["description"].GetString();
                        newMod.id = mod["id"].GetString();
                        newMod.source = mod["source"].GetString();
                        newMod.author = mod["author"].GetString();
                        newMod.cover = mod["cover"].GetString();
                        newMod.modloader = mod["modloader"].GetString();
                        ModVersion version;
                        version.name = mod["name"].GetString();
                        version.description = mod["description"].GetString();
                        version.id = mod["id"].GetString();
                        version.version = mod["version"].GetString();
                        version.download = mod["download"].GetString();
                        version.source = mod["source"].GetString();
                        version.author = mod["author"].GetString();
                        version.cover = mod["cover"].GetString();
                        version.modloader = mod["modloader"].GetString();
                        newMod.versions.push_back(version);
                        modList.push_back(newMod);
                    }
                }
                return modList;
            }
        }

        return {};
    }
}