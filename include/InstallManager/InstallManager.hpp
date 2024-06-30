#include <iostream>
#include <vector>
#include <string>
#include "Loaders/localloader.hpp"
#include "Loaders/loader.hpp"

namespace ModDownloader::InstallManager {
    std::vector<LocalLoader::LocalMod> scheduledForDeletion;
    std::vector<LocalLoader::LocalMod> scheduledForUpdate;
    std::vector<Loader::ModVersion> scheduledForInstall;

    class DeletionResponse {
    public:
        bool success;
        std::string message;
    };

    class UpdateResponse {
    public:
        bool success;
        std::string message;
    };

    class InstallResponse {
    public:
        bool success;
        std::string message;
    };

    class ApplyResponse {
    public:
        bool success;
        std::string message;
    };

    DeletionResponse scheduleDeleteMod(std::string id);
    UpdateResponse scheduleUpdateMod(std::string id);
    InstallResponse scheduleInstallMod(std::string id, std::string version);
    ApplyResponse applyChanges();
}