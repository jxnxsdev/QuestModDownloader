#include "WebServer/WebServer.hpp"
#include "assets.hpp"
#include "scotland2/shared/modloader.h"
#include "constants.hpp"
#include "web-utils/shared/DownloaderUtility.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <filesystem>
#include "main.hpp"
#include "loaders/localloader.hpp"
#include "loaders/loader.hpp"
#include "UnityEngine/Application.hpp"

namespace WebServer {
    bool wsrunning = false;

    bool downloadFrontend() {
        std::string ModDataDir = ModDownloader::Constants::ModDataDir;
        std::string frontendDir = ModDataDir + "/frontend";

        // Remove existing frontend directory if it exists
        if (std::filesystem::exists(frontendDir)) {
            std::filesystem::remove_all(frontendDir);
        }

        // Create a new frontend directory
        std::filesystem::create_directories(frontendDir);

        std::string frontendPkg = ModDataDir + "/frontend.qpkg";
        if (std::filesystem::exists(frontendPkg)) {
            std::filesystem::remove(frontendPkg);
        }

        std::string frontendURL = ModDownloader::Constants::GithubRepo + "/releases/download/" + ModDownloader::Constants::CurrentVersion + "/frontend.qpkg";

        WebUtils::DownloaderUtility downloader;
        WebUtils::URLOptions options = WebUtils::URLOptions(frontendURL);
        options.useSSL = false;
        auto response = downloader.GetAsync<WebUtils::StringResponse>(options);
        if (!response.valid()) {
            PaperLogger.error("Failed to download frontend.qpkg");
            return false;
        }

        auto res = response.get();
        if (!res.IsSuccessful()) {
            PaperLogger.error("Failed to download frontend.qpkg");
            return false;
        }

        std::ofstream frontendFile(frontendPkg, std::ios::binary);
        frontendFile << res.responseData.value();
        frontendFile.close();

        std::filesystem::copy(frontendPkg, frontendDir + "/frontend.zip");
        std::string unzipCmd = "unzip -o " + frontendDir + "/frontend.zip -d " + frontendDir;
        system(unzipCmd.c_str());
        std::filesystem::remove(frontendDir + "/frontend.zip");
        std::filesystem::remove(frontendPkg);

        return true;
    }


