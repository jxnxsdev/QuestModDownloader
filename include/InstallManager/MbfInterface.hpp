#include <iostream>
#include <fstream>
#include "constants.hpp"
#include "main.hpp"
#include "filesystem"

namespace ModDownloader::InstallManager::MbfInterface {
    class MbfMod {
    public:
        std::string name;
        std::string id;
        std::string version;
        std::string path;
        bool isCore = false;
    };

    std::vector<MbfMod> mbfMods;

    void parseMods() {
        PaperLogger.info("Parsing Mbf mods...");
        std::string mbfPath = ModDownloader::Constants::MBFPath;

        std::string tempPath = mbfPath + "/moddl_temp";
        if (std::filesystem::exists(tempPath)) {
            PaperLogger.info("Deleting temp folder...");
            std::filesystem::remove_all(tempPath);
        }

        std::filesystem::create_directory(tempPath);

        // for each file with .qmod in the directory
        for (const auto & entry : std::filesystem::directory_iterator(mbfPath)) {
            if (!(entry.path().extension() == ".qmod")) {
                continue;
            }

            std::string modPath = entry.path().string();

            // copy the file to the temp folder as .zip and extract it
            std::filesystem::copy(modPath, tempPath + "/" + entry.path().filename().string() + ".zip");
            std::string zipPath = tempPath + "/" + entry.path().filename().string() + ".zip";
            std::string extractPath = tempPath + "/" + entry.path().filename().string();
            std::filesystem::create_directory(extractPath);
            std::string command = "unzip " + zipPath + " -d " + extractPath;
            system(command.c_str());

            // read the mod.json file as text
            std::string modJsonPath = extractPath + "/mod.json";
            std::ifstream modJsonFile(modJsonPath);
            std::string modJsonString;
            modJsonFile.seekg(0, std::ios::end);
            modJsonString.reserve(modJsonFile.tellg());
            modJsonFile.seekg(0, std::ios::beg);
            modJsonString.assign((std::istreambuf_iterator
                <char>(modJsonFile)),
                std::istreambuf_iterator<char>());
            modJsonFile.close();

            // parse the mod.json file
            rapidjson::Document modJson;
            modJson.Parse(modJsonString.c_str());
            if (modJson.HasParseError()) {
                PaperLogger.error("Failed to parse mod.json for mod at {0}", modPath);
                continue;
            }

            // create a new MbfMod object and add it to the list
            MbfMod newMod;
            newMod.name = modJson["name"].GetString();
            newMod.id = modJson["id"].GetString();
            newMod.version = modJson["version"].GetString();
            newMod.path = modPath;

            // check if the original .qmod ends in -CORE.qmod
            if (entry.path().filename().string().find("-CORE.qmod") != std::string::npos) {
                newMod.isCore = true;
            }

            mbfMods.push_back(newMod);

            PaperLogger.info("Found Mbf mod: {0} ({1})", newMod.name, newMod.id);
        }

        // delete the temp folder
        PaperLogger.info("Deleting temp folder...");
        std::filesystem::remove_all(tempPath);
    }

    std::vector<MbfMod> getMbfMods() {
        return mbfMods;
    }
}