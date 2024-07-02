#include "InstallManager/InstallManager.hpp"
#include "InstallManager/MbfInterface.hpp"

namespace ModDownloader::InstallManager {
    DeletionResponse scheduleDeleteMod(std::string id) {
        auto MbfMods = ModDownloader::InstallManager::MbfInterface::getMbfMods();




        DeletionResponse response;
        response.success = false;
        response.message = "Not implemented";
        return response;
    }

    UpdateResponse scheduleUpdateMod(std::string id) {
        UpdateResponse response;
        response.success = false;
        response.message = "Not implemented";
        return response;
    }

    InstallResponse scheduleInstallMod(std::string id, std::string version) {
        InstallResponse response;
        response.success = false;
        response.message = "Not implemented";
        return response;
    }

    ApplyResponse applyChanges() {
        ApplyResponse response;
        response.success = false;
        response.message = "Not implemented";
        return response;
    }
}