    void start() {
        if (wsrunning) return;
        wsrunning = true;
        new std::thread([]() {
            httplib::Server server;

            bool success = false;
            std::string ModDataDir = ModDownloader::Constants::ModDataDir;
            try {
                std::ifstream versionfile(ModDataDir + "/frontend/version.dat");
                std::string version;
                versionfile >> version;
                if (version != ModDownloader::Constants::CurrentVersion) {
                    success = downloadFrontend();
                } else {
                    success = true;
                }
            } catch (const std::exception& e) {
                success = downloadFrontend();
            }

            server.Get("/api/getMods", [](const httplib::Request& req, httplib::Response& res) {
                auto mods = ModDownloader::Loader::getMods();
                // convert vector to json and return
                rapidjson::Document doc;
                doc.SetObject();
                rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
                rapidjson::Value modArray(rapidjson::kArrayType);

                for (auto& mod : mods) {
                    rapidjson::Value modObj(rapidjson::kObjectType);
                    modObj.AddMember("name", rapidjson::StringRef(mod.name.c_str()), allocator);
                    modObj.AddMember("description", rapidjson::StringRef(mod.description.c_str()), allocator);
                    modObj.AddMember("id", rapidjson::StringRef(mod.id.c_str()), allocator);
                    modObj.AddMember("source", rapidjson::StringRef(mod.source.c_str()), allocator);
                    modObj.AddMember("author", rapidjson::StringRef(mod.author.c_str()), allocator);
                    modObj.AddMember("cover", rapidjson::StringRef(mod.cover.c_str()), allocator);
                    modObj.AddMember("modloader", rapidjson::StringRef(mod.modloader.c_str()), allocator);
                    rapidjson::Value versionArray(rapidjson::kArrayType);
                    for (auto& version : mod.versions) {
                        rapidjson::Value versionObj(rapidjson::kObjectType);
                        versionObj.AddMember("name", rapidjson::StringRef(version.name.c_str()), allocator);
                        versionObj.AddMember("description", rapidjson::StringRef(version.description.c_str()), allocator);
                        versionObj.AddMember("id", rapidjson::StringRef(version.id.c_str()), allocator);
                        versionObj.AddMember("version", rapidjson::StringRef(version.version.c_str()), allocator);
                        versionObj.AddMember("download", rapidjson::StringRef(version.download.c_str()), allocator);
                        versionObj.AddMember("source", rapidjson::StringRef(version.source.c_str()), allocator);
                        versionObj.AddMember("author", rapidjson::StringRef(version.author.c_str()), allocator);
                        versionObj.AddMember("cover", rapidjson::StringRef(version.cover.c_str()), allocator);
                        versionObj.AddMember("modloader", rapidjson::StringRef(version.modloader.c_str()), allocator);
                        versionArray.PushBack(versionObj, allocator);
                    }
                    modObj.AddMember("versions", versionArray, allocator);
                    modArray.PushBack(modObj, allocator);
                }

                doc.AddMember("mods", modArray, allocator);

                rapidjson::StringBuffer buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

                doc.Accept(writer);

                res.set_content(buffer.GetString(), "application/json");
            });

            server.Get("/api/getLocalMods", [](const httplib::Request& req, httplib::Response& res) {
                auto mods = ModDownloader::LocalLoader::getLocalMods();
                // convert vector to json and return
                rapidjson::Document doc;
                doc.SetObject();
                rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
                rapidjson::Value modArray(rapidjson::kArrayType);
                for (auto& mod : mods) {
                    rapidjson::Value modObj(rapidjson::kObjectType);
                    modObj.AddMember("name", rapidjson::StringRef(mod.name.c_str()), allocator);
                    modObj.AddMember("version", rapidjson::StringRef(mod.version.c_str()), allocator);
                    modObj.AddMember("path", rapidjson::StringRef(mod.path.c_str()), allocator);
                    modObj.AddMember("hasUpdate", mod.hasUpdate, allocator);
                    modObj.AddMember("updateVersion", rapidjson::StringRef(mod.updateVersion.c_str()), allocator);
                    modArray.PushBack(modObj, allocator);
                }

                doc.AddMember("mods", modArray, allocator);

                rapidjson::StringBuffer buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                doc.Accept(writer);

                res.set_content(buffer.GetString(), "application/json");
            });

            server.Get("/api/installMod", [](const httplib::Request& req, httplib::Response& res) {
                std::string downloadUrl = req.get_param_value("downloadUrl");

                // TODO: Add download and install logic
            });

            server.Get("/api/uninstallMod", [](const httplib::Request& req, httplib::Response& res) {
                std::string modId = req.get_param_value("modId");

                // TODO: Add uninstall logic
            });

            server.Get("/api/updateMod", [](const httplib::Request& req, httplib::Response& res) {
                std::string modId = req.get_param_value("modId");

                // TODO: Add update logic
            });

            server.Get("/api/restartGame", [](const httplib::Request& req, httplib::Response& res) {
                UnityEngine::Application::Quit();
            });

            server.Get("/api/getGameVersion", [](const httplib::Request& req, httplib::Response& res) {
                std::string json = "{\"version\": \"" + ModDownloader::Constants::CurrentVersion + "\"}";
            });

            server.Get("/api/getFrontendVersion", [](const httplib::Request& req, httplib::Response& res) {
                std::string ModDataDir = ModDownloader::Constants::ModDataDir;
                std::ifstream versionfile(ModDataDir + "/frontend/version.dat");
                std::string version;
                versionfile >> version;
                std::string json = "{\"version\": \"" + version + "\"}";
            });

            if (!success) {
                PaperLogger.error("Failed to download frontend.qpkg");
                server.Get("/", [](const httplib::Request& req, httplib::Response& res) {
                    const std::string_view htmlContent = (const std::string_view &) IncludedAssets::error_html;
                    res.set_content(htmlContent.data(), htmlContent.size(), "text/html");
                });
                server.listen("0.0.0.0", 2777);
                return;
            }

            server.set_base_dir(ModDataDir + "/frontend");

            server.listen("0.0.0.0", 2777);
        });
    }
}
