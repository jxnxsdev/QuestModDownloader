#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "web-utils/shared/WebUtils.hpp"
#include "web-utils/shared/Response.hpp"
#include "web-utils/shared/DownloaderUtility.hpp"
#include "rapidjson-macros/shared/macros.hpp"
#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "constants.hpp"

/* example:
 {
 "1.35.0_8016709773": [
    {
      "name": "HeartBeatLanReceiver",
      "description": "Receive and display heart rate data from an external apk. The apk can be installed in quest headset itself or an android device via local network. ",
      "id": "HeartBeatLanReceiver",
      "version": "0.2.0",
      "download": "https://github.com/frto027/HeartBeatLanClientBSQuest/releases/download/v0.2.0/HeartBeatLanReceiver.qmod",
      "source": "https://github.com/frto027/HeartBeatLanClientBSQuest/",
      "author": "frto027",
      "cover": "https://raw.githubusercontent.com/frto027/HeartBeatLanClientBSQuest/master/cover.png",
      "modloader": "Scotland2"
    }
    ]
}*/

namespace ModDownloader::Loader {

    class ModVersion {
    public:
        std::string name;
        std::string description;
        std::string id;
        std::string version;
        std::string download;
        std::string source;
        std::string author;
        std::string cover;
        std::string modloader;
    };

    class Mod {
    public:
        std::string name;
        std::string description;
        std::string id;
        std::string source;
        std::string author;
        std::string cover;
        std::string modloader;
        std::vector<ModVersion> versions;
    };

    std::vector<Mod> getMods();
}
