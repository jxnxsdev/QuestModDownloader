#include <iostream>
#include <fstream>
#include <filesystem>
#include "constants.hpp"
#include "main.hpp"

namespace ModDownloader::InstallManager::MbfInterface {

    class MbfModDependency {
    public:
        std::string id;
        std::string version;
        std::string download;
    };

    class MbfMod {
    public:
        std::string name;
        std::string id;
        std::string version;
        std::string path;
        bool isCore = false;
        std::vector<MbfModDependency> dependencies;
    };

    std::vector<MbfMod> mbfMods;

    void parseMods() {
        PaperLogger.info("Parsing Mbf mods...");
        const std::string mbfPath = ModDownloader::Constants::MBFPath;
        const std::string tempPath = mbfPath + "/moddl_temp";

        if (std::filesystem::exists(tempPath)) {
            PaperLogger.info("Deleting temp folder...");
            std::filesystem::remove_all(tempPath);
        }

        std::filesystem::create_directory(tempPath);

        for (const auto& entry : std::filesystem::directory_iterator(mbfPath)) {
            if (entry.path().extension() != ".qmod") {
                continue;
            }

            const std::string modPath = entry.path().string();
            const std::string zipPath = tempPath + "/" + entry.path().filename().string() + ".zip";
            const std::string extractPath = tempPath + "/" + entry.path().filename().string();

            std::filesystem::copy(modPath, zipPath);
            std::filesystem::create_directory(extractPath);

            const std::string command = "unzip " + zipPath + " -d " + extractPath;
            system(command.c_str());

            const std::string modJsonPath = extractPath + "/mod.json";
            std::ifstream modJsonFile(modJsonPath);
            if (!modJsonFile.is_open()) {
                PaperLogger.error("Failed to open mod.json for mod at {}", modPath);
                continue;
            }

            std::string modJsonString((std::istreambuf_iterator<char>(modJsonFile)),
                                      std::istreambuf_iterator<char>());
            modJsonFile.close();

            rapidjson::Document modJson;
            modJson.Parse(modJsonString.c_str());
            if (modJson.HasParseError()) {
                PaperLogger.error("Failed to parse mod.json for mod at {}", modPath);
                continue;
            }

            MbfMod newMod;
            newMod.name = modJson["name"].GetString();
            newMod.id = modJson["id"].GetString();
            newMod.version = modJson["version"].GetString();
            newMod.path = modPath;

            if (modJson.HasMember("dependencies")) {
                for (const auto& dependency : modJson["dependencies"].GetArray()) {
                    MbfModDependency newDependency;
                    newDependency.id = dependency["id"].GetString();
                    newDependency.version = dependency["version"].GetString();
                    newDependency.download = dependency["downloadIfMissing"].GetString();
                    newMod.dependencies.push_back(newDependency);
                }
            }

            if (entry.path().filename().string().find("-CORE.qmod") != std::string::npos) {
                newMod.isCore = true;
            }

            mbfMods.push_back(newMod);

            PaperLogger.info("Found Mbf mod: {} ({})", newMod.name, newMod.id);
        }

        PaperLogger.info("Deleting temp folder...");
        std::filesystem::remove_all(tempPath);
    }

    std::vector<MbfMod> getMbfMods() {
        return mbfMods;
    }

    bool ensureMbfModded() {
        if (!std::filesystem::exists(ModDownloader::Constants::MBFPath)) {
            PaperLogger.error("MBF directory does not exist");
            return false;
        }

        if (std::filesystem::is_empty(ModDownloader::Constants::MBFPath)) {
            PaperLogger.error("MBF directory is empty");
            return false;
        }
    }
